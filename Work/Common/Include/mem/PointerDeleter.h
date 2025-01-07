// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once

namespace mem
{
	template <typename T>
	class PointerDeleter
	{
	protected:
		T* _ptr;
	public:
		PointerDeleter(T* ptr)
		: _ptr(ptr)
		{			
		}

		PointerDeleter()
			: _ptr(0)
		{			
		}
		
		T* operator -> ()
		{
			return _ptr;
		}

		const T* operator -> () const
		{
			return _ptr;
		}

		T* ptr()
		{
			return _ptr;
		}

		const T* ptr() const
		{
			return _ptr;
		}
		
		T* getPtr()
		{
			return _ptr;
		}

		const T* getPtr() const
		{
			return _ptr;
		}

		T* operator () ()
		{
			return _ptr;
		}


		inline T* operator & ()
		{
			return _ptr;
		}

		const inline T* operator & () const
		{
			return _ptr;
		}

		const T* operator () () const
		{
			return _ptr;
		}

		void operator = (T* ptr)
		{
			if (_ptr)
				delete _ptr;;
			_ptr = ptr;
		}

		~PointerDeleter()		
		{
			if (_ptr)
			{
				delete _ptr;				
			}
		}
	};

	template <class T>
	inline void deleteContainerContent(T& c)
	{
		typename T::iterator it(c.begin());
		typename T::const_iterator itEnd(c.end());
		while (it != itEnd)
		{
			delete *it;
			++it;
		}
	}

    template <class T>
    inline void deleteContainerContentVects(T& c)
    {
        typename T::iterator it(c.begin());
        typename T::const_iterator itEnd(c.end());
        while (it != itEnd)
        {
            delete [] *it;
            ++it;
        }
    }
};
