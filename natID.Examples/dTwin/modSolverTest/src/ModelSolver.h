#pragma once
#include <sc/IModel.h>
#include <iostream>
#include <fstream>
#include <mu/mu.h>
#include <syst/Environment.h>
#include <tuple>

inline fo::fs::path getOutPath(const fo::fs::path& pathIn, const td::String& outFolder)
{
    auto outLen = outFolder.length();
    int pos = 1;
    if (outLen > 1)
    {
        char ch = outFolder.at(1);
#ifdef MU_WINDOWS
        if (ch== '\\')
            ++pos;
#else
        if (ch== '/')
            ++pos;
#endif
    }
    const char* pStr = outFolder.c_str();
    fo::fs::path outPath = pathIn / (pStr + pos);
    return outPath;
}

inline std::tuple<td::String, td::String> getInOutFileNames(const td::String& inFileName, const td::String& outFolder)
{
    td::String outFileName;
    auto outLen = outFolder.length();
    
    if (outLen == 0)
    {
        outFileName = fo::replaceFileExtension<false>(inFileName, ".txt");
        if (outFileName.length() == 0)
        {
            std::cout << "Input file name doesn't exist! Fn=" << inFileName << td::endl;
            return {td::String(), td::String()};
        }
    }
    else
    {
        fo::fs::path outPath;
        if (outFolder.at(0) == '~')
        {
            fo::fs::path homePath;
            mu::getHomePath(homePath);

            outPath = getOutPath(homePath, outFolder);
        }
        else if (outFolder.at(0) == ':')
        {
            fo::fs::path filePath = inFileName.c_str();

            fo::fs::path folderPath = filePath.parent_path();
            outPath = getOutPath(folderPath, outFolder);
        }
        else if (outFolder.at(0) == '.')
        {
            fo::fs::path currentDir = fo::fs::current_path();
            outPath = getOutPath(currentDir, outFolder);
        }
        else
            outPath = outFolder.c_str();
        
        fo::fs::path filePath = inFileName.c_str();
        fo::fs::path fileName = filePath.filename();
        auto fn = fo::replaceFileExtension<false>(fileName, ".txt");
        fo::fs::path outPathWithFileName = outPath / fn;
        fo::createFolders(outPath);
        outFileName = outPathWithFileName.string();
    }
    td::String inFileNameWoEnvVars = syst::Environment::replaceVariablesInPath(inFileName);
    
    return {inFileNameWoEnvVars, outFileName};
}
