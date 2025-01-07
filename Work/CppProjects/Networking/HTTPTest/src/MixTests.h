//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once

#include <no/netOp.h>
#include <no/NetworkInterfaces.h>
#include <iostream>

inline void testSecu()
{
    const size_t stage2Len = 96;
    unsigned char tmp[stage2Len+1];
    
    for (int i=0; i<96; ++i)
        tmp[i] = ' ' + i;
    tmp[94] = 'X';
    tmp[95] = 'X';
    tmp[stage2Len] = 0;
    
    unsigned char keys[48+1]; //96 should be enough
    
    for (int i=0; i<32; ++i)
        keys[i] = 'A'+i;
    for (int i=0; i<16; ++i)
        keys[i] = 'a' +i;
    
    keys[48] = 0;
    
    const unsigned char* key = &keys[0];
    const unsigned char* iv = &keys[32];
    
    const size_t encodedLen = 112;
    
    unsigned char tmp3[encodedLen+1];
    tmp3[encodedLen] = 0;
    
    auto nLen = no::secu::encrypt(&tmp[0], 96, key, iv, tmp3, 113);
    std::cout << "Encrypted length = " << nLen << std::endl;
    if (nLen <= 0)
    {
        std::cout << "ERROR Ecryption!" << std::endl;
        return;
    }
    fo::OutFile f1;
    td::String strFn = getFileNameOnRamDisk("EncryptedText.txt");
    f1.open(strFn.c_str());
    f1.write((const char*)  & tmp3[0], nLen);
    f1.close();

    //Decode
    {
        unsigned char tmpD[stage2Len+1];
        td::UINT4 startingPos = 0;
        td::UINT4 stillToProcess = 0;
        td::UINT4 msgLen = 0;
        td::BYTE customClient = 0;        

        tmpD[stage2Len] = 0;
        
        int decMsglen = no::secu::decrypt(&tmp3[0], encodedLen, key, iv, &tmpD[0]);
        std::cout << "Decrypted length = " << decMsglen << std::endl;
        if (decMsglen <= 0)
        {
            std::cout << "ERROR Decrypt!" << std::endl;
            return;
        }
        
        if (decMsglen != 96)
        {
            std::cout << "ERROR Decrypt96!" << std::endl;
            return;
        }
        
        if (memcmp(tmp, tmpD, 96) != 0)
        {
            std::cout << "ERROR Incorrect encryption-decryption cycle output!" << std::endl;
            return;
        }
        
        std::cout << "Secu ALL OK!" << std::endl;
    }
}

inline void testNIFs()
{
    no::NetworkInterfaces nifs;
    no::getNetworkInterfaces(nifs, true);
    nifs.show(std::cout);
}

