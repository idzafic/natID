// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <td/Types.h>
#include <mu/Utils.h>
#include <td/String.h>
#include <fo/FileOperations.h>

namespace mu
{	
	template <class TSTR>
	class TxtOutFile
	{
	protected:
		TSTR f;		
		Utils u;
	public:		

		TxtOutFile()
		{			
		}		

		TxtOutFile(const td::String& fileName)			
		{
			fo::openFile(f, fileName);
		}

		TSTR& getStream()
		{
			return f;
		}

		~TxtOutFile()
		{
			if (f.is_open())
				f.close();
		}

		void flush()
		{
			f.flush();
		}

		void write(const char* pStr, int nLen)
		{
			f.write(pStr, nLen);
		}

		TxtOutFile<TSTR>& operator << (char val)
		{
			f << val;
			return *this;
		}

		TxtOutFile<TSTR>& operator << (td::INT4 val)
		{
			f << val;
			return *this;
		}

		TxtOutFile<TSTR>& operator << (td::LINT8 val)
		{
			f << val;
			return *this;
		}

		TxtOutFile<TSTR>& operator << (td::UINT4 val)
		{
			f << val;
			return *this;
		}

		TxtOutFile<TSTR>& operator << (td::LUINT8 val)
		{
			f << val;
			return *this;
		}

		TxtOutFile<TSTR>& operator << (td::WORD val)
		{
			f << val;
			return *this;
		}

		TxtOutFile<TSTR>& operator << (td::INT2 val)
		{
			f << val;
			return *this;
		}

		TxtOutFile<TSTR>& operator << (float val)
		{
			f << val;
			return *this;
		}

		TxtOutFile<TSTR>& operator << (double val)
		{
			f << val;
			return *this;
		}
		template <size_t size>
		inline TxtOutFile<TSTR>& operator << (const char(&ctStrIn)[size])
		{
			f << ctStrIn;
			return *this;
		}

		TxtOutFile<TSTR>& operator << (const char* val)
		{
			f << val;
			return *this;
		}


		TxtOutFile<TSTR>& operator << (td::String& val)
		{
			f << val.c_str();
			return *this;
		}		

		

		template <typename T>
		TxtOutFile<TSTR>& operator << (const T& val)
		{
			f << u.c_str(val);
			return *this;
		}
        
        bool is_open() const
        {
            return f.is_open();
        }
        
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

	
