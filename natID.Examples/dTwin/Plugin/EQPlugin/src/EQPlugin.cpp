#include "EQPlugin.h"
#include <sc/IPlugin.h>
#include "WindowPlugin.h"
#include <td/StringUtils.h>
#include <dense/Matrix.h>
#include <mu/ScopedCLocale.h>

class Plugin : public sc::IPlugin
{
    MemoryArchiveContainer _outArchives;
    WindowPlugin* _pWnd = nullptr;
public:
    Plugin()
    {
        //dont change this
        for (size_t i=0; i< size_t(ArchType::NA); ++i)
            _outArchives[i] = nullptr;
    }
    
    void show(gui::Window* parentWnd, MemoryArchiveContainer& archives, td::UINT4 wndID, const sc::IPlugin::Cleaner& cleaner, const sc::IPlugin::CallBack& onComplete) override final
    {
        //dont change this
        for (size_t i=0; i< size_t(ArchType::NA); ++i)
            _outArchives[i] = archives[i];
        
        if (_pWnd)
            _pWnd->setFocus();
        else
        {
            _pWnd = new WindowPlugin(parentWnd, this, onComplete, cleaner, wndID);
            _pWnd->open();
        }
    }
    
    td::String getMenuName() const override final
    {
        return "EQ 1D Converter";
    }
    
    arch::MemoryOut* getArchive(sc::IPlugin::ArchType type) override final
    {
        //dont change this
        auto iType = size_t(type);
        if (iType >= getMaxSupportedArchiveParts())
            return nullptr;
        
        return _outArchives[size_t(type)];
    }
    
    MemoryArchiveContainer& getArchives() override final
    {
        //dont change this
        return _outArchives;
    }
    
    td::String getOutFileName() const override final
    {
        //dont change this
        assert(_pWnd);
        return _pWnd->getOutFileName();
    }
    
    size_t getMaxSupportedArchiveParts() const override final
    {
        return size_t(ArchType::NA); //don't change this
    }
    
    ModelType getModelType() const override final
    {
        //NOTE: adjust this to match your converter type
        return ModelType::DAE;
    }
    
    void onClosedPluginWindow()
    {
        //dont change this
        _pWnd = nullptr;
    }
    
};

static Plugin s_plugin;

void onClosedPluginWindow()
{
    s_plugin.onClosedPluginWindow();
}

//Plugin requires extern C
extern "C"
{

PLUGIN_API sc::IPlugin* getPluginInterface()
{
    return &s_plugin;
}
}

enum class Format {Unknown=0, Plain, Matlab};

