#pragma once
#include <compiler/Definitions.h>
#include <fo/FileOperations.h>

//#define USE_DROPBOX 1

inline fo::fs::path getTestMatricesPath(const fo::fs::path& natIDSDKPath)
{
    return natIDSDKPath;
    
//#ifdef MU_MACOS
//    
//#ifdef USE_DROPBOX
//    return "/Volumes/macExtra/Dropbox/Work/TestData/TestMatrices";
//#else
//    return "/Volumes/VMs/Dropbox/Work/TestData/TestMatrices";
//#endif
//    
//#else
//    return "/Users/idzafic/Dropbox/Work/TestData/TestMatrices";
//#endif
}

inline fo::fs::path getRealCholleskyPath(const fo::fs::path& natIDSDKPath)
{
    return natIDSDKPath / "TSE/NE/Real";
}

inline fo::fs::path getRealLDLTPath(const fo::fs::path& natIDSDKPath)
{
    return natIDSDKPath / "TSE/EC/Real";
}

inline fo::fs::path getCmplxCholleskyPath(const fo::fs::path& natIDSDKPath)
{
    return natIDSDKPath / "TSE/NE/Cmplx";
}

inline fo::fs::path getCmplxLDLTPath(const fo::fs::path& natIDSDKPath)
{
    return natIDSDKPath / "TSE/EC/Cmplx";
}


inline fo::fs::path getResultPath(const fo::fs::path& natIDSDKPath)
{
#ifdef MU_WINDOWS
    fo::fs::path resPath = "R:/Results";
#else
#ifdef MU_MACOS
    fo::fs::path resPath = "/Volumes/RAMDisk/Results";
#else
    fo::fs::path resPath = "/media/RAMDisk/Results";
#endif
#endif
    auto rootPath = resPath.parent_path();
    if (fo::folderExists(rootPath))
        return resPath;
    fo::fs::path homePath = mu::getHomePath();
    resPath = homePath / "ba.natID/TestSparse";
    return resPath;
    
}
