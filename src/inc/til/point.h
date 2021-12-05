// Copyright (c) Microsoft Corporation.
// Licensed under the MIT license.

#pragma once

#include "vec.h"

#ifdef UNIT_TESTING
class PointTests;
#endif

namespace til // Terminal Implementation Library. Also: "Today I Learned"
{
    using CoordType = int32_t;
    using coord = til::vec2<CoordType>;

    constexpr coord wrap_coord(const COORD& p) noexcept
    {
        return { p.X, p.Y };
    }

    constexpr COORD unwrap_coord(const coord& p) noexcept
    {
        return { gsl::narrow<short>(p.x), gsl::narrow<short>(p.y) };
    }

    class point
    {
    public:
        constexpr point() noexcept = default;
        
        constexpr point(CoordType x, CoordType y) noexcept :
            _x(x),
            _y(y)
        {
        }

        template<typename T>
        point(const vec2<T>& other) :
            point(static_cast<CoordType>(other.x), static_cast<CoordType>(other.y))
        {
        }

        // This template will convert to size from anything that has an X and a Y field that appear convertible to an integer value
        template<typename TOther>
        constexpr point(const TOther& other, std::enable_if_t<std::is_integral_v<decltype(std::declval<TOther>().X)> && std::is_integral_v<decltype(std::declval<TOther>().Y)>, int> /*sentinel*/ = 0) :
            point(static_cast<CoordType>(other.X), static_cast<CoordType>(other.Y))
        {
        }

        // This template will convert to size from anything that has a x and a y field that appear convertible to an integer value
        template<typename TOther>
        constexpr point(const TOther& other, std::enable_if_t<std::is_integral_v<decltype(std::declval<TOther>().x)> && std::is_integral_v<decltype(std::declval<TOther>().y)>, int> /*sentinel*/ = 0) :
            point(static_cast<CoordType>(other.x), static_cast<CoordType>(other.y))
        {
        }

        // This template will convert to point from floating-point args;
        // a math type is required. If you _don't_ provide one, you're going to
        // get a compile-time error about "cannot convert from initializer-list to til::point"
        template<typename TilMath, typename TOther>
        constexpr point(TilMath, const TOther& x, const TOther& y, std::enable_if_t<std::is_floating_point_v<TOther>, int> /*sentinel*/ = 0) :
            point(TilMath::template cast<CoordType>(x), TilMath::template cast<CoordType>(y))
        {
        }

        // This template will convert to size from anything that has a X and a Y field that are floating-point;
        // a math type is required. If you _don't_ provide one, you're going to
        // get a compile-time error about "cannot convert from initializer-list to til::point"
        template<typename TilMath, typename TOther>
        constexpr point(TilMath, const TOther& other, std::enable_if_t<std::is_floating_point_v<decltype(std::declval<TOther>().X)> && std::is_floating_point_v<decltype(std::declval<TOther>().Y)>, int> /*sentinel*/ = 0) :
            point(TilMath::template cast<CoordType>(other.X), TilMath::template cast<CoordType>(other.Y))
        {
        }

        // This template will convert to size from anything that has a x and a y field that are floating-point;
        // a math type is required. If you _don't_ provide one, you're going to
        // get a compile-time error about "cannot convert from initializer-list to til::point"
        template<typename TilMath, typename TOther>
        constexpr point(TilMath, const TOther& other, std::enable_if_t<std::is_floating_point_v<decltype(std::declval<TOther>().x)> && std::is_floating_point_v<decltype(std::declval<TOther>().y)>, int> /*sentinel*/ = 0) :
            point(TilMath::template cast<CoordType>(other.x), TilMath::template cast<CoordType>(other.y))
        {
        }

        constexpr bool operator==(const point& other) const noexcept
        {
            return _x == other._x &&
                   _y == other._y;
        }

        constexpr bool operator!=(const point& other) const noexcept
        {
            return !(*this == other);
        }