//Converter implemenation
static bool loadMatrices(const td::String& fileName, dense::DblMatrix matrices[3], gui::LineEdit& status)
{
    fo::InFile inFile;
    if (!fo::openExistingBinaryFile(inFile, fileName))
        return false;
    
    cnt::PushBackVector<td::String> cntTokens;
    cntTokens.reserve(16);
    cnt::PushBackVector<td::String> cntRowTokens;
    cntRowTokens.reserve(16);
    cnt::PushBackVector<td::String> cntColTokens;
    cntColTokens.reserve(16);
    
    Format format = Format::Unknown;
    
    fo::LineNormal buffer;
    int line = 0;
    td::UINT4 nRows = 0;
    int iCurrRow = -1;
    int iMatrix = 0;
    
    
    while (fo::getLine(inFile, buffer))
    {
        ++line;
        const char* pBuff = buffer.c_str();
        auto buffLen = buffer.length();
        
        pBuff = td::findFirstNonWhiteSpace(pBuff);
        
        if (!pBuff)
            continue;
        //check first nonwhite space character
        char ch = *pBuff;
        if (ch == 0)
            continue;
        if (ch == '#')  //comment (MATLAB style)
            continue;
        if (ch == '/')  //comment c++ style
            continue;
        
        td::String str(pBuff);
        
        if (format == Format::Unknown)
        {
            int nEq = str.countAppearance('=');
            if (nEq == 0)
            {
                status = "ERROR! Missing '=' sign in format definition!";
                return false;
            }
                
            str.split('=', cntTokens);
            auto nToks = cntTokens.size();
            if (nToks != 2)
            {
                status = "ERROR! Incomplete format declaration in input";
                return false;
            }
            const auto& fmt = cntTokens[0];
            if (!fmt.compareConstStr("format"))
                if (nToks != 2)
                {
                    status = "ERROR! Format declaration must start with 'format ='";
                    return false;
                }
            const auto& fmtType = cntTokens[1];
            if (fmtType.compareCI("MATLAB"))
                format = Format::Matlab;
            else if (fmtType.compareCI("Plain"))
                format = Format::Plain;
            else
            {
                status = "ERROR! Unknown format type. Supported: MATLAB and Plain'";
                return false;
            }
        }
        else
        {
            if (iCurrRow >= int(nRows))
            {
                nRows = 0;
                iCurrRow = -1;
            }
            
            if (iCurrRow < 0)
            {
                int nEq = str.countAppearance('=');
                if (nEq == 0)
                {
                    status = "ERROR! Missing '=' sign in matrix definition!";
                    return false;
                }
                str.split('=', cntTokens);
                auto nToks = cntTokens.size();
                if (nToks != 2)
                {
                    status = "ERROR! Incomplete matrix declaration!";
                    return false;
                }
                const auto& fmt = cntTokens[0];
                if (fmt.compareCI("M"))
                {
                    iMatrix = 0;
                }
                else if (fmt.compareCI("K"))
                {
                    iMatrix = 1;
                }
                else if (fmt.compareCI("C"))
                {
                    iMatrix = 2;
                }
                else
                {
                    status = "ERROR! Unknown matrix name! Supported names: M,K,C";
                    return false;
                }
                if (matrices[iMatrix].getNoOfRows() > 0)
                {
                    status = "ERROR! Matrix entered twice";
                    return false;
                }
                //check second part
                //extract rows
                const auto& secondPart = cntTokens[1];
                auto sec2 = secondPart.replace("[", " ");
                auto sec3 = sec2.replace("]", " ");
                sec3.split(";", cntRowTokens);
                auto nRowsToProcess = cntRowTokens.size();
                if (iCurrRow < 0)
                    iCurrRow = 0;
                for (td::UINT4 iRow = 0; iRow < nRowsToProcess; ++iRow)
                {
                    if (iCurrRow > nRows)
                    {
                        status = "ERROR! Matrix size mismatch";
                        return false;
                    }
                    const auto& row = cntRowTokens[iRow];
                    row.split(" ,", cntColTokens);
                    td::UINT4 nCols = td::UINT4(cntColTokens.size());
                    if (nRows == 0)
                    {
                        nRows = nCols;
                        matrices[iMatrix].reserve(nRows, nRows);
                    }
                    else if (nRows != nCols)
                    {
                        status = "ERROR! Matrix has to be quadratic";
                        return false;
                    }
                    
                    auto mat = matrices[iMatrix].getManipulator();
                    for (td::UINT4 iCol=0; iCol<nRows; ++iCol)
                    {
                        mat(iCurrRow+iRow, iCol) = std::atof(cntColTokens[iCol].c_str());
                    }
                    ++iCurrRow;
                }
            }
            else
            {
                int nEq = str.countAppearance('=');
                if (nEq != 0)
                {
                    status = "ERROR! Duplicated '=' in matrix definition";
                    return false;
                }
                
                if (matrices[iMatrix].getNoOfRows() == 0)
                {
                    status = "ERROR! Matrix should have specified number of rows at first entry";
                    return false;
                }

                auto sec3 = str.replace("]", " ");
                sec3.split(";", cntRowTokens);
                auto nRowsToProcess = cntRowTokens.size();
                if (iCurrRow < 0)
                    iCurrRow = 0;
                for (td::UINT4 iRow = 0; iRow < nRowsToProcess; ++iRow)
                {
                    if (iCurrRow > nRows)
                    {
                        status = "ERROR! Too many rows";
                        return false;
                    }
                    const auto& row = cntRowTokens[iRow];
                    row.split(" ,", cntColTokens);
                    td::UINT4 nCols = td::UINT4(cntColTokens.size());
                    if (nRows == 0)
                    {
                        nRows = nCols;
                        matrices[iMatrix].reserve(nRows, nRows, nullptr, true);
                    }
                    else if (nRows != nCols)
                    {
                        status = "ERROR! Number of rows and columns mismatch";
                        return false;
                    }
                    
                    auto mat = matrices[iMatrix].getManipulator();
                    for (td::UINT4 iCol=0; iCol<nRows; ++iCol)
                    {
                        mat(iCurrRow+iRow, iCol) = std::atof(cntColTokens[iCol].c_str());
                    }
                    ++iCurrRow;
                }
            }
        }

    }
    return true;
}

