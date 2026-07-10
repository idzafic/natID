// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Signal.h @brief Generic 1-D signal class template with sampling-rate support. */
#pragma once
#include <cassert>

/// @brief Template class representing a discrete-time signal with configurable sample type.
template <typename T>
class Signal
{
protected:
	T* _data = nullptr;                       ///< Pointer to the sample data array
	int _nSamples = 0;                        ///< Total number of allocated samples
	int _relativeStartingSample = 0;          ///< Offset applied to all index accesses
	///sampling rate
	double _fs = 0;                           ///< Sampling frequency in Hz

	/// @brief Allocates the internal sample buffer.
	/// @param initWithZeros If true, initializes all samples to zero.
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
	/// @brief Default constructor — creates an empty signal.
	Signal() {}
	/// @brief Constructs a signal with the given number of samples and sampling frequency.
	/// @param nSamples Number of samples to allocate.
	/// @param fs Sampling frequency in Hz.
	/// @param startingSample Relative starting sample offset (default 0).
	Signal(int nSamples, double fs, int startingSample = 0)
		: _data(nullptr)
		, _nSamples(nSamples)
		, _relativeStartingSample(startingSample)
		, _fs(fs)
	{
		allocData();
	}

	/// @brief Destructor — releases the sample buffer.
	~Signal()
	{
		if (_data)
			delete[] _data;
	}

	/// @brief Releases the sample buffer and resets the sample count.
	void clean()
	{
		if (_data)
			delete[] _data;
		_nSamples = 0;
	}

	/// @brief Reallocates storage for a new number of samples.
	/// @param nSamples New number of samples.
	/// @param initWithZeros If true, initializes all samples to zero.
	void reserve(int nSamples, bool initWithZeros = false)
	{
		clean();
		_nSamples = nSamples;
		allocData(initWithZeros);
	}

	/// @brief Returns a mutable reference to the sample at index i (offset-adjusted).
	/// @param i Sample index relative to the starting sample offset.
	/// @return Mutable reference to the sample.
	T& operator() (int i)
	{
		assert(_nSamples > i + _relativeStartingSample);
		return _data[i + _relativeStartingSample];
	}

	/// @brief Returns a const reference to the sample at index i (offset-adjusted).
	/// @param i Sample index relative to the starting sample offset.
	/// @return Const reference to the sample.
	const T& operator () (int i) const
	{
		assert(_nSamples > i + _relativeStartingSample);
		return _data[i + _relativeStartingSample];
	}

	/// @brief Copy assignment operator — deep copies samples and metadata.
	/// @param A Source signal to copy from.
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

	/// @brief Sets the sampling frequency.
	/// @param fs New sampling frequency in Hz.
	void setSamplingFrequency(double fs)
	{
		_fs = fs;
	}

	/// @brief Returns the sampling frequency.
	/// @return Sampling frequency in Hz.
	double getSamplingFrequency() const
	{
		return _fs;
	}

	/// @brief Returns the number of accessible samples (capacity minus start offset).
	/// @return Number of usable samples.
	int size() const
	{
		return _nSamples - _relativeStartingSample;
	}

	/// @brief Returns the total allocated sample capacity.
	/// @return Total number of allocated samples.
	int capacity() const
	{
		return _nSamples;
	}


	/// @brief Sets all samples to zero.
	void zeros()
	{
		assert(_data != nullptr);
		memset(_data, 0, _nSamples * sizeof(T));
	}

	/// @brief Loads signal data from a stream.
	/// @param ar Input stream containing sample count, start offset, frequency, and sample values.
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

	/// @brief Saves signal data to a stream.
	/// @param ar Output stream.
	/// @param minVal Values with absolute value below this threshold are written as zero.
	/// @param width Field width for formatted output (-1 for default).
	/// @param precision Number of decimal digits (-1 for default).
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

	/// @brief Loads signal data from a file.
	/// @param fileName Path to the input file.
	/// @return true on success, false if the file could not be opened.
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

	/// @brief Saves signal data to a file.
	/// @param fileName Path to the output file.
	/// @return true on success, false if the file could not be opened.
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


	/// @brief Prints the signal to stdout with a label.
	/// @param name Label string printed before the signal data.
	/// @param minVal Values below this threshold are displayed as zero.
	/// @param width Field width for formatted output (-1 for default).
	/// @param precision Number of decimal digits (-1 for default).
	void show(const char* name, double minVal = 0, int width = -1, int precision = -1) const
	{
		std::cout << name << " ";
		save(std::cout, minVal, width, precision);
	}

	/// @brief Prints the signal to a given output stream with a label.
	/// @param o Pointer to the output stream; does nothing if nullptr.
	/// @param name Label string printed before the signal data.
	/// @param minVal Values below this threshold are displayed as zero.
	/// @param width Field width for formatted output (-1 for default).
	/// @param precision Number of decimal digits (-1 for default).
	void show(std::ostream* o, const char* name, double minVal = 0, int width = -1, int precision = -1) const
	{
		if (o == nullptr)
			return;
		*o << name << " ";
		save(*o, minVal, width, precision);
	}
};
