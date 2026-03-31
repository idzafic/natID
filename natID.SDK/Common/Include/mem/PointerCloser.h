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
	template<class T>
	class PointerCloser
	{
		T* _ptr;

	public:

		PointerCloser(T* ptr)
			: _ptr(ptr)
		{
		}

		PointerCloser(mem::PointerReleaser<T>& ptr)
			: _ptr(ptr.getPtr())
		{
		}

		PointerCloser()
			: _ptr(nullptr)
		{
		}

		inline void operator = (T* ptr)
		{
			if (_ptr)
				_ptr->close();
			_ptr = ptr;
		}

		inline T* operator -> ()
		{
			return _ptr;
		}

		inline bool operator ! ()
		{
			return (_ptr == nullptr);
		}

		inline const T* operator -> () const
		{
			return _ptr;
		}

		inline T& ref()
		{
			return *_ptr;
		}

		inline T* ptr()
		{
			return _ptr;
		}

		inline const T* ptr() const
		{
			return _ptr;
		}

		inline T* getPtr()
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

		inline T* operator () ()
		{
			return _ptr;
		}

		inline const T* operator () () const
		{
			return _ptr;
		}

		~PointerCloser()
		{
			if (_ptr)
			{
				_ptr->close();
			}
		}
	};
};