static void writeAccelorometerData(arch::MemoryOut& memDigitalOut, const td::String& fullOutFleName, const td::String& accelData, td::MutableString& mStr, gui::LineEdit& status)
{
    td::String fullAccDataFileName = fo::replaceFileExtension<false>(fullOutFleName, ".dacc");
    std::ofstream f;
    if (!fo::createTextFile(f, fullAccDataFileName))
    {
        status = "ERROR! Cannto create accelorometer data file!";
        return;
    }
    
    fo::writeString(f, accelData);
    f.close();
    
    td::String accFileName = fo::getFilename(fullAccDataFileName);
    
    memDigitalOut.put("\n\nDataSets:");
    memDigitalOut.put("\n\t// The 'conn' attribute (in case of txt or sqlite type) accepts:");
    memDigitalOut.put("\n\t//   - Full file path (doesn't start with $)");
    memDigitalOut.put("\n\t//   - $ModLoc - relative to model location");
    memDigitalOut.put("\n\t//   - $AppRes - relative to app resource folder");
    memDigitalOut.put("\n\t//   - $Home   - relative to user's home folder");
    memDigitalOut.put("\n\t// The 'conn' attribute, in case of ODBC type, contains full ODBC connection string");
    mStr.appendFormat("\n\tdsAccell [type=txt conn=\"$ModLoc/%s\" data=\"SELECT time, xAcc as ag\"]", accFileName.c_str());
    memDigitalOut.put(mStr.c_str(), mStr.length());
    mStr.reset();
//    memDigitalOut.put("\n\tdsAccell [type=txt conn=\"$ModLoc/EQ_Petrovac_01.dacc\" data=\"SELECT time, xAcc as ax, yAcc as ay\"]");
    memDigitalOut.put("\n");
    memDigitalOut.put("\nSamplers:");
    memDigitalOut.put("\n\taccell [ds=dsAccell param=\"time\" outVals=\"ag\"] -> a_g");
    memDigitalOut.put("\n");
    memDigitalOut.put("\nPreProc:");
    memDigitalOut.put("\n\tsample(accell, t)\n");
}

