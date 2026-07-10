// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Size.h
    @brief Defines a generic two-dimensional size type with width and height. */
#pragma once
#include <td/Types.h>

namespace td
{
	/// @brief Represents a two-dimensional size with width and height components.
	/// @tparam T The numeric type used for the width and height values.
	template <typename T>
	class Size
	{
	public:
		T width;  ///< Horizontal extent.
		T height; ///< Vertical extent.

		/// @brief Default constructor; initialises width and height to zero.
		Size()
			: width(0)
			, height(0)
		{};

		/// @brief Copy constructor.
		/// @param p The Size to copy from.
		Size(const Size<T>& p)
			: width(p.width)
			, height(p.height)
		{};

		/// @brief Constructs a Size with explicit width and height values.
		/// @param w The width value.
		/// @param h The height value.
		Size(T w, T h)
			: width(w)
			, height(h)
		{};

        /// @brief Returns the area (width * height).
        /// @return The product of width and height.
        T area() const
        {
            return width * height;
        }

        /// @brief Sets both width and height to zero.
        void toZero()
        {
            width = 0;
            height = 0;
        }

        /// @brief Scales both dimensions by a uniform double factor.
        /// @param scale The scale factor to apply.
        void scale(double scale)
        {
            width *= scale;
            height *= scale;
        }

		/// @brief Assigns width and height from another Size of the same type.
		/// @param p The source Size.
		void operator = (const Size& p)
		{
			width = p.width;
			height = p.height;
		}

        /// @brief Assigns width and height from a Size of a different element type.
        /// @tparam T2 The element type of the source Size.
        /// @param p The source Size.
        template <typename T2>
        void operator = (const Size<T2>& p)
        {
            width = T(p.width);
            height = T(p.height);
        }

        /// @brief Adds another Size component-wise.
        /// @param p The Size to add.
        void operator += (const Size& p)
        {
            width += p.width;
            height += p.height;
        }

        /// @brief Scales both dimensions by a double factor in-place.
        /// @param scale The scale factor to apply.
        /// @return Reference to this Size after scaling.
        Size& operator *= (double scale)
        {
            width *= scale;
            height *= scale;
            return *this;
        }

        /// @brief Tests equality with another Size.
        /// @param s The Size to compare against.
        /// @return true if both width and height are equal.
        bool operator == (const Size& s) const
        {
            return ((width == s.width) && (height == s.height));
        }

        /// @brief Tests inequality with another Size.
        /// @param s The Size to compare against.
        /// @return true if width or height differ.
        bool operator != (const Size& s) const
        {
            return ((width != s.width) || (height != s.height));
        }

		/// @brief Serializes width and height using a Boost-style archive.
		/// @tparam Archive The archive type.
		/// @param ar The archive instance.
		/// @param file_version Unused version parameter.
		template<class Archive>
		void serialize(Archive& ar, [[maybe_unused]] const unsigned int file_version) const
		{
			ar & width & height;
		}

		/// @brief Formats the size as a quoted XML attribute string into a buffer.
		/// @param buff Destination character buffer.
		/// @param buffLen Size of the destination buffer in bytes.
		void c_str(char* buff, int buffLen) const
		{
			if (SNPRINTF(buff, buffLen, _TRUNCATE, "w=\"%g\" h=\"%g\"", width, height) < 0)
				buff[buffLen - 1] = 0;
		}

		/// @brief Outputs the size in the form "(width,height)" to a stream.
		/// @tparam TOSTREAM Output stream type.
		/// @param o The output stream to write to.
		template <class TOSTREAM>
		void show(TOSTREAM& o) const
		{
			o << "(" << width << "," << height << ")";
		}
	};

	//template <typename TPRIM>
	//inline td::DataType getType(const td::Size<TPRIM>&) { return td::size; }
};
