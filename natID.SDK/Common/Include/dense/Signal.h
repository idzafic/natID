// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <cassert>

template <typename T>
class Signal
{
protected:
	T* _data = nullptr;
	int _nSamples = 0;
	int _relativeStartingSample = 0;
	///sampling rate
	double _fs = 0;

	inline void allocData(bool initWithZeros = false)
	{
		assert(_nSamples > 0);
		if (_nSamples <= 0)
			return;
		_data = new T[_nSamples];
		if (initWithZeros)
			memset(_data, 0, _nSamples * sizeof(T));
	}

public:
	Signal() {}
	Signal(int nSamples, double fs, int startingSample = 0)
		: _data(nullptr)
		, _nSamples(nSamples)
		, _relativeStartingSample(startingSample)
		, _fs(fs)
	{
		allocData();
	}

	~Signal() 
	{
		if (_data)
			delete[] _data;
	}

	void clean()
	{
		if (_data)
			delete[] _data;
		_nSamples = 0;
	}

	void reserve(int nSamples, bool initWithZeros = false)
	{
		clean();
		_nSamples = nSamples;
		allocData(initWithZeros);
	}

	T& operator() (int i)
	{
		assert(_nSamples > i + _relativeStartingSample);
		return _data[i + _relativeStartingSample];
	}

	const T& operator () (int i) const
	{
		assert(_nSamples > i + _relativeStartingSample);
		return _data[i + _relativeStartingSample];
	}

	void operator = (const Signal& A)
	{
		clean();
		_nSamples = A._nSamples;
		_relativeStartingSample = A._relativeStartingSample;
		_fs = A._fs;
		allocData();
		const T* pData = A._data;		
		for (int i = 0; i < _nSamples; i++)
			_data[i] = pData[i];
	}

	void setSamplingFrequency(double fs)
	{
		_fs = fs;
	}

	double getSamplingFrequency() const
	{
		return _fs;
	}

	int size() const
	{
		return _nSamples - _relativeStartingSample;
	}

	int capacity() const
	{
		return _nSamples;
	}


	void zeros()
	{
		assert(_data != nullptr);		
		memset(_data, 0, _nSamples * sizeof(T));
	}

	void load(std::istream& ar)
	{
		clean();

		ar >> _nSamples >> _relativeStartingSample >> _fs;

		allocData(true);

		for (int i = 0; i < _nSamples; i++)
		{
			ar >> _data[i];
		}
	}

	void save(std::ostream& ar, double minVal = 0, int width = -1, int precision = -1) const
	{
		ar << _nSamples << " " << _relativeStartingSample << " " << _fs << std::endl;
		if (_data == nullptr)
			return;
		int nElems = _nSamples;
		std::streamsize ss = ar.precision();
		for (int i = 0; i < nElems; i++)
		{
			T val = _data[i];
			if (minVal > 0)
			{
				if (std::abs(val) <= minVal)
					val = 0;
			}

			if (width != -1)
				ar << std::fixed << std::setw(width);

			if (precision != -1)
				ar << std::setprecision(precision);
			ar << val << " ";
		}

		if (precision != -1)
			ar << std::setprecision(ss); //set it back

		ar << std::endl;
	}

	bool load(const char *fileName)
	{
		//assert(_data != nullptr);
		std::ifstream f;
		f.open(fileName, std::ios::in);
		if (f.is_open())
		{
			load(f);

			f.close();
			return true;
		}

		return false;
	}

	bool save(const char *fileName)
	{
		//assert(_data != nullptr);
		std::ofstream f(fileName);
		if (f.is_open())
		{
			save(f);

			f.close();
			return true;
		}

		return false;
	}


	void show(const char* name, double minVal = 0, int width = -1, int precision = -1) const
	{
		std::cout << name << " ";
		save(std::cout, minVal, width, precision);
	}

	void show(std::ostream* o, const char* name, double minVal = 0, int width = -1, int precision = -1) const
	{
		if (o == nullptr)
			return;
		*o << name << " ";
		save(*o, minVal, width, precision);
	}
};