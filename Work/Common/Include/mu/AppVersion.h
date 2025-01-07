// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <fo/FileOperations.h>
#include <td/Types.h>
#include <mu/IAppSettings.h>
#include <fstream>
#include <string>
#include <cnt/PushBackVector.h>
namespace mu
{
	struct AppVersion
	{
	private:
		td::INT1 _pos = 0;
	public:
		td::INT2 _major;
		td::INT2 _majorSub;
		td::INT2 _minor;
		td::INT2 _minorSub;

		const td::LINT8 toLINT8() const
		{
			return _major * 1000000 + _majorSub * 10000 + _minor * 100 + _minorSub;
		}

		AppVersion()
			: _major(0)
			, _majorSub(0)
			, _minor(0)
			, _minorSub(0)
		{}

		AppVersion(td::INT2 major, td::INT2 majorSub, td::INT2 minor, td::INT2 minorSub)
			: _major(major)
			, _majorSub(majorSub)
			, _minor(minor)
			, _minorSub(minorSub)
		{}

		AppVersion(td::LINT8 version)
		{
			_minorSub = version % 100;
			version /= 100;
			_minor = version % 100;
			version /= 100;
			_majorSub = version % 100;
			version /= 100;
			_major = version % 100;
		}

		bool operator = (const AppVersion& oth)
		{
			return _major == oth._major && _majorSub == oth._majorSub && _minor == oth._minor && _minorSub == oth._minorSub;
		}

		bool operator != (const AppVersion& oth)
		{
			return !operator=(oth);
		}

		bool operator < (const AppVersion& oth)
		{
			return _major < oth._major ? true : _major > oth._major ? false : _majorSub < oth._majorSub ? true : _majorSub > oth._majorSub ? false
				: _minor < oth._minor ? true : _minor > oth._minor ? false : _minorSub < oth._minorSub ? true : false;
		}

		bool operator > (const AppVersion& oth)
		{
			return _major > oth._major ? true : _major < oth._major ? false : _majorSub > oth._majorSub ? true : _majorSub < oth._majorSub ? false
				: _minor > oth._minor ? true : _minor < oth._minor ? false : _minorSub > oth._minorSub ? true : false;
		}

		bool operator >= (const AppVersion& oth)
		{
			return !operator<(oth);
		}

		bool operator <= (const AppVersion& oth)
		{
			return !operator>(oth);
		}

		AppVersion& operator << (td::INT2 verPart)
		{
			if (_pos == 0)
				_major = verPart;
			else if (_pos == 1)
				_majorSub = verPart;
			else if (_pos == 2)
				_minor = verPart;
			else if (_pos == 3)
				_minorSub = verPart;

			++_pos %= 4;
			return *this;
		}
	};

	void splitStringIntoVersion(AppVersion& version, td::String word, char delimiter)
	{
		int pos = 0;
		while ((pos = word.find(delimiter)) != -1)
		{
			td::String ver = word.subStr(0, pos);
			version << ver.toINT2();
			word = word.subStr(pos + 1, word.unitLength());
		}
		version << word.toINT2();
	}

	inline bool getAppVersion(td::String fileName, AppVersion& programVersion, AppVersion& supportVersion)
	{
		const fo::fs::path appFolder = mu::getAppSettings()->getResPath().c_str();
		fo::fs::path verFile(appFolder / fileName.c_str());

		std::ifstream fStr(verFile);
		if (!fStr.is_open())
			return false;

		std::string line;
		std::getline(fStr, line);
		splitStringIntoVersion(programVersion, line, '.');

		std::getline(fStr, line);
		splitStringIntoVersion(supportVersion, line, '.');
		return true;
	}




	inline bool getSetupVersion(td::String folderName, td::String appName, AppVersion& setupVersion)
	{
		fo::fs::path setupFile(folderName.c_str());
		fo::fs::directory_iterator dir(setupFile);
		for (auto file : dir)
		{
			td::String fileName = file.path().filename().c_str();
			fileName = fileName.subStr(0, fileName.find('_') - 1);
			if (appName == fileName)
			{
				fileName = file.path().filename().c_str();
				splitStringIntoVersion(setupVersion, fileName.subStr(fileName.find('_') + 1, fileName.findFromRight('.') - 1).c_str(), '.');
				return true;
			}
		}	
		return false;
	}
}
