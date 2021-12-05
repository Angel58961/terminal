/*++
Copyright (c) Microsoft Corporation

Module Name:
- viewport.hpp

Abstract:
- This method provides an interface for abstracting viewport operations

Author(s):
- Michael Niksa (miniksa) Nov 2015
--*/

#pragma once

namespace Microsoft::Console::Types
{
    class Viewport;

    using SomeViewports = til::some<Viewport, 4>;

    class Viewport final
    {
    public:
        Viewport() = default;

        static Viewport Empty() noexcept;

        static Viewport FromInclusive(const SMALL_RECT sr) noexcept;
        static Viewport FromExclusive(const SMALL_RECT sr) noexcept;

        static Viewport FromInclusive(const til::small_rect sr) noexcept;
        static Viewport FromExclusive(const til::small_rect sr) noexcept;

        static Viewport FromDimensions(const til::coord origin,
                                       const til::CoordType width,
                                       const til::CoordType height) noexcept;

        static Viewport FromDimensions(const til::coord origin,
                                       const til::coord dimensions) noexcept;

        static Viewport FromDimensions(const til::coord dimensions) noexcept;

        static Viewport FromCoord(const til::coord origin) noexcept;

        til::CoordType Left() const noexcept;
        til::CoordType RightInclusive() const noexcept;
        til::CoordType RightExclusive() const noexcept;
        til::CoordType Top() const noexcept;
        til::CoordType BottomInclusive() const noexcept;
        til::CoordType BottomExclusive() const noexcept;
        til::CoordType Height() const noexcept;
        til::CoordType Width() const noexcept;
        til::coord Origin() const noexcept;
        til::coord BottomRightExclusive() const noexcept;
        til::coord EndExclusive() const noexcept;
        til::coord Dimensions() const noexcept;

        bool IsInBounds(const Viewport& other) const noexcept;
        bool IsInBounds(const til::coord& pos, bool allowEndExclusive = false) const noexcept;

        void Clamp(til::coord& pos) const;
        Viewport Clamp(const Viewport& other) const noexcept;

        bool MoveInBounds(const ptrdiff_t move, til::coord& pos) const noexcept;
        bool IncrementInBounds(til::coord& pos, bool allowEndExclusive = false) const noexcept;
        bool IncrementInBoundsCircular(til::coord& pos) const noexcept;
        bool DecrementInBounds(til::coord& pos, bool allowEndExclusive = false) const noexcept;
        bool DecrementInBoundsCircular(til::coord& pos) const noexcept;
        int CompareInBounds(const til::coord& first, const til::coord& second, bool allowEndExclusive = false) const noexcept;

        enum class XWalk
        {
            LeftToRight,
            RightToLeft
        };

        enum class YWalk
        {
            TopToBottom,
            BottomToTop
        };

        struct WalkDir final
        {
            const XWalk x;
            const YWalk y;
        };

        bool WalkInBounds(til::coord& pos, const WalkDir dir, bool allowEndExclusive = false) const noexcept;
        bool WalkInBoundsCircular(til::coord& pos, const WalkDir dir, bool allowEndExclusive = false) const noexcept;
        til::coord GetWalkOrigin(const WalkDir dir) const noexcept;
        static WalkDir DetermineWalkDirection(const Viewport& source, const Viewport& target) noexcept;

        bool TrimToViewport(_Inout_ til::small_rect* const psr) const noexcept;
        bool TrimToViewport(_Inout_ SMALL_RECT* const psr) const noexcept;
        void ConvertToOrigin(_Inout_ til::small_rect* const psr) const noexcept;
        void ConvertToOrigin(_Inout_ SMALL_RECT* const psr) const noexcept;
        void ConvertToOrigin(_Inout_ til::coord* const pcoord) const noexcept;
        void ConvertFromOrigin(_Inout_ til::small_rect* const psr) const noexcept;
        void ConvertFromOrigin(_Inout_ SMALL_RECT* const psr) const noexcept;
        void ConvertFromOrigin(_Inout_ til::coord* const pcoord) const noexcept;
        [[nodiscard]] Viewport ConvertToOrigin(const Viewport& other) const noexcept;
        [[nodiscard]] Viewport ConvertFromOrigin(const Viewport& other) const noexcept;

        til::small_rect ToExclusive() const noexcept;
        til::small_rect ToInclusive() const noexcept;
        RECT ToRect() const noexcept;

        Viewport ToOrigin() const noexcept;

        bool IsValid() const noexcept;

        [[nodiscard]] static Viewport Offset(const Viewport& original, const til::coord delta);

        [[nodiscard]] static Viewport Union(const Viewport& lhs, const Viewport& rhs) noexcept;

        [[nodiscard]] static Viewport Intersect(const Viewport& lhs, const Viewport& rhs) noexcept;

        [[nodiscard]] static SomeViewports Subtract(const Viewport& original, const Viewport& removeMe) noexcept;

    private:
        Viewport(const til::small_rect sr) noexcept;

        // This is always stored as a Inclusive rect.
        til::small_rect _sr;

#if UNIT_TESTING
        friend class ViewportTests;
#endif
    };
}

inline til::coord operator-(const til::coord& a, const til::coord& b) noexcept
{
    return { a.x - b.x, a.y - b.y };
}

inline til::coord operator-(const til::coord& c) noexcept
{
    return { -c.x, -c.y };
}

inline bool operator==(const Microsoft::Console::Types::Viewport& a,
                       const Microsoft::Console::Types::Viewport& b) noexcept
{
    return a.ToInclusive() == b.ToInclusive();
}

inline bool operator!=(const Microsoft::Console::Types::Viewport& a,
                       const Microsoft::Console::Types::Viewport& b) noexcept
{
    return !(a == b);
}
