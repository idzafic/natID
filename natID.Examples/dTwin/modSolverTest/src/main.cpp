// main.cpp : Tests static and dynamic models

#include "ModelSolver.h"
#include <fo/FileOperations.h>
#include <td/StringConverter.h>
#include <mem/PointerReleaser.h>

fo::fs::path s_sdkPath;

enum class Location {Real=0, Complex, Selected};

td::String getModlInput(const char* fileName, Location location)
{
    fo::fs::path path;
    switch (location)
    {
        case Location::Real:
            path = s_sdkPath / "bin/TestData/modSolver/real" / fileName;
            break;
            
        case Location::Complex:
            path = s_sdkPath / "bin/TestData/modSolver/cmplx" / fileName;
            break;
        case Location::Selected:
            path = s_sdkPath / "bin/TestData/modSolver/PowerSystem/PaperExamples" / fileName;
            break;
    }
    td::String str = path.string();
    return str;
}


template <class TNAMES, class TVALS>
inline void showResults(std::ofstream& fOut, const char* lbl, const TNAMES& outNames, const TVALS& vals)
{
    fOut << td::endl;
    fOut << lbl << td::endl;
    fOut << "--------------------" << td::endl;
    fOut << "Name      value" << td::endl;
    fOut << "--------------------" << td::endl;
    auto nVals = outNames.size();
    for (decltype(nVals) i = 0; i < nVals; ++i)
        fOut << outNames[i] << ": " << vals[i] << td::endl;
    fOut << "--------------------" << td::endl;
}


inline void showResHeader(std::ofstream&fOut, sc::IRealDynamicModel::NameVector& outNames, const char* lbl = nullptr)
{
    if (lbl)
        fOut << lbl << td::endl;
    
    fOut << "t";
    for (const auto& name : outNames)
        fOut << " " << name;
    fOut << td::endl;
}

inline void showResRow(std::ofstream&fOut, double t, sc::IRealDynamicModel::ValueVector& values)
{
    fOut << t;
    for (const auto& val : values)
        fOut << " " << val;
    fOut << td::endl;
}

