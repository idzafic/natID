// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file PointerDeleter.h
    @brief RAII wrapper that calls delete on the managed pointer when it goes out of scope. */
#pragma once

namespace mem
{
	/// @brief RAII smart-pointer wrapper that calls delete on the managed pointer upon destruction or assignment.
	/// @tparam T The pointed-to type.
	template <typename T>
	class PointerDeleter
	{
	protected:
		T* _ptr; ///< Raw pointer to the managed object.
	public:
		/// @brief Constructs a PointerDeleter that takes ownership of the given raw pointer.
		/// @param ptr Raw pointer to manage.
		PointerDeleter(T* ptr)
		: _ptr(ptr)
		{
		}

		/// @brief Default constructor; initialises the managed pointer to null.
		PointerDeleter()
			: _ptr(0)
		{
		}

		/// @brief Arrow operator providing mutable access to the managed object.
		/// @return Mutable raw pointer to the managed object.
		T* operator -> ()
		{
			return _ptr;
		}

		/// @brief Arrow operator providing const access to the managed object.
		/// @return Const raw pointer to the managed object.
		const T* operator -> () const
		{
			return _ptr;
		}

		/// @brief Returns the raw mutable pointer to the managed object.
		/// @return Mutable raw pointer.
		T* ptr()
		{
			return _ptr;
		}

		/// @brief Returns the raw const pointer to the managed object.
		/// @return Const raw pointer.
		const T* ptr() const
		{
			return _ptr;
		}

		/// @brief Returns the raw mutable pointer to the managed object.
		/// @return Mutable raw pointer.
		T* getPtr()
		{
			return _ptr;
		}

		/// @brief Returns the raw const pointer to the managed object.
		/// @return Const raw pointer.
		const T* getPtr() const
		{
			return _ptr;
		}

		/// @brief Call operator returning the managed mutable raw pointer.
		/// @return Mutable raw pointer to the managed object.
		T* operator () ()
		{
			return _ptr;
		}

		/// @brief Address-of operator returning the managed mutable raw pointer.
		/// @return Mutable raw pointer to the managed object.
		inline T* operator & ()
		{
			return _ptr;
		}

		/// @brief Address-of operator returning the managed const raw pointer.
		/// @return Const raw pointer to the managed object.
		const inline T* operator & () const
		{
			return _ptr;
		}

		/// @brief Call operator returning the managed const raw pointer.
		/// @return Const raw pointer to the managed object.
		const T* operator () () const
		{
			return _ptr;
		}

		/// @brief Assigns a new raw pointer, deleting the previously managed pointer.
		/// @param ptr New raw pointer to manage.
		void operator = (T* ptr)
		{
			if (_ptr)
				delete _ptr;;
			_ptr = ptr;
		}

		/// @brief Destructor; calls delete on the managed pointer if it is not null.
		~PointerDeleter()
		{
			if (_ptr)
			{
				delete _ptr;
			}
		}
	};

	/// @brief Deletes every element stored in a container by calling delete on each.
	/// @tparam T Container type whose value type is a raw pointer.
	/// @param c Container whose elements are to be deleted.
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

	/// @brief Deletes every array element stored in a container by calling delete[] on each.
	/// @tparam T Container type whose value type is a raw pointer to an array.
	/// @param c Container whose array elements are to be deleted.
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
