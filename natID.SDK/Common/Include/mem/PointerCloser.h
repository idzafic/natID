// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file PointerCloser.h
    @brief RAII wrapper that calls close() on the managed pointer when it goes out of scope. */
#pragma once

namespace mem
{
	/// @brief RAII smart-pointer wrapper that calls T::close() on destruction or assignment.
	/// @tparam T The pointed-to type, which must expose a close() member function.
	template<class T>
	class PointerCloser
	{
		T* _ptr; ///< Raw pointer to the managed object.

	public:

		/// @brief Constructs a PointerCloser that takes ownership of the given raw pointer.
		/// @param ptr Raw pointer to manage.
		PointerCloser(T* ptr)
			: _ptr(ptr)
		{
		}

		/// @brief Constructs a PointerCloser by acquiring the pointer held in a PointerReleaser.
		/// @param ptr Source PointerReleaser whose internal pointer is transferred.
		PointerCloser(mem::PointerReleaser<T>& ptr)
			: _ptr(ptr.getPtr())
		{
		}

		/// @brief Default constructor; initialises the managed pointer to nullptr.
		PointerCloser()
			: _ptr(nullptr)
		{
		}

		/// @brief Assigns a new raw pointer, calling close() on the previously managed pointer.
		/// @param ptr New raw pointer to manage.
		inline void operator = (T* ptr)
		{
			if (_ptr)
				_ptr->close();
			_ptr = ptr;
		}

		/// @brief Arrow operator providing mutable access to the managed object.
		/// @return Mutable raw pointer to the managed object.
		inline T* operator -> ()
		{
			return _ptr;
		}

		/// @brief Logical-NOT operator testing whether the managed pointer is null.
		/// @return true if the managed pointer is nullptr, false otherwise.
		inline bool operator ! ()
		{
			return (_ptr == nullptr);
		}

		/// @brief Arrow operator providing const access to the managed object.
		/// @return Const raw pointer to the managed object.
		inline const T* operator -> () const
		{
			return _ptr;
		}

		/// @brief Returns a mutable reference to the managed object.
		/// @return Mutable reference to the managed object.
		inline T& ref()
		{
			return *_ptr;
		}

		/// @brief Returns the raw mutable pointer to the managed object.
		/// @return Mutable raw pointer.
		inline T* ptr()
		{
			return _ptr;
		}

		/// @brief Returns the raw const pointer to the managed object.
		/// @return Const raw pointer.
		inline const T* ptr() const
		{
			return _ptr;
		}

		/// @brief Returns the raw mutable pointer to the managed object.
		/// @return Mutable raw pointer.
		inline T* getPtr()
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

		/// @brief Call operator returning the managed mutable raw pointer.
		/// @return Mutable raw pointer to the managed object.
		inline T* operator () ()
		{
			return _ptr;
		}

		/// @brief Call operator returning the managed const raw pointer.
		/// @return Const raw pointer to the managed object.
		inline const T* operator () () const
		{
			return _ptr;
		}

		/// @brief Destructor; calls close() on the managed pointer if it is not null.
		~PointerCloser()
		{
			if (_ptr)
			{
				_ptr->close();
			}
		}
	};
};
