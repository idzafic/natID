// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once

//#include <boost/config/warning_disable.hpp>
#ifdef USE_BOOST_FILE_SYSTEM
#include <boost/filesystem.hpp>
//#include <boost/filesystem/fstream.hpp>
#else
#include <filesystem> 
#endif

namespace fo
{

#ifdef USE_BOOST_FILE_SYSTEM
  namespace fs = boost::filesystem;
#else
    //namespace fs = std::experimental::filesystem::v1;
    namespace fs = std::filesystem;
#endif


} //namespace fo