        constexpr bool operator<(const point& other) const noexcept
        {
            if (_y < other._y)
            {
                return true;
            }
            else if (_y > other._y)
            {
                return false;
            }
            else
            {
                return _x < other._x;
            }
        }

        constexpr bool operator>(const point& other) const noexcept
        {
            if (_y > other._y)
            {
                return true;
            }
            else if (_y < other._y)
            {
                return false;
            }
            else
            {
                return _x > other._x;
            }
        }

        constexpr bool operator<=(const point& other) const noexcept
        {
            if (_y < other._y)
            {
                return true;
            }
            else if (_y > other._y)
            {
                return false;
            }
            else
            {
                return _x <= other._x;
            }
        }

        constexpr bool operator>=(const point& other) const noexcept
        {
            if (_y > other._y)
            {
                return true;
            }
            else if (_y < other._y)
            {
                return false;
            }
            else
            {
                return _x >= other._x;
            }
        }

        point operator+(const point& other) const
        {
            CoordType x;
            THROW_HR_IF(E_ABORT, !base::CheckAdd(_x, other._x).AssignIfValid(&x));

            CoordType y;
            THROW_HR_IF(E_ABORT, !base::CheckAdd(_y, other._y).AssignIfValid(&y));

            return point{ x, y };
        }

        point& operator+=(const point& other)
        {
            *this = *this + other;
            return *this;
        }

        point operator-(const point& other) const
        {
            CoordType x;
            THROW_HR_IF(E_ABORT, !base::CheckSub(_x, other._x).AssignIfValid(&x));

            CoordType y;
            THROW_HR_IF(E_ABORT, !base::CheckSub(_y, other._y).AssignIfValid(&y));

            return point{ x, y };
        }

        point& operator-=(const point& other)
        {
            *this = *this - other;
            return *this;
        }

        point operator*(const point& other) const
        {
            CoordType x;
            THROW_HR_IF(E_ABORT, !base::CheckMul(_x, other._x).AssignIfValid(&x));

            CoordType y;
            THROW_HR_IF(E_ABORT, !base::CheckMul(_y, other._y).AssignIfValid(&y));

            return point{ x, y };
        }

        point& operator*=(const point& other)
        {
            *this = *this * other;
            return *this;
        }

        template<typename TilMath>
        point scale(TilMath, const float scale) const
        {
            struct
            {
                float x, y;
            } pt;
            THROW_HR_IF(E_ABORT, !base::CheckMul(scale, _x).AssignIfValid(&pt.x));
            THROW_HR_IF(E_ABORT, !base::CheckMul(scale, _y).AssignIfValid(&pt.y));

            return til::point(TilMath(), pt);
        }

        point operator/(const point& other) const
        {
            CoordType x;
            THROW_HR_IF(E_ABORT, !base::CheckDiv(_x, other._x).AssignIfValid(&x));

            CoordType y;
            THROW_HR_IF(E_ABORT, !base::CheckDiv(_y, other._y).AssignIfValid(&y));

            return point{ x, y };
        }

        point& operator/=(const point& other)
        {
            *this = *this / other;
            return *this;
        }

        template<typename T>
        point operator*(const T& scale) const
        {
            static_assert(std::is_arithmetic<T>::value, "Type must be arithmetic");
            CoordType x;
            THROW_HR_IF(E_ABORT, !base::CheckMul(_x, scale).AssignIfValid(&x));

            CoordType y;
            THROW_HR_IF(E_ABORT, !base::CheckMul(_y, scale).AssignIfValid(&y));

            return point{ x, y };
        }

        template<typename T>
        point operator/(const T& scale) const
        {
            static_assert(std::is_arithmetic<T>::value, "Type must be arithmetic");
            CoordType x;
            THROW_HR_IF(E_ABORT, !base::CheckDiv(_x, scale).AssignIfValid(&x));

            CoordType y;
            THROW_HR_IF(E_ABORT, !base::CheckDiv(_y, scale).AssignIfValid(&y));

            return point{ x, y };
        }

