// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file AppVersion.h
    @brief Defines the AppVersion struct and utility functions for parsing and comparing application version numbers. */
#pragma once
#include <fo/FileOperations.h>
#include <td/Types.h>
#include <mu/IAppSettings.h>
#include <fstream>
#include <string>
#include <cnt/PushBackVector.h>
namespace mu
{
	/// @brief Represents a four-part application version number (major.majorSub.minor.minorSub).
	struct AppVersion
	{
	private:
		td::INT1 _pos = 0; ///< Current streaming position used by operator<<.
	public:
		td::INT2 _major; ///< Major version component.
		td::INT2 _majorSub; ///< Major sub-version component.
		td::INT2 _minor; ///< Minor version component.
		td::INT2 _minorSub; ///< Minor sub-version component.

		/// @brief Encodes the four version parts into a single 64-bit integer.
		/// @return A LINT8 value encoding the version as (major*1000000 + majorSub*10000 + minor*100 + minorSub).
		const td::LINT8 toLINT8() const
		{
			return _major * 1000000 + _majorSub * 10000 + _minor * 100 + _minorSub;
		}

		/// @brief Default constructor; initializes all version components to zero.
		AppVersion()
			: _major(0)
			, _majorSub(0)
			, _minor(0)
			, _minorSub(0)
		{}

		/// @brief Constructs an AppVersion from explicit version component values.
		/// @param major Major version number.
		/// @param majorSub Major sub-version number.
		/// @param minor Minor version number.
		/// @param minorSub Minor sub-version number.
		AppVersion(td::INT2 major, td::INT2 majorSub, td::INT2 minor, td::INT2 minorSub)
			: _major(major)
			, _majorSub(majorSub)
			, _minor(minor)
			, _minorSub(minorSub)
		{}

		/// @brief Constructs an AppVersion by decoding a packed 64-bit version integer.
		/// @param version Encoded version produced by toLINT8().
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

		/// @brief Equality comparison operator.
		/// @param oth The other AppVersion to compare against.
		/// @return true if all four version components are equal.
		bool operator = (const AppVersion& oth)
		{
			return _major == oth._major && _majorSub == oth._majorSub && _minor == oth._minor && _minorSub == oth._minorSub;
		}

		/// @brief Inequality comparison operator.
		/// @param oth The other AppVersion to compare against.
		/// @return true if any version component differs.
		bool operator != (const AppVersion& oth)
		{
			return !operator=(oth);
		}

		/// @brief Less-than comparison operator.
		/// @param oth The other AppVersion to compare against.
		/// @return true if this version is strictly less than oth.
		bool operator < (const AppVersion& oth)
		{
			return _major < oth._major ? true : _major > oth._major ? false : _majorSub < oth._majorSub ? true : _majorSub > oth._majorSub ? false
				: _minor < oth._minor ? true : _minor > oth._minor ? false : _minorSub < oth._minorSub ? true : false;
		}

		/// @brief Greater-than comparison operator.
		/// @param oth The other AppVersion to compare against.
		/// @return true if this version is strictly greater than oth.
		bool operator > (const AppVersion& oth)
		{
			return _major > oth._major ? true : _major < oth._major ? false : _majorSub > oth._majorSub ? true : _majorSub < oth._majorSub ? false
				: _minor > oth._minor ? true : _minor < oth._minor ? false : _minorSub > oth._minorSub ? true : false;
		}

		/// @brief Greater-than-or-equal comparison operator.
		/// @param oth The other AppVersion to compare against.
		/// @return true if this version is greater than or equal to oth.
		bool operator >= (const AppVersion& oth)
		{
			return !operator<(oth);
		}

		/// @brief Less-than-or-equal comparison operator.
		/// @param oth The other AppVersion to compare against.
		/// @return true if this version is less than or equal to oth.
		bool operator <= (const AppVersion& oth)
		{
			return !operator>(oth);
		}

		/// @brief Stream insertion operator for loading version parts sequentially.
		/// @param verPart A single version component (cycles through major, majorSub, minor, minorSub).
		/// @return Reference to this AppVersion for chaining.
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

	/// @brief Splits a dot-delimited (or custom delimiter) version string into an AppVersion.
	/// @param version Output AppVersion that receives the parsed components.
	/// @param word The string containing the version number.
	/// @param delimiter The character used as separator between version components.
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

	/// @brief Reads program and support version numbers from a text file in the application resource path.
	/// @param fileName Name of the version file relative to the resource path.
	/// @param programVersion Output AppVersion populated with the program version (first line).
	/// @param supportVersion Output AppVersion populated with the support version (second line).
	/// @return true if the file was opened and both versions were read successfully.
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




	/// @brief Searches a directory for a setup package matching the given application name and extracts its version.
	/// @param folderName Path to the directory containing setup files.
	/// @param appName Name prefix of the application to search for.
	/// @param setupVersion Output AppVersion populated with the version found in the matching file name.
	/// @return true if a matching setup file was found and the version was extracted.
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
