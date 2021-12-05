// Copyright (c) Microsoft Corporation.
// Licensed under the MIT license.

#pragma once

#include "point.h"

#ifdef UNIT_TESTING
class SizeTests;
#endif

namespace til // Terminal Implementation Library. Also: "Today I Learned"
{
    class size
    {
    public:
        constexpr size() noexcept = default;

        constexpr size(CoordType width, CoordType height) noexcept :
            _width(width),
            _height(height)
        {
        }

        template<typename T>
        size(const vec2<T>& other) :
            size(static_cast<CoordType>(other.x), static_cast<CoordType>(other.y))
        {
        }

        // This template will convert to size from anything that has an X and a Y field that appear convertible to an integer value
        template<typename TOther>
        constexpr size(const TOther& other, std::enable_if_t<std::is_integral_v<decltype(std::declval<TOther>().X)> && std::is_integral_v<decltype(std::declval<TOther>().Y)>, int> /*sentinel*/ = 0) :
            size(static_cast<CoordType>(other.X), static_cast<CoordType>(other.Y))
        {
        }

        // This template will convert to size from anything that has a cx and a cy field that appear convertible to an integer value
        template<typename TOther>
        constexpr size(const TOther& other, std::enable_if_t<std::is_integral_v<decltype(std::declval<TOther>().cx)> && std::is_integral_v<decltype(std::declval<TOther>().cy)>, int> /*sentinel*/ = 0) :
            size(static_cast<CoordType>(other.cx), static_cast<CoordType>(other.cy))
        {
        }

        // This template will convert to size from anything that has a X and a Y field that are floating-point;
        // a math type is required. If you _don't_ provide one, you're going to
        // get a compile-time error about "cannot convert from initializer-list to til::size"
        template<typename TilMath, typename TOther>
        constexpr size(TilMath, const TOther& other, std::enable_if_t<std::is_floating_point_v<decltype(std::declval<TOther>().X)> && std::is_floating_point_v<decltype(std::declval<TOther>().Y)>, int> /*sentinel*/ = 0) :
            size(TilMath::template cast<CoordType>(other.X), TilMath::template cast<CoordType>(other.Y))
        {
        }

        // This template will convert to size from anything that has a cx and a cy field that are floating-point;
        // a math type is required. If you _don't_ provide one, you're going to
        // get a compile-time error about "cannot convert from initializer-list to til::size"
        template<typename TilMath, typename TOther>
        constexpr size(TilMath, const TOther& other, std::enable_if_t<std::is_floating_point_v<decltype(std::declval<TOther>().cx)> && std::is_floating_point_v<decltype(std::declval<TOther>().cy)>, int> /*sentinel*/ = 0) :
            size(TilMath::template cast<CoordType>(other.cx), TilMath::template cast<CoordType>(other.cy))
        {
        }

        // This template will convert to size from anything that has a Width and a Height field that are floating-point;
        // a math type is required. If you _don't_ provide one, you're going to
        // get a compile-time error about "cannot convert from initializer-list to til::size"
        template<typename TilMath, typename TOther>
        constexpr size(TilMath, const TOther& other, std::enable_if_t<std::is_floating_point_v<decltype(std::declval<TOther>().Width)> && std::is_floating_point_v<decltype(std::declval<TOther>().Height)>, int> /*sentinel*/ = 0) :
            size(TilMath::template cast<CoordType>(other.Width), TilMath::template cast<CoordType>(other.Height))
        {
        }

        // This template will convert to size from floating-point args;
        // a math type is required. If you _don't_ provide one, you're going to
        // get a compile-time error about "cannot convert from initializer-list to til::size"
        template<typename TilMath, typename TOther>
        constexpr size(TilMath, const TOther& width, const TOther& height, std::enable_if_t<std::is_floating_point_v<TOther>, int> /*sentinel*/ = 0) :
            size(TilMath::template cast<CoordType>(width), TilMath::template cast<CoordType>(height))
        {
        }

        constexpr bool operator==(const size& other) const noexcept
        {
            return _width == other._width &&
                   _height == other._height;
        }

        constexpr bool operator!=(const size& other) const noexcept
        {
            return !(*this == other);
        }

        constexpr explicit operator bool() const noexcept
        {
            return _width > 0 && _height > 0;
        }

        size operator+(const size& other) const
        {
            CoordType width;
            THROW_HR_IF(E_ABORT, !base::CheckAdd(_width, other._width).AssignIfValid(&width));

            CoordType height;
            THROW_HR_IF(E_ABORT, !base::CheckAdd(_height, other._height).AssignIfValid(&height));

            return size{ width, height };
        }

        size operator-(const size& other) const
        {
            CoordType width;
            THROW_HR_IF(E_ABORT, !base::CheckSub(_width, other._width).AssignIfValid(&width));

            CoordType height;
            THROW_HR_IF(E_ABORT, !base::CheckSub(_height, other._height).AssignIfValid(&height));

            return size{ width, height };
        }

        size operator*(const size& other) const
        {
            CoordType width;
            THROW_HR_IF(E_ABORT, !base::CheckMul(_width, other._width).AssignIfValid(&width));

            CoordType height;
            THROW_HR_IF(E_ABORT, !base::CheckMul(_height, other._height).AssignIfValid(&height));

            return size{ width, height };
        }

