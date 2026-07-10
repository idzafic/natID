// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file TxtOutFile.h
    @brief Template wrapper around a stream type that provides convenient text-output operators. */
#pragma once
#include <td/Types.h>
#include <mu/Utils.h>
#include <td/String.h>
#include <fo/FileOperations.h>

namespace mu
{
	/// @brief Template text-output file wrapper providing stream insertion operators for common types.
	/// @tparam TSTR The underlying stream type (e.g. std::ofstream).
	template <class TSTR>
	class TxtOutFile
	{
	protected:
		TSTR f;   ///< The underlying output stream.
		Utils u;  ///< Utility instance used to convert values to C-strings.
	public:

		/// @brief Default constructor; opens no file.
		TxtOutFile()
		{
		}

		/// @brief Opens the output file at the specified path.
		/// @param fileName Path to the file to open for writing.
		TxtOutFile(const td::String& fileName)
		{
			fo::openFile(f, fileName);
		}

		/// @brief Returns a reference to the underlying stream.
		/// @return Reference to the stream object.
		TSTR& getStream()
		{
			return f;
		}

		/// @brief Destructor; closes the file if it is still open.
		~TxtOutFile()
		{
			if (f.is_open())
				f.close();
		}

		/// @brief Flushes the underlying stream buffer to disk.
		void flush()
		{
			f.flush();
		}

		/// @brief Writes a raw byte buffer of given length to the stream.
		/// @param pStr Pointer to the character data to write.
		/// @param nLen Number of bytes to write.
		void write(const char* pStr, int nLen)
		{
			f.write(pStr, nLen);
		}

		/// @brief Inserts a single character into the stream.
		/// @param val The character to write.
		/// @return Reference to this TxtOutFile for chaining.
		TxtOutFile<TSTR>& operator << (char val)
		{
			f << val;
			return *this;
		}

		/// @brief Inserts a 32-bit signed integer into the stream.
		/// @param val The integer value to write.
		/// @return Reference to this TxtOutFile for chaining.
		TxtOutFile<TSTR>& operator << (td::INT4 val)
		{
			f << val;
			return *this;
		}

		/// @brief Inserts a 64-bit signed integer into the stream.
		/// @param val The integer value to write.
		/// @return Reference to this TxtOutFile for chaining.
		TxtOutFile<TSTR>& operator << (td::LINT8 val)
		{
			f << val;
			return *this;
		}

		/// @brief Inserts a 32-bit unsigned integer into the stream.
		/// @param val The unsigned integer value to write.
		/// @return Reference to this TxtOutFile for chaining.
		TxtOutFile<TSTR>& operator << (td::UINT4 val)
		{
			f << val;
			return *this;
		}

		/// @brief Inserts a 64-bit unsigned integer into the stream.
		/// @param val The unsigned integer value to write.
		/// @return Reference to this TxtOutFile for chaining.
		TxtOutFile<TSTR>& operator << (td::LUINT8 val)
		{
			f << val;
			return *this;
		}

		/// @brief Inserts a 16-bit unsigned integer (WORD) into the stream.
		/// @param val The WORD value to write.
		/// @return Reference to this TxtOutFile for chaining.
		TxtOutFile<TSTR>& operator << (td::WORD val)
		{
			f << val;
			return *this;
		}

		/// @brief Inserts a 16-bit signed integer into the stream.
		/// @param val The INT2 value to write.
		/// @return Reference to this TxtOutFile for chaining.
		TxtOutFile<TSTR>& operator << (td::INT2 val)
		{
			f << val;
			return *this;
		}

		/// @brief Inserts a single-precision floating-point value into the stream.
		/// @param val The float value to write.
		/// @return Reference to this TxtOutFile for chaining.
		TxtOutFile<TSTR>& operator << (float val)
		{
			f << val;
			return *this;
		}

		/// @brief Inserts a double-precision floating-point value into the stream.
		/// @param val The double value to write.
		/// @return Reference to this TxtOutFile for chaining.
		TxtOutFile<TSTR>& operator << (double val)
		{
			f << val;
			return *this;
		}

		/// @brief Inserts a compile-time-sized C-string literal into the stream.
		/// @tparam size Compile-time size of the string array including the null terminator.
		/// @param ctStrIn Reference to the string literal array.
		/// @return Reference to this TxtOutFile for chaining.
		template <size_t size>
		inline TxtOutFile<TSTR>& operator << (const char(&ctStrIn)[size])
		{
			f << ctStrIn;
			return *this;
		}

		/// @brief Inserts a null-terminated C-string pointer into the stream.
		/// @param val Pointer to the null-terminated string to write.
		/// @return Reference to this TxtOutFile for chaining.
		TxtOutFile<TSTR>& operator << (const char* val)
		{
			f << val;
			return *this;
		}


		/// @brief Inserts a td::String value into the stream.
		/// @param val Reference to the String object to write.
		/// @return Reference to this TxtOutFile for chaining.
		TxtOutFile<TSTR>& operator << (td::String& val)
		{
			f << val.c_str();
			return *this;
		}



		/// @brief Inserts any value convertible to a C-string via Utils::c_str into the stream.
		/// @tparam T The type of the value; must be supported by Utils::c_str.
		/// @param val The value to write.
		/// @return Reference to this TxtOutFile for chaining.
		template <typename T>
		TxtOutFile<TSTR>& operator << (const T& val)
		{
			f << u.c_str(val);
			return *this;
		}

        /// @brief Returns true if the underlying file stream is open.
        /// @return true if the file is open; false otherwise.
        bool is_open() const
        {
            return f.is_open();
        }

        /// @brief Closes the underlying file stream.
        void close()
        {
            f.close();
        }

		//template <typename TDEC, int NLEN>
		//TxtOutFile<TSTR>& operator << (const td::Decimal<TDEC, NLEN>& val)
		//{
		//	f << u.toString(val);
		//	return *this;
		//}
	};
}


