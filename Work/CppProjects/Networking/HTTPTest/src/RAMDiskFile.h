//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once

#include <iostream>

//Adjust according to your needs
inline td::String getFileNameOnRamDisk(const char* fileName)
{
#ifdef MU_WINDOWS
    td::String fn("R:/");
#elif defined(MU_LINUX)
    td::String fn("/media/RAMDisk/");
#else
    td::String fn("/Volumes/RAMDisk/");
#endif
    fn += fileName;
    return fn;
}