bool createModel(const td::String& inputFileName, const td::String& outFileName, sc::IPlugin* pIPlugin, const Options& options, gui::LineEdit& status)
{
	mu::ScopedCLocale scopedLocale; //set numerics locale to "C" for the scope of this function (for correct parsing of floating point numbers with '.' as decimal separator)
    dense::DblMatrix matrices[3]; //m,k,c
    if (!loadMatrices(inputFileName, matrices, status))
        return false;
    td::UINT4 N = 0;
    for (size_t i=0; i<3;++i)
    {
        auto nRows = matrices[i].getNoOfRows();
        auto nCols = matrices[i].getNoOfCols();
        if (nRows == 0 || nCols == 0)
        {
            status = "ERROR! Empty matrix";
            return false;
        }
        if (nRows != nCols)
        {
            status = "ERROR! Matrix is not quadratic";
            return false;
        }
        if (i==0)
            N = nRows;
        else if (N != nRows)
        {
            status = "ERROR! Matrices are not of the same size";
            return false;
        }
    }
    
    //create output
    auto pDigitModel = pIPlugin->getArchive(sc::IPlugin::ArchType::DigitalModel);
    auto& memDigitalOut = *pDigitModel;
    td::MutableString mStr;
    mStr.reserve(1024);
    int maxIter = 20;
    float dTime = 0.001;
    float endTime = 60;
    mStr.appendFormat("Header:\n\tmaxIter = %d\t//Maximum number of iterations for NL, WLS, and DAE problems\n\treport = Solver\t//Only successfully solved cases. Other options: All - shows solved and nonSolved with iterations, AllWithDetails - All + debug information.\n\tmaxReps = -1\n\toutToTxt = false\n\ttxtFile = \"\"\n\tstartTime = 0\n\tdTime = %.3f\n\tendTime = %.3f\nend\n//Model created by a plugin-in converter\n", options.maxIter, options.dTime, options.endTime);
    memDigitalOut.put(mStr.c_str(), mStr.length());
    mStr.reset();
    
    td::String modelName = options.modelName;
    modelName.replace('\"', '\''); //in-place replacement of " with '
    mStr.reset();
    mStr.appendFormat("Model [type=DAE domain=real method=RK2 name=\"%s\"]:\nVars [out=true]:\n\t", modelName.c_str());
    memDigitalOut.put(mStr.c_str(), mStr.length());
    mStr.reset();
    
    
    for (td::UINT4 i=1; i<=N; ++i)
        mStr.appendFormat("x_%d; ", i);
    mStr.append("\n\t");
    for (td::UINT4 i=1; i<=N; ++i)
        mStr.appendFormat("v_%d; ", i);
    mStr.append("\n");
    memDigitalOut.put(mStr.c_str(), mStr.length());
    mStr.reset();
    
    auto M = matrices[0].getManipulator1(); //indices are 1-based
    memDigitalOut.put("Params:\n\t");
    for (td::UINT4 i=1; i<=N; ++i)
    {
        bool second = false;
        for (td::UINT4 j=1; j<=N; ++j)
        {
            auto mass = M(i,j);
            if (mass != 0.0)
            {
                if (second)
                    mStr.append("; ");
                else
                    second = true;
                mStr.appendFormat("m_%d%d = %.3f", i, j, mass);
            }
        }
        mStr.append("\n\t");
    }
    memDigitalOut.put(mStr.c_str(), mStr.length());
    mStr.reset();
    
    memDigitalOut.put("\n\t");
    
    auto K = matrices[1].getManipulator1();
    for (td::UINT4 i=1; i<=N; ++i)
    {
        bool second = false;
        for (td::UINT4 j=1; j<=N; ++j)
        {
            auto k = K(i,j);
            if (k != 0.0)
            {
                if (second)
                    mStr.append("; ");
                else
                    second = true;
                mStr.appendFormat("k_%d%d = %.3f", i, j, k);
            }
        }
        mStr.append("\n\t");
    }
    memDigitalOut.put(mStr.c_str(), mStr.length());
    mStr.reset();
    
    memDigitalOut.put("\n\t");
    
    auto C = matrices[2].getManipulator1();
    for (td::UINT4 i=1; i<=N; ++i)
    {
        bool second = false;
        for (td::UINT4 j=1; j<=N; ++j)
        {
            auto c = C(i,j);
            if (c != 0.0)
            {
                if (second)
                    mStr.append("; ");
                else
                    second = true;
                mStr.appendFormat("c_%d%d = %.3f", i, j, c);
            }
        }
        mStr.append("\n\t");
    }
    memDigitalOut.put(mStr.c_str(), mStr.length());
    mStr.reset();
    
    memDigitalOut.put("\n\ta_g = 0\t[out=true]");
    for (td::UINT4 i=1; i<=N; i+=2)
        mStr.appendFormat("\n\ta_%d = 0\t[out=true]", i);
    
    memDigitalOut.put(mStr.c_str(), mStr.length());
    mStr.reset();
    
    if (options.useAccelorometerData)
        writeAccelorometerData(memDigitalOut, outFileName, options.accelorometerData, mStr, status);
    
    mStr.append("\nODEs:\n\t");
    
    
    for (td::UINT4 i=1; i<=N; ++i)
        mStr.appendFormat("x_%d\' = v_%d\n\t", i, i);
    
    memDigitalOut.put(mStr.c_str(), mStr.length());
    mStr.reset();
    
    for (td::UINT4 i=1; i<=N; ++i)
    {
        mStr.appendFormat("v_%d\' =  a_g - (", i);
        bool second = false;
        for (td::UINT4 j=1; j<=N; ++j)
        {
            auto c = C(i,j);
            if (c != 0.0)
            {
                if (second)
                    mStr.append(" + ");
                else
                    second = true;
                mStr.appendFormat("c_%d%d*v_%d", i, j, j);
            }
        }
        for (td::UINT4 j=1; j<=N; ++j)
        {
            auto k = K(i,j);
            if (k != 0.0)
            {
                if (second)
                    mStr.append(" + ");
                else
                    second = true;
                mStr.appendFormat("k_%d%d*x_%d", i, j, j);
            }
        }
        mStr.appendFormat(") / m_%d%d\n\t", i, i);
    }
    mStr.append("\nPostProc:\n\t");
    memDigitalOut.put(mStr.c_str(), mStr.length());
    mStr.reset();
    
    for (td::UINT4 i=1; i<=N; i+=2)
    {
        mStr.appendFormat("a_%d =  a_g - (", i);
        bool second = false;
        for (td::UINT4 j=1; j<=N; ++j)
        {
            auto c = C(i,j);
            if (c != 0.0)
            {
                if (second)
                    mStr.append(" + ");
                else
                    second = true;
                mStr.appendFormat("c_%d%d*v_%d", i, j, j);
            }
        }
        for (td::UINT4 j=1; j<=N; ++j)
        {
            auto k = K(i,j);
            if (k != 0.0)
            {
                if (second)
                    mStr.append(" + ");
                else
                    second = true;
                mStr.appendFormat("k_%d%d*x_%d", i, j, j);
            }
        }
        mStr.appendFormat(") / m_%d%d\n\t", i,i);
    }
    
    memDigitalOut.put(mStr.c_str(), mStr.length());
    mStr.reset();
    
    if (!options.useAccelorometerData)
    {
        mStr.append("\n\tif (t>=0.5) and (t<0.7):    //make some moves between 0.5s and 0.7s");
        mStr.append("\n\t\tif t<0.65:");
        mStr.append("\n\t\t\ta_g=1");
        mStr.append("\n\t\telse:");
        mStr.append("\n\t\t\ta_g=-2");
        mStr.append("\n\t\tend");
        mStr.append("\n\telse:");
        mStr.append("\n\t\ta_g = 0");
        mStr.append("\n\tend");
        mStr.append("\nend\n");
        memDigitalOut.put(mStr.c_str(), mStr.length());
        mStr.reset();
    }
    
    std::ofstream fDigital;
    if (!fo::createTextFile(fDigital, outFileName))
    {
        status = "ERROR! Cannot create output file!";
        return false;
    }
    memDigitalOut.writeToFile(fDigital);
    fDigital.close();
    
    //write visual model
    if (N >= 3)
    {
        auto pVisualModel = pIPlugin->getArchive(sc::IPlugin::ArchType::VisualModel);
        auto& memVisualOut = *pVisualModel;
        
        memVisualOut.put("Header:\n\tnewTab = false     //true:Create new tab whenever figure changes, false: update figure in the existing tab");
        memVisualOut.put("\n\tdrawPlots = true     //if false: plots will not be drawn\nend");
        memVisualOut.put("\nModel [name=\"Visualization of results\"]:");
        memVisualOut.put("\nPlots [backColor=auto]: //backColor: auto - system back color, light - white backcolor, dark - black background color");
        
        memVisualOut.put("\n\t//plot absolute displacements");
        memVisualOut.put("\n\tlinePlot [xLabel=\"Time [s]\" yLabel=\"shifts\" name=\"Shifts\" anchor=TR legend=true nCols=1 anchorX=130 anchorY=35]:");
        memVisualOut.put("\n\t\t@x << t");
        memVisualOut.put("\n\t\t@y << x_1 [colorL=black colorD=red width=2 name=\"x_1\"]");
        memVisualOut.put("\n\t\t@y << x_2 [colorL=darkGreen colorD=green width=2 name=\"x_2\"]");
        memVisualOut.put("\n\t\t@y << x_3 [colorL=darkBlue colorD=cyan width=2 name=\"x_3\"]");
        if (N >= 5)
        {
            memVisualOut.put("\n\t\t@y << x_4 [colorL=darkYellow colorD=lightBlue width=2 name=\"x_4\"]");
            memVisualOut.put("\n\t\t@y << x_5 [colorL=magenta colorD=magenta width=2 name=\"x_5\"]");
        }
        
        memVisualOut.put("\n\tend");
        
        memVisualOut.put("\n\t//plot relative drifts from the shear building model");
        memVisualOut.put("\n\tlinePlot [xLabel=\"Time [s]\" yLabel=\"rel. shifts.\" name=\"Rel. shifts.\" anchor=TR legend=true nCols=1 anchorX=190 anchorY=35]:");
        memVisualOut.put("\n\t\t@x << t");
        memVisualOut.put("\n\t\t@y << x_1 [colorL=black colorD=red width=2 name=\"drift1\"]");
        memVisualOut.put("\n\t\t@y << x_2-x_2 [colorL=darkGreen colorD=green width=2 name=\"drift2-1\"]");
        memVisualOut.put("\n\t\t@y << x_3-x_2 [colorL=darkBlue colorD=cyan width=2 name=\"drift3-2\"]");
        if (N >= 5)
        {
            memVisualOut.put("\n\t\t@y << x_4-x_3 [colorL=darkYellow colorD=lightBlue width=2 name=\"drift4-3\"]");
            memVisualOut.put("\n\t\t@y << x_5-x_4 [colorL=magenta colorD=magenta width=2 name=\"drift5-4\"]");
        }
        
        memVisualOut.put("\n\tend");
        
        memVisualOut.put("\n\t//plot relative drifts from the shear building model");
        memVisualOut.put("\n\tlinePlot [xLabel=\"Time [s]\" yLabel=\"Accel\" name=\"Accels\" anchor=TR legend=true nCols=1 anchorX=130 anchorY=35]:");
        memVisualOut.put("\n\t\t@x << t");
        memVisualOut.put("\n\t\t@y << a_g [colorL=black colorD=red width=2 name=\"a_g\"]");
        memVisualOut.put("\n\t\t@y << a_1 [colorL=darkGreen colorD=green width=2 name=\"a_1\"]");
        memVisualOut.put("\n\t\t@y << a_3 [colorL=darkBlue colorD=cyan width=2 name=\"a_3\"]");
        if (N >= 5)
            memVisualOut.put("\n\t\t@y << a_5 [colorL=magenta colorD=magenta width=2 name=\"a_5\"]");
        memVisualOut.put("\n\tend");
        memVisualOut.put("\nend //end of the visual model");
        
        td::String strVisualFileName = fo::replaceFileExtension<false>(outFileName, ".vmodl");
        std::ofstream fVisual;
        if (!fo::createTextFile(fVisual, strVisualFileName))
        {
            status = "ERROR! Cannot visoual output file!";
            return false;
        }
        memVisualOut.writeToFile(fVisual);
        fVisual.close();
    }
    
    
    return true;

}
