// Copyright (c) Microsoft Corporation.
// Licensed under the MIT license.

#pragma once

namespace til // Terminal Implementation Library. Also: "Today I Learned"
{
    template<typename T>
    struct vec2
    {
        T x{};
        T y{};
        
        constexpr bool operator==(const vec2& rhs) const noexcept
        {
            return __builtin_memcmp(this, &rhs, sizeof(rhs)) == 0;
        }

        constexpr bool operator!=(const vec2& rhs) const noexcept
        {
            return __builtin_memcmp(this, &rhs, sizeof(rhs)) != 0;
        }
    };

    using u16x2 = vec2<uint16_t>;
    using i16x2 = vec2<int16_t>;
    using u32x2 = vec2<uint32_t>;
    using i32x2 = vec2<int32_t>;

    template<typename T>
    struct vec4
    {
        union
        {
            T x{};
            T r;
        };
        union
        {
            T y{};
            T g;
        };
        union
        {
            T z{};
            T b;
        };
        union
        {
            T w{};
            T a;
        };

        constexpr bool operator==(const vec4& rhs) const noexcept
        {
            return __builtin_memcmp(this, &rhs, sizeof(rhs)) == 0;
        }

        constexpr bool operator!=(const vec4& rhs) const noexcept
        {
            return __builtin_memcmp(this, &rhs, sizeof(rhs)) != 0;
        }
    };

    using u16x4 = vec4<uint16_t>;
    using i16x4 = vec4<int16_t>;
    using u32x4 = vec4<uint32_t>;
    using i32x4 = vec4<int32_t>;
};