inline void testRealDynamic(sc::IDynamic::Problem problem, const td::String& inFn, const td::String& outFolder, double tFinal, const td::String& paramName = td::String())
{
    sc::ILog* pLog = sc::getConsoleLogger();
    sc::IRealDynamicModel* pModel = sc::createRealDynamicModel(problem, pLog);
    assert(pModel);
    mem::PointerReleaser releaser(pModel);
    auto[inFileName, outFileName] = getInOutFileNames(inFn, outFolder);
    if (inFileName.length() == 0 || outFileName.length() == 0)
    {
        std::cout << "ERROR! Wrong input or output location!" << td::endl;
        return;
    }
    
    if (!pModel->initFromFile(inFileName))
    {
        std::cout << "ERROR! Cannot init from file!" << td::endl;
        return;
    }
    
    auto pDynSolver = pModel->getSolverInterface();
    if (!pDynSolver)
    {
        std::cout << "ERROR! Cannot obtain solver interface!" << td::endl;
        return;
    }
    
    //res file
    std::ofstream fOut;
    if (!fo::createTextFile(fOut, outFileName))
    {
        std::cout << "ERROR! Cannot obtain output file name!: " << outFileName << td::endl;
        return;
    }
    
    //check params
    ssize_t paramIndex = -1;
    sc::IRealStaticModel::NameVector paramNames(1);
    sc::IRealStaticModel::IndexVector paramIndices(1);
    sc::IRealStaticModel::ValueVector paramValues;
    
    if (paramName.length() != 0)
    {
        paramIndex = pModel->getParameterIndex(paramName);
        if (paramIndex < 0)
        {
            std::cout << "ERROR! Cannot find param='" << paramName << "' in model parameters" << td::endl;
            return;
        }
        
        paramNames[0] = paramName;
        paramIndices[0] = (unsigned int) paramIndex;
        
        //get parameter values
        pModel->getParameterValues(paramIndices, paramValues);
    }
    
    //check deltaT
    double dT = 0;
    {
        dT = pDynSolver->getStepSize();
        if (dT <= 0)
        {
            std::cout << "Warning! deltaT was not initially specified. Defaut value is used-" << td::endl;
            dT = 0.001;
            pDynSolver->setStepSize(dT);
        }
    }
    //initial reset
    if (!pDynSolver->reset(0))
    {
        std::cout << "ERROR! Cannot reset the problem" << td::endl;
        return;
    }
    
    auto outIndices = pModel->getOutputSymbolIndices();
    if (outIndices.size() == 0)
    {
        std::cout << "ERROR! Cannot obtain output indices!" << td::endl;
        return;
    }
    
    auto outNames = pModel->getOutputSymbolNames(outIndices);
    if (outNames.size() == 0)
    {
        std::cout << "ERROR! Cannot obtain output names!" << td::endl;
        return;
    }
    
    sc::IRealStaticModel::ValueVector outValues;
    if (!pModel->getOutputSymbolValues(outIndices, outValues))
    {
        std::cout << "ERROR! Cannot obtain output solution values!" << td::endl;
        return;
    }
    
    showResHeader(fOut, outNames);
    
    showResRow(fOut, 0, outValues);
    
//    if (paramIndex < 0)
//    {
//        std::cout << "INFO: testRealDynamic completed successfully (without param manipulaitons)" << td::endl;
//        return;
//    }
        

    double t=0;
    
    double epsT = 1e-6;
    
    while (t <= tFinal)
    {
        t += dT;
        
        if (paramIndex >= 0)
        {
            if (t >= 0.5-epsT && t <= 0.5+epsT)
            {
                //update param values
                //at t=0.5s icrease the load 10 times
                paramValues[0] = -10;
                pModel->setParameterValues(paramIndices, paramValues);
            }
            if ( t >= 10-epsT && t <= 10+epsT)
            {
                //at 10s decrease the load
                paramValues[0] = -1;
                pModel->setParameterValues(paramIndices, paramValues);
            }
        }
        
        auto sol = pDynSolver->step();
        if (sol != sc::Solution::OK)
        {
            std::cout << "ERROR! Cannot solve the problem!" << td::endl;
            return;
        }
        pModel->getOutputSymbolValues(outIndices, outValues);
        showResRow(fOut, t, outValues);
    }
    
    std::cout << "INFO: testRealDynamic completed successfully." << td::endl;
}