        constexpr CoordType x() const noexcept
        {
            return _x;
        }

        template<typename T>
        T x() const
        {
            T ret;
            THROW_HR_IF(E_ABORT, !base::MakeCheckedNum(x()).AssignIfValid(&ret));
            return ret;
        }

        constexpr CoordType y() const noexcept
        {
            return _y;
        }

        template<typename T>
        T y() const
        {
            T ret;
            THROW_HR_IF(E_ABORT, !base::MakeCheckedNum(y()).AssignIfValid(&ret));
            return ret;
        }

        template<typename T>
        T to_vec() const
        {
            T ret;
            using I = decltype(ret.x);
            ret.x = gsl::narrow<I>(_x);
            ret.y = gsl::narrow<I>(_y);
            return ret;
        }

#ifdef _WINCONTYPES_
        operator COORD() const
        {
            COORD ret;
            THROW_HR_IF(E_ABORT, !base::MakeCheckedNum(_x).AssignIfValid(&ret.X));
            THROW_HR_IF(E_ABORT, !base::MakeCheckedNum(_y).AssignIfValid(&ret.Y));
            return ret;
        }
#endif

#ifdef _WINDEF_
        operator POINT() const
        {
            POINT ret;
            THROW_HR_IF(E_ABORT, !base::MakeCheckedNum(_x).AssignIfValid(&ret.x));
            THROW_HR_IF(E_ABORT, !base::MakeCheckedNum(_y).AssignIfValid(&ret.y));
            return ret;
        }
#endif

#ifdef DCOMMON_H_INCLUDED
        constexpr operator D2D1_POINT_2F() const noexcept
        {
            return D2D1_POINT_2F{ gsl::narrow_cast<float>(_x), gsl::narrow_cast<float>(_y) };
        }
#endif

#ifdef WINRT_Windows_Foundation_H
        operator winrt::Windows::Foundation::Point() const
        {
            winrt::Windows::Foundation::Point ret;
            THROW_HR_IF(E_ABORT, !base::MakeCheckedNum(_x).AssignIfValid(&ret.X));
            THROW_HR_IF(E_ABORT, !base::MakeCheckedNum(_y).AssignIfValid(&ret.Y));
            return ret;
        }
#endif

#ifdef WINRT_Microsoft_Terminal_Core_H
        constexpr point(const winrt::Microsoft::Terminal::Core::Point& corePoint) :
            point(corePoint.X, corePoint.Y)
        {
        }

        operator winrt::Microsoft::Terminal::Core::Point() const
        {
            winrt::Microsoft::Terminal::Core::Point ret;
            ret.X = x<int>();
            ret.Y = y<int>();
            return ret;
        }
#endif

        std::wstring to_string() const
        {
            return wil::str_printf<std::wstring>(L"(X:%td, Y:%td)", x(), y());
        }

    protected:
        CoordType _x = 0;
        CoordType _y = 0;

#ifdef UNIT_TESTING
        friend class ::PointTests;
#endif
    };
}

#ifdef __WEX_COMMON_H__
namespace WEX::TestExecution
{
    template<>
    class VerifyOutputTraits<::til::point>
    {
    public:
        static WEX::Common::NoThrowString ToString(const ::til::point& point)
        {
            return WEX::Common::NoThrowString(point.to_string().c_str());
        }
    };

    template<>
    class VerifyCompareTraits<::til::point, ::til::point>
    {
    public:
        static bool AreEqual(const ::til::point& expected, const ::til::point& actual) noexcept
        {
            return expected == actual;
        }

        static bool AreSame(const ::til::point& expected, const ::til::point& actual) noexcept
        {
            return &expected == &actual;
        }

        static bool IsLessThan(const ::til::point& expectedLess, const ::til::point& expectedGreater) = delete;

        static bool IsGreaterThan(const ::til::point& expectedGreater, const ::til::point& expectedLess) = delete;

        static bool IsNull(const ::til::point& object) noexcept
        {
            return object == til::point{};
        }
    };
};
#endif