        template<typename TilMath>
        size scale(TilMath, const float scale) const
        {
            struct
            {
                float Width, Height;
            } sz;
            THROW_HR_IF(E_ABORT, !base::CheckMul(scale, _width).AssignIfValid(&sz.Width));
            THROW_HR_IF(E_ABORT, !base::CheckMul(scale, _height).AssignIfValid(&sz.Height));

            return til::size(TilMath(), sz);
        }

        size operator/(const size& other) const
        {
            CoordType width;
            THROW_HR_IF(E_ABORT, !base::CheckDiv(_width, other._width).AssignIfValid(&width));

            CoordType height;
            THROW_HR_IF(E_ABORT, !base::CheckDiv(_height, other._height).AssignIfValid(&height));

            return size{ width, height };
        }

        size divide_ceil(const size& other) const
        {
            // Divide normally to get the floor.
            const size floor = *this / other;

            CoordType adjWidth = 0;
            CoordType adjHeight = 0;

            // Check for width remainder, anything not 0.
            // If we multiply the floored number with the other, it will equal
            // the old width if there was no remainder.
            if (other._width * floor._width != _width)
            {
                // If there was any remainder,
                // Grow the magnitude by 1 in the
                // direction of the sign.
                if (floor.width() >= 0)
                {
                    ++adjWidth;
                }
                else
                {
                    --adjWidth;
                }
            }

            // Check for height remainder, anything not 0.
            // If we multiply the floored number with the other, it will equal
            // the old width if there was no remainder.
            if (other._height * floor._height != _height)
            {
                // If there was any remainder,
                // Grow the magnitude by 1 in the
                // direction of the sign.
                if (_height >= 0)
                {
                    ++adjHeight;
                }
                else
                {
                    --adjHeight;
                }
            }

            return floor + size{ adjWidth, adjHeight };
        }

        constexpr CoordType width() const noexcept
        {
            return _width;
        }

        template<typename T>
        T width() const
        {
            T ret;
            THROW_HR_IF(E_ABORT, !base::MakeCheckedNum(width()).AssignIfValid(&ret));
            return ret;
        }

        constexpr CoordType height() const noexcept
        {
            return _height;
        }

        template<typename T>
        T height() const
        {
            T ret;
            THROW_HR_IF(E_ABORT, !base::MakeCheckedNum(height()).AssignIfValid(&ret));
            return ret;
        }

        CoordType area() const
        {
            CoordType result;
            THROW_HR_IF(E_ABORT, !base::CheckMul(_width, _height).AssignIfValid(&result));
            return result;
        }

        template<typename T>
        T area() const
        {
            T ret;
            THROW_HR_IF(E_ABORT, !base::CheckMul(_width, _height).AssignIfValid(&ret));
            return ret;
        }

#ifdef _WINCONTYPES_
        operator COORD() const
        {
            COORD ret;
            THROW_HR_IF(E_ABORT, !base::MakeCheckedNum(_width).AssignIfValid(&ret.X));
            THROW_HR_IF(E_ABORT, !base::MakeCheckedNum(_height).AssignIfValid(&ret.Y));
            return ret;
        }
#endif

#ifdef _WINDEF_
        operator SIZE() const
        {
            SIZE ret;
            THROW_HR_IF(E_ABORT, !base::MakeCheckedNum(_width).AssignIfValid(&ret.cx));
            THROW_HR_IF(E_ABORT, !base::MakeCheckedNum(_height).AssignIfValid(&ret.cy));
            return ret;
        }
#endif

#ifdef DCOMMON_H_INCLUDED
        constexpr operator D2D1_SIZE_F() const noexcept
        {
            return D2D1_SIZE_F{ gsl::narrow_cast<float>(_width), gsl::narrow_cast<float>(_height) };
        }
#endif

        std::wstring to_string() const
        {
            return wil::str_printf<std::wstring>(L"[W:%td, H:%td]", width(), height());
        }

    protected:
        CoordType _width = 0;
        CoordType _height = 0;

#ifdef UNIT_TESTING
        friend class ::SizeTests;
#endif
    };
};

#ifdef __WEX_COMMON_H__
namespace WEX::TestExecution
{
    template<>
    class VerifyOutputTraits<::til::size>
    {
    public:
        static WEX::Common::NoThrowString ToString(const ::til::size& size)
        {
            return WEX::Common::NoThrowString(size.to_string().c_str());
        }
    };

    template<>
    class VerifyCompareTraits<::til::size, ::til::size>
    {
    public:
        static bool AreEqual(const ::til::size& expected, const ::til::size& actual) noexcept
        {
            return expected == actual;
        }

        static bool AreSame(const ::til::size& expected, const ::til::size& actual) noexcept
        {
            return &expected == &actual;
        }

        static bool IsLessThan(const ::til::size& expectedLess, const ::til::size& expectedGreater) = delete;

        static bool IsGreaterThan(const ::til::size& expectedGreater, const ::til::size& expectedLess) = delete;

        static bool IsNull(const ::til::size& object) noexcept
        {
            return object == til::size{};
        }
    };
};
#endif