inline void testRealStatic(sc::IStatic::Problem problem, const td::String& inFn, const td::String& outFolder, const td::String& paramName = td::String(), double paramDelta = -0.5)
{
    sc::ILog* pLog = sc::getConsoleLogger();
    sc::IRealStaticModel* pModel = sc::createRealStaticModel(problem, pLog);
    assert(pModel);
    mem::PointerReleaser releaser(pModel);
    auto[inFileName, outFileName] = getInOutFileNames(inFn, outFolder);
    if (inFileName.length() == 0 || outFileName.length() == 0)
    {
        std::cout << "ERROR! Wrong input or output location!" << td::endl;
        return;
    }
    
    if (!pModel->initFromFile(inFileName))
    {
        std::cout << "ERROR! Cannot init from file!" << td::endl;
        return;
    }
    
    auto outIndices = pModel->getOutputSymbolIndices();
    if (outIndices.size() == 0)
    {
        std::cout << "ERROR! Cannot obtain output indices!" << td::endl;
        return;
    }
    
    auto outNames = pModel->getOutputSymbolNames(outIndices);
    if (outNames.size() == 0)
    {
        std::cout << "ERROR! Cannot obtain output names!" << td::endl;
        return;
    }
    
    auto pStaticSolver = pModel->getSolverInterface();
    if (!pStaticSolver)
    {
        std::cout << "ERROR! Cannot obtain solver interface!" << td::endl;
        return;
    }
    
    //res file
    std::ofstream fOut;
    if (!fo::createTextFile(fOut, outFileName))
    {
        std::cout << "ERROR! Cannot obtain output file name!: " << outFileName << td::endl;
        return;
    }
    
    //check params
    ssize_t paramIndex = -1;
    sc::IRealStaticModel::NameVector paramNames(1);
    sc::IRealStaticModel::IndexVector paramIndices(1);
    sc::IRealStaticModel::ValueVector paramValues;
    
    if (paramName.length() != 0)
    {
        paramIndex = pModel->getParameterIndex(paramName);
        if (paramIndex < 0)
        {
            std::cout << "ERROR! Cannot find param='" << paramName << "' in model parameters" << td::endl;
            return;
        }
        
        paramNames[0] = paramName;
        
        paramIndices[0] = (unsigned int) paramIndex;
        
        //get parameter values
        pModel->getParameterValues(paramIndices, paramValues);
        
        showResults(fOut, "Initial param values:", paramNames, paramValues);
    }
    
    //solve
    sc::Solution sol = pStaticSolver->solve();
    if (sol != sc::Solution::OK)
    {
        std::cout << "ERROR! Cannot solve the problem!" << td::endl;
        return;
    }
    cnt::SafeFullVector<double> vals;
    if (!pModel->getOutputSymbolValues(outIndices, vals))
    {
        std::cout << "ERROR! Cannot obtain output solution values!" << td::endl;
        return;
    }
    
    showResults(fOut, "Output symbols for initial solution:", outNames, vals);
    
    if (paramIndex < 0)
    {
        std::cout << "INFO: testRealStatic completed successfully (without param manipulaitons)" << td::endl;
        return;
    }
    
    //update param values
    paramValues[0] += paramDelta;
    
    pModel->setParameterValues(paramIndices, paramValues);
    
    showResults(fOut, "Updated param values:", paramNames, paramValues);
    
    //solve with updated param values
    sol = pStaticSolver->solve();
    if (sol != sc::Solution::OK)
    {
        std::cout << "ERROR! Cannot solve the problem!" << td::endl;
        return;
    }
    //get new values
    pModel->getOutputSymbolValues(outIndices, vals);
    showResults(fOut, "Output symbols for solution with updated params:", outNames, vals);
    
    std::cout << "INFO: testRealStatic completed successfully." << td::endl;
}

int main(int argc, const char* argv[])
{

    s_sdkPath = mu::getHomePath() / "natID.SDK";
    
    //Select your output folder
    std::cout << "Warning! Adjust output folder here" << std::endl;
#if defined(MU_WINDOWS)
    const char* outputFolder = "R:/Res"; // NOTE: adjust output folder!!
#elif defined(MU_MACOS)
    const char* outputFolder = "/Volumes/RAMDisk/Res"; // NOTE: adjust output folder!!
#else
    // Linux
    const char* outputFolder = "/media/RAMDisk/Res"; // NOTE: adjust output folder!!
#endif
    
    testRealStatic(sc::IStatic::Problem::NLE, getModlInput("PF_PV_03.dmodl", Location::Real), outputFolder, "P3_inj", -0.8);
    
    testRealStatic(sc::IStatic::Problem::NLE, getModlInput("IBR_Switch.dmodl", Location::Real), outputFolder, "Q3_inj", 0.8);
    
    testRealDynamic(sc::IDynamic::Problem::DAE, getModlInput("FreqReg_01.dmodl", Location::Real), outputFolder, 20.0, "P_l");
    
    testRealDynamic(sc::IDynamic::Problem::DAE, getModlInput("TF_Dorf_E760_PD_Disk_RK4.dmodl", Location::Real), outputFolder, 0.5);
    
    return 0;
}
