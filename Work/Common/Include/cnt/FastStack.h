// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
namespace cnt
{
	template <class TStackVal> 
	class FastStack
	{
		TStackVal* _entries;
		TStackVal* _top;
	public:

		FastStack(int size)		
		{
			_entries = new TStackVal[size+1];
			_top = _entries;
		};
		void push()
		{
			_top++;
		};

		void push(TStackVal& val)
		{
			_top++;
			*_top = val;
		};

		void pop()
		{
			_top--;
		}

		bool isEmpty()
		{
			return (_top <= _entries) ? true : false;
		};

		TStackVal* top()
		{
			return _top;
		}
	public:

		~FastStack(void)
		{
			delete [] _entries;
		};
	};
}
