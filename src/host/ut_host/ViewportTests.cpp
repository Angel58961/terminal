// Copyright (c) Microsoft Corporation.
// Licensed under the MIT license.

#include "precomp.h"
#include "WexTestClass.h"
#include "../../inc/consoletaeftemplates.hpp"

#include "CommonState.hpp"

#include "../../types/inc/Viewport.hpp"

using namespace WEX::Common;
using namespace WEX::Logging;
using namespace WEX::TestExecution;

using Viewport = Microsoft::Console::Types::Viewport;

class ViewportTests
{
    TEST_CLASS(ViewportTests);

    TEST_METHOD(CreateEmpty)
    {
        const auto v = Viewport::Empty();

        VERIFY_ARE_EQUAL(0, v.Left());
        VERIFY_ARE_EQUAL(-1, v.RightInclusive());
        VERIFY_ARE_EQUAL(0, v.RightExclusive());
        VERIFY_ARE_EQUAL(0, v.Top());
        VERIFY_ARE_EQUAL(-1, v.BottomInclusive());
        VERIFY_ARE_EQUAL(0, v.BottomExclusive());
        VERIFY_ARE_EQUAL(0, v.Height());
        VERIFY_ARE_EQUAL(0, v.Width());
        VERIFY_ARE_EQUAL(til::coord({ 0 }), v.Origin());
        VERIFY_ARE_EQUAL(til::coord({ 0 }), v.Dimensions());
    }

    TEST_METHOD(CreateFromInclusive)
    {
        til::small_rect rect;
        rect.top = 3;
        rect.bottom = 5;
        rect.left = 10;
        rect.right = 20;

        til::coord origin;
        origin.x = rect.left;
        origin.y = rect.top;

        til::coord dimensions;
        dimensions.x = rect.right - rect.left + 1;
        dimensions.y = rect.bottom - rect.top + 1;

        const auto v = Viewport::FromInclusive(rect);

        VERIFY_ARE_EQUAL(rect.left, v.Left());
        VERIFY_ARE_EQUAL(rect.right, v.RightInclusive());
        VERIFY_ARE_EQUAL(rect.right + 1, v.RightExclusive());
        VERIFY_ARE_EQUAL(rect.top, v.Top());
        VERIFY_ARE_EQUAL(rect.bottom, v.BottomInclusive());
        VERIFY_ARE_EQUAL(rect.bottom + 1, v.BottomExclusive());
        VERIFY_ARE_EQUAL(dimensions.y, v.Height());
        VERIFY_ARE_EQUAL(dimensions.x, v.Width());
        VERIFY_ARE_EQUAL(origin, v.Origin());
        VERIFY_ARE_EQUAL(dimensions, v.Dimensions());
    }

    TEST_METHOD(CreateFromExclusive)
    {
        til::small_rect rect;
        rect.top = 3;
        rect.bottom = 5;
        rect.left = 10;
        rect.right = 20;

        til::coord origin;
        origin.x = rect.left;
        origin.y = rect.top;

        til::coord dimensions;
        dimensions.x = rect.right - rect.left;
        dimensions.y = rect.bottom - rect.top;

        const auto v = Viewport::FromExclusive(rect);

        VERIFY_ARE_EQUAL(rect.left, v.Left());
        VERIFY_ARE_EQUAL(rect.right - 1, v.RightInclusive());
        VERIFY_ARE_EQUAL(rect.right, v.RightExclusive());
        VERIFY_ARE_EQUAL(rect.top, v.Top());
        VERIFY_ARE_EQUAL(rect.bottom - 1, v.BottomInclusive());
        VERIFY_ARE_EQUAL(rect.bottom, v.BottomExclusive());
        VERIFY_ARE_EQUAL(dimensions.y, v.Height());
        VERIFY_ARE_EQUAL(dimensions.x, v.Width());
        VERIFY_ARE_EQUAL(origin, v.Origin());
        VERIFY_ARE_EQUAL(dimensions, v.Dimensions());
    }

    TEST_METHOD(CreateFromDimensionsWidthHeight)
    {
        til::small_rect rect;
        rect.top = 3;
        rect.bottom = 5;
        rect.left = 10;
        rect.right = 20;

        til::coord origin;
        origin.x = rect.left;
        origin.y = rect.top;

        til::coord dimensions;
        dimensions.x = rect.right - rect.left + 1;
        dimensions.y = rect.bottom - rect.top + 1;

        const auto v = Viewport::FromDimensions(origin, dimensions.x, dimensions.y);

        VERIFY_ARE_EQUAL(rect.left, v.Left());
        VERIFY_ARE_EQUAL(rect.right, v.RightInclusive());
        VERIFY_ARE_EQUAL(rect.right + 1, v.RightExclusive());
        VERIFY_ARE_EQUAL(rect.top, v.Top());
        VERIFY_ARE_EQUAL(rect.bottom, v.BottomInclusive());
        VERIFY_ARE_EQUAL(rect.bottom + 1, v.BottomExclusive());
        VERIFY_ARE_EQUAL(dimensions.y, v.Height());
        VERIFY_ARE_EQUAL(dimensions.x, v.Width());
        VERIFY_ARE_EQUAL(origin, v.Origin());
        VERIFY_ARE_EQUAL(dimensions, v.Dimensions());
    }

    TEST_METHOD(CreateFromDimensions)
    {
        til::small_rect rect;
        rect.top = 3;
        rect.bottom = 5;
        rect.left = 10;
        rect.right = 20;

        til::coord origin;
        origin.x = rect.left;
        origin.y = rect.top;

        til::coord dimensions;
        dimensions.x = rect.right - rect.left + 1;
        dimensions.y = rect.bottom - rect.top + 1;

        const auto v = Viewport::FromDimensions(origin, dimensions);

        VERIFY_ARE_EQUAL(rect.left, v.Left());
        VERIFY_ARE_EQUAL(rect.right, v.RightInclusive());
        VERIFY_ARE_EQUAL(rect.right + 1, v.RightExclusive());
        VERIFY_ARE_EQUAL(rect.top, v.Top());
        VERIFY_ARE_EQUAL(rect.bottom, v.BottomInclusive());
        VERIFY_ARE_EQUAL(rect.bottom + 1, v.BottomExclusive());
        VERIFY_ARE_EQUAL(dimensions.y, v.Height());
        VERIFY_ARE_EQUAL(dimensions.x, v.Width());
        VERIFY_ARE_EQUAL(origin, v.Origin());
        VERIFY_ARE_EQUAL(dimensions, v.Dimensions());
    }

    TEST_METHOD(CreateFromDimensionsNoOrigin)
    {
        til::small_rect rect;
        rect.top = 0;
        rect.left = 0;
        rect.bottom = 5;
        rect.right = 20;

        til::coord origin;
        origin.x = rect.left;
        origin.y = rect.top;

        til::coord dimensions;
        dimensions.x = rect.right - rect.left + 1;
        dimensions.y = rect.bottom - rect.top + 1;

        const auto v = Viewport::FromDimensions(dimensions);

        VERIFY_ARE_EQUAL(rect.left, v.Left());
        VERIFY_ARE_EQUAL(rect.right, v.RightInclusive());
        VERIFY_ARE_EQUAL(rect.right + 1, v.RightExclusive());
        VERIFY_ARE_EQUAL(rect.top, v.Top());
        VERIFY_ARE_EQUAL(rect.bottom, v.BottomInclusive());
        VERIFY_ARE_EQUAL(rect.bottom + 1, v.BottomExclusive());
        VERIFY_ARE_EQUAL(dimensions.y, v.Height());
        VERIFY_ARE_EQUAL(dimensions.x, v.Width());
        VERIFY_ARE_EQUAL(origin, v.Origin());
        VERIFY_ARE_EQUAL(dimensions, v.Dimensions());
    }

    TEST_METHOD(CreateFromCoord)
    {
        til::coord origin;
        origin.x = 12;
        origin.y = 24;

        const auto v = Viewport::FromCoord(origin);

        VERIFY_ARE_EQUAL(origin.x, v.Left());
        VERIFY_ARE_EQUAL(origin.x, v.RightInclusive());
        VERIFY_ARE_EQUAL(origin.x + 1, v.RightExclusive());
        VERIFY_ARE_EQUAL(origin.y, v.Top());
        VERIFY_ARE_EQUAL(origin.y, v.BottomInclusive());
        VERIFY_ARE_EQUAL(origin.y + 1, v.BottomExclusive());
        VERIFY_ARE_EQUAL(1, v.Height());
        VERIFY_ARE_EQUAL(1, v.Width());
        VERIFY_ARE_EQUAL(origin, v.Origin());
        // clang-format off
        VERIFY_ARE_EQUAL(til::coord({ 1, 1, }), v.Dimensions());
        // clang-format on
    }

    TEST_METHOD(ToRect)
    {
        til::coord origin;
        origin.x = 2;
        origin.y = 4;

        til::coord dimensions;
        dimensions.x = 10;
        dimensions.y = 20;

        const auto v = Viewport::FromDimensions(origin, dimensions);

        const RECT rc = v.ToRect();
        const auto exclusive = v.ToExclusive();

        VERIFY_ARE_EQUAL(exclusive.left, v.Left());
        VERIFY_ARE_EQUAL(rc.left, v.Left());

        VERIFY_ARE_EQUAL(exclusive.top, v.Top());
        VERIFY_ARE_EQUAL(rc.top, v.Top());

        VERIFY_ARE_EQUAL(exclusive.right, v.RightExclusive());
        VERIFY_ARE_EQUAL(rc.right, v.RightExclusive());

        VERIFY_ARE_EQUAL(exclusive.bottom, v.BottomExclusive());
        VERIFY_ARE_EQUAL(rc.bottom, v.BottomExclusive());
    }

    TEST_METHOD(IsInBoundsCoord)
    {
        til::small_rect r;
        r.top = 3;
        r.bottom = 5;
        r.left = 10;
        r.right = 20;

        const auto v = Viewport::FromInclusive(r);

        til::coord c;
        c.x = r.left;
        c.y = r.top;
        VERIFY_IS_TRUE(v.IsInBounds(c), L"Top left corner in bounds.");

        c.y = r.bottom;
        VERIFY_IS_TRUE(v.IsInBounds(c), L"Bottom left corner in bounds.");

        c.x = r.right;
        VERIFY_IS_TRUE(v.IsInBounds(c), L"Bottom right corner in bounds.");

        c.y = r.top;
        VERIFY_IS_TRUE(v.IsInBounds(c), L"Top right corner in bounds.");

        c.x++;
        VERIFY_IS_FALSE(v.IsInBounds(c), L"One right out the top right is out of bounds.");

        c.x--;
        c.y--;
        VERIFY_IS_FALSE(v.IsInBounds(c), L"One up out the top right is out of bounds.");

        c.x = r.left;
        c.y = r.top;
        c.x--;
        VERIFY_IS_FALSE(v.IsInBounds(c), L"One left out the top left is out of bounds.");

        c.x++;
        c.y--;
        VERIFY_IS_FALSE(v.IsInBounds(c), L"One up out the top left is out of bounds.");

        c.x = r.left;
        c.y = r.bottom;
        c.x--;
        VERIFY_IS_FALSE(v.IsInBounds(c), L"One left out the bottom left is out of bounds.");

        c.x++;
        c.y++;
        VERIFY_IS_FALSE(v.IsInBounds(c), L"One down out the bottom left is out of bounds.");

        c.x = r.right;
        c.y = r.bottom;
        c.x++;
        VERIFY_IS_FALSE(v.IsInBounds(c), L"One right out the bottom right is out of bounds.");

        c.x--;
        c.y++;
        VERIFY_IS_FALSE(v.IsInBounds(c), L"One down out the bottom right is out of bounds.");
    }

    TEST_METHOD(IsInBoundsViewport)
    {
        til::small_rect rect;
        rect.top = 3;
        rect.bottom = 5;
        rect.left = 10;
        rect.right = 20;

        const auto original = rect;

        const auto view = Viewport::FromInclusive(rect);

        auto test = Viewport::FromInclusive(rect);
        VERIFY_IS_TRUE(view.IsInBounds(test), L"Same size/position viewport is in bounds.");

        rect.top++;
        rect.bottom--;
        rect.left++;
        rect.right--;
        test = Viewport::FromInclusive(rect);
        VERIFY_IS_TRUE(view.IsInBounds(test), L"Viewport inscribed inside viewport is in bounds.");

        rect = original;
        rect.top--;
        test = Viewport::FromInclusive(rect);
        VERIFY_IS_FALSE(view.IsInBounds(test), L"Viewport that is one taller upwards is out of bounds.");

        rect = original;
        rect.bottom++;
        test = Viewport::FromInclusive(rect);
        VERIFY_IS_FALSE(view.IsInBounds(test), L"Viewport that is one taller downwards is out of bounds.");

        rect = original;
        rect.left--;
        test = Viewport::FromInclusive(rect);
        VERIFY_IS_FALSE(view.IsInBounds(test), L"Viewport that is one wider leftwards is out of bounds.");

        rect = original;
        rect.right++;
        test = Viewport::FromInclusive(rect);
        VERIFY_IS_FALSE(view.IsInBounds(test), L"Viewport that is one wider rightwards is out of bounds.");

        rect = original;
        rect.left++;
        rect.right++;
        rect.top++;
        rect.bottom++;
        test = Viewport::FromInclusive(rect);
        VERIFY_IS_FALSE(view.IsInBounds(test), L"Viewport offset at the origin but same size is out of bounds.");
    }

    TEST_METHOD(ClampCoord)
    {
        til::small_rect rect;
        rect.top = 3;
        rect.bottom = 5;
        rect.left = 10;
        rect.right = 20;

        const auto view = Viewport::FromInclusive(rect);

        til::coord pos;
        pos.x = rect.left;
        pos.y = rect.top;

        auto before = pos;
        view.Clamp(pos);
        VERIFY_ARE_EQUAL(before, pos, L"Verify clamp did nothing for position in top left corner.");

        pos.x = rect.left;
        pos.y = rect.bottom;
        before = pos;
        view.Clamp(pos);
        VERIFY_ARE_EQUAL(before, pos, L"Verify clamp did nothing for position in bottom left corner.");

        pos.x = rect.right;
        pos.y = rect.bottom;
        before = pos;
        view.Clamp(pos);
        VERIFY_ARE_EQUAL(before, pos, L"Verify clamp did nothing for position in bottom right corner.");

        pos.x = rect.right;
        pos.y = rect.top;
        before = pos;
        view.Clamp(pos);
        VERIFY_ARE_EQUAL(before, pos, L"Verify clamp did nothing for position in top right corner.");

        til::coord expected;
        expected.x = rect.right;
        expected.y = rect.top;

        pos = expected;
        pos.x++;
        pos.y--;
        before = pos;

        view.Clamp(pos);
        VERIFY_ARE_NOT_EQUAL(before, pos, L"Verify clamp modified position out the top right corner back.");
        VERIFY_ARE_EQUAL(expected, pos, L"Verify position was clamped into the top right corner.");

        expected.x = rect.left;
        expected.y = rect.top;

        pos = expected;
        pos.x--;
        pos.y--;
        before = pos;

        view.Clamp(pos);
        VERIFY_ARE_NOT_EQUAL(before, pos, L"Verify clamp modified position out the top left corner back.");
        VERIFY_ARE_EQUAL(expected, pos, L"Verify position was clamped into the top left corner.");

        expected.x = rect.left;
        expected.y = rect.bottom;

        pos = expected;
        pos.x--;
        pos.y++;
        before = pos;

        view.Clamp(pos);
        VERIFY_ARE_NOT_EQUAL(before, pos, L"Verify clamp modified position out the bottom left corner back.");
        VERIFY_ARE_EQUAL(expected, pos, L"Verify position was clamped into the bottom left corner.");

        expected.x = rect.right;
        expected.y = rect.bottom;

        pos = expected;
        pos.x++;
        pos.y++;
        before = pos;

        view.Clamp(pos);
        VERIFY_ARE_NOT_EQUAL(before, pos, L"Verify clamp modified position out the bottom right corner back.");
        VERIFY_ARE_EQUAL(expected, pos, L"Verify position was clamped into the bottom right corner.");

        Viewport invalidView = Viewport::Empty();
        VERIFY_THROWS_SPECIFIC(invalidView.Clamp(pos),
                               wil::ResultException,
                               [](wil::ResultException& e) { return e.GetErrorCode() == E_NOT_VALID_STATE; });
    }

    TEST_METHOD(ClampViewport)
    {
        // Create the rectangle/view we will clamp to.
        til::small_rect rect;
        rect.top = 3;
        rect.bottom = 5;
        rect.left = 10;
        rect.right = 20;

        const auto view = Viewport::FromInclusive(rect);

        Log::Comment(L"Make a rectangle that is larger than and fully encompasses our clamping rectangle.");
        til::small_rect testRect;
        testRect.top = rect.top - 3;
        testRect.bottom = rect.bottom + 3;
        testRect.left = rect.left - 3;
        testRect.right = rect.right + 3;

        auto testView = Viewport::FromInclusive(testRect);

        auto actual = view.Clamp(testView);
        VERIFY_ARE_EQUAL(view, actual, L"All sides should get reduced down to the size of the given rect.");

        Log::Comment(L"Make a rectangle that is fully inscribed inside our clamping rectangle.");
        testRect.top = rect.top + 1;
        testRect.bottom = rect.bottom - 1;
        testRect.left = rect.left + 1;
        testRect.right = rect.right - 1;
        testView = Viewport::FromInclusive(testRect);

        actual = view.Clamp(testView);
        VERIFY_ARE_EQUAL(testView, actual, L"Verify that nothing changed because this rectangle already sat fully inside the clamping rectangle.");

        Log::Comment(L"Craft a rectangle where the left is outside the right, right is outside the left, top is outside the bottom, and bottom is outside the top.");
        testRect.top = rect.bottom + 10;
        testRect.bottom = rect.top - 10;
        testRect.left = rect.right + 10;
        testRect.right = rect.left - 10;
        testView = Viewport::FromInclusive(testRect);

        Log::Comment(L"We expect it to be pulled back so each coordinate is in bounds, but the rectangle is still invalid (since left will be > right).");
        til::small_rect expected;
        expected.top = rect.bottom;
        expected.bottom = rect.top;
        expected.left = rect.right;
        expected.right = rect.left;
        const auto expectedView = Viewport::FromInclusive(expected);

        actual = view.Clamp(testView);
        VERIFY_ARE_EQUAL(expectedView, actual, L"Every dimension should be pulled just inside the clamping rectangle.");
    }

    TEST_METHOD(IncrementInBounds)
    {
        bool success = false;

        til::small_rect edges;
        edges.left = 10;
        edges.right = 19;
        edges.top = 20;
        edges.bottom = 29;

        const auto v = Viewport::FromInclusive(edges);
        til::coord original;
        til::coord screen;

        // #1 coord inside region
        original.x = screen.x = 15;
        original.y = screen.y = 25;

        success = v.IncrementInBounds(screen);

        VERIFY_IS_TRUE(success);
        VERIFY_ARE_EQUAL(screen.x, original.x + 1);
        VERIFY_ARE_EQUAL(screen.y, original.y);

        // #2 coord right edge, not bottom
        original.x = screen.x = edges.right;
        original.y = screen.y = 25;

        success = v.IncrementInBounds(screen);

        VERIFY_IS_TRUE(success);
        VERIFY_ARE_EQUAL(screen.x, edges.left);
        VERIFY_ARE_EQUAL(screen.y, original.y + 1);

        // #3 coord right edge, bottom
        original.x = screen.x = edges.right;
        original.y = screen.y = edges.bottom;

        success = v.IncrementInBounds(screen);

        VERIFY_IS_FALSE(success);
        VERIFY_ARE_EQUAL(screen.x, edges.right);
        VERIFY_ARE_EQUAL(screen.y, edges.bottom);
    }

    TEST_METHOD(IncrementInBoundsCircular)
    {
        bool success = false;

        til::small_rect edges;
        edges.left = 10;
        edges.right = 19;
        edges.top = 20;
        edges.bottom = 29;

        const auto v = Viewport::FromInclusive(edges);
        til::coord original;
        til::coord screen;

        // #1 coord inside region
        original.x = screen.x = 15;
        original.y = screen.y = 25;

        success = v.IncrementInBoundsCircular(screen);

        VERIFY_IS_TRUE(success);
        VERIFY_ARE_EQUAL(screen.x, original.x + 1);
        VERIFY_ARE_EQUAL(screen.y, original.y);

        // #2 coord right edge, not bottom
        original.x = screen.x = edges.right;
        original.y = screen.y = 25;

        success = v.IncrementInBoundsCircular(screen);

        VERIFY_IS_TRUE(success);
        VERIFY_ARE_EQUAL(screen.x, edges.left);
        VERIFY_ARE_EQUAL(screen.y, original.y + 1);

        // #3 coord right edge, bottom
        original.x = screen.x = edges.right;
        original.y = screen.y = edges.bottom;

        success = v.IncrementInBoundsCircular(screen);

        VERIFY_IS_FALSE(success);
        VERIFY_ARE_EQUAL(screen.x, edges.left);
        VERIFY_ARE_EQUAL(screen.y, edges.top);
    }

    TEST_METHOD(DecrementInBounds)
    {
        bool success = false;

        til::small_rect edges;
        edges.left = 10;
        edges.right = 19;
        edges.top = 20;
        edges.bottom = 29;

        const auto v = Viewport::FromInclusive(edges);
        til::coord original;
        til::coord screen;

        // #1 coord inside region
        original.x = screen.x = 15;
        original.y = screen.y = 25;

        success = v.DecrementInBounds(screen);

        VERIFY_IS_TRUE(success);
        VERIFY_ARE_EQUAL(screen.x, original.x - 1);
        VERIFY_ARE_EQUAL(screen.y, original.y);

        // #2 coord left edge, not top
        original.x = screen.x = edges.left;
        original.y = screen.y = 25;

        success = v.DecrementInBounds(screen);

        VERIFY_IS_TRUE(success);
        VERIFY_ARE_EQUAL(screen.x, edges.right);
        VERIFY_ARE_EQUAL(screen.y, original.y - 1);

        // #3 coord left edge, top
        original.x = screen.x = edges.left;
        original.y = screen.y = edges.top;

        success = v.DecrementInBounds(screen);

        VERIFY_IS_FALSE(success);
        VERIFY_ARE_EQUAL(screen.x, edges.left);
        VERIFY_ARE_EQUAL(screen.y, edges.top);
    }

    TEST_METHOD(DecrementInBoundsCircular)
    {
        bool success = false;

        til::small_rect edges;
        edges.left = 10;
        edges.right = 19;
        edges.top = 20;
        edges.bottom = 29;

        const auto v = Viewport::FromInclusive(edges);
        til::coord original;
        til::coord screen;

        // #1 coord inside region
        original.x = screen.x = 15;
        original.y = screen.y = 25;

        success = v.DecrementInBoundsCircular(screen);

        VERIFY_IS_TRUE(success);
        VERIFY_ARE_EQUAL(screen.x, original.x - 1);
        VERIFY_ARE_EQUAL(screen.y, original.y);

        // #2 coord left edge, not top
        original.x = screen.x = edges.left;
        original.y = screen.y = 25;

        success = v.DecrementInBoundsCircular(screen);

        VERIFY_IS_TRUE(success);
        VERIFY_ARE_EQUAL(screen.x, edges.right);
        VERIFY_ARE_EQUAL(screen.y, original.y - 1);

        // #3 coord left edge, top
        original.x = screen.x = edges.left;
        original.y = screen.y = edges.top;

        success = v.DecrementInBoundsCircular(screen);

        VERIFY_IS_FALSE(success);
        VERIFY_ARE_EQUAL(screen.x, edges.right);
        VERIFY_ARE_EQUAL(screen.y, edges.bottom);
    }

    SHORT RandomShort()
    {
        SHORT s;

        do
        {
            s = (SHORT)rand() % SHORT_MAX;
        } while (s == 0);

        return s;
    }

    TEST_METHOD(MoveInBounds)
    {
        const UINT cTestLoopInstances = 100;

        const auto sRowWidth = 20;
        VERIFY_IS_TRUE(sRowWidth > 0);

        // 20x20 box
        til::small_rect srectEdges;
        srectEdges.top = srectEdges.left = 0;
        srectEdges.bottom = srectEdges.right = sRowWidth - 1;

        const auto v = Viewport::FromInclusive(srectEdges);

        // repeat test
        for (UINT i = 0; i < cTestLoopInstances; i++)
        {
            til::coord coordPos;
            coordPos.x = RandomShort() % 20;
            coordPos.y = RandomShort() % 20;

            auto sAddAmount = RandomShort() % (sRowWidth * sRowWidth);

            til::coord coordFinal;
            coordFinal.x = (coordPos.x + sAddAmount) % sRowWidth;
            coordFinal.y = coordPos.y + ((coordPos.x + sAddAmount) / sRowWidth);

            Log::Comment(String().Format(L"Add To Position: (%d, %d)  Amount to add: %d", coordPos.y, coordPos.x, sAddAmount));

            // Movement result is expected to be true, unless there's an error.
            bool fExpectedResult = true;

            // if we've calculated past the final row, then the function will reset to the original position and the output will be false.
            if (coordFinal.y >= sRowWidth)
            {
                coordFinal = coordPos;
                fExpectedResult = false;
            }

            bool const fActualResult = v.MoveInBounds(sAddAmount, coordPos);

            VERIFY_ARE_EQUAL(fExpectedResult, fActualResult);
            VERIFY_ARE_EQUAL(coordPos.x, coordFinal.x);
            VERIFY_ARE_EQUAL(coordPos.y, coordFinal.y);

            Log::Comment(String().Format(L"Actual: (%d, %d) Expected: (%d, %d)", coordPos.y, coordPos.x, coordFinal.y, coordFinal.x));
        }
    }

    TEST_METHOD(CompareInBounds)
    {
        til::small_rect edges;
        edges.left = 10;
        edges.right = 19;
        edges.top = 20;
        edges.bottom = 29;

        const auto v = Viewport::FromInclusive(edges);

        til::coord first, second;
        first.x = 12;
        first.y = 24;
        second = first;
        second.x += 2;

        VERIFY_ARE_EQUAL(-2, v.CompareInBounds(first, second), L"Second and first on same row. Second is right of first.");
        VERIFY_ARE_EQUAL(2, v.CompareInBounds(second, first), L"Reverse params, should get opposite direction, same magnitude.");

        first.x = edges.left;
        first.y = 24;

        second.x = edges.right;
        second.y = first.y - 1;

        VERIFY_ARE_EQUAL(1, v.CompareInBounds(first, second), L"Second is up a line at the right edge from first at the line below on the left edge.");
        VERIFY_ARE_EQUAL(-1, v.CompareInBounds(second, first), L"Reverse params, should get opposite direction, same magnitude.");
    }

    TEST_METHOD(Offset)
    {
        til::small_rect edges;
        edges.top = 0;
        edges.left = 0;
        edges.right = 10;
        edges.bottom = 10;

        const auto original = Viewport::FromInclusive(edges);

        Log::Comment(L"Move down and to the right first.");
        til::coord adjust{ 7, 2 };
        til::small_rect expectedEdges;
        expectedEdges.top = edges.top + adjust.y;
        expectedEdges.bottom = edges.bottom + adjust.y;
        expectedEdges.left = edges.left + adjust.x;
        expectedEdges.right = edges.right + adjust.x;

        Viewport expected = Viewport::FromInclusive(expectedEdges);

        Viewport actual = Viewport::Offset(original, adjust);
        VERIFY_ARE_EQUAL(expected, actual);

        Log::Comment(L"Now try moving up and to the left.");
        adjust = { -3, -5 };

        expectedEdges.top = edges.top + adjust.y;
        expectedEdges.bottom = edges.bottom + adjust.y;
        expectedEdges.left = edges.left + adjust.x;
        expectedEdges.right = edges.right + adjust.x;

        expected = Viewport::FromInclusive(expectedEdges);
        actual = Viewport::Offset(original, adjust);
        VERIFY_ARE_EQUAL(expected, actual);

        Log::Comment(L"Now try adding way too much to cause an overflow.");
        adjust = { SHORT_MAX, SHORT_MAX };

        VERIFY_THROWS_SPECIFIC(const auto vp = Viewport::Offset(original, adjust),
                               wil::ResultException,
                               [](wil::ResultException& e) { return e.GetErrorCode() == HRESULT_FROM_WIN32(ERROR_ARITHMETIC_OVERFLOW); });
    }

    TEST_METHOD(Union)
    {
        til::small_rect srOne;
        srOne.left = 4;
        srOne.right = 10;
        srOne.top = 6;
        srOne.bottom = 14;
        const auto one = Viewport::FromInclusive(srOne);

        til::small_rect srTwo;
        srTwo.left = 5;
        srTwo.right = 13;
        srTwo.top = 2;
        srTwo.bottom = 10;
        const auto two = Viewport::FromInclusive(srTwo);

        til::small_rect srExpected;
        srExpected.left = srOne.left < srTwo.left ? srOne.left : srTwo.left;
        srExpected.right = srOne.right > srTwo.right ? srOne.right : srTwo.right;
        srExpected.top = srOne.top < srTwo.top ? srOne.top : srTwo.top;
        srExpected.bottom = srOne.bottom > srTwo.bottom ? srOne.bottom : srTwo.bottom;

        const auto expected = Viewport::FromInclusive(srExpected);

        const auto actual = Viewport::Union(one, two);
        VERIFY_ARE_EQUAL(expected, actual);
    }

    TEST_METHOD(Intersect)
    {
        til::small_rect srOne;
        srOne.left = 4;
        srOne.right = 10;
        srOne.top = 6;
        srOne.bottom = 14;
        const auto one = Viewport::FromInclusive(srOne);

        til::small_rect srTwo;
        srTwo.left = 5;
        srTwo.right = 13;
        srTwo.top = 2;
        srTwo.bottom = 10;
        const auto two = Viewport::FromInclusive(srTwo);

        til::small_rect srExpected;
        srExpected.left = srOne.left > srTwo.left ? srOne.left : srTwo.left;
        srExpected.right = srOne.right < srTwo.right ? srOne.right : srTwo.right;
        srExpected.top = srOne.top > srTwo.top ? srOne.top : srTwo.top;
        srExpected.bottom = srOne.bottom < srTwo.bottom ? srOne.bottom : srTwo.bottom;

        const auto expected = Viewport::FromInclusive(srExpected);

        const auto actual = Viewport::Intersect(one, two);
        VERIFY_ARE_EQUAL(expected, actual);
    }

    TEST_METHOD(SubtractFour)
    {
        til::small_rect srOriginal;
        srOriginal.top = 0;
        srOriginal.left = 0;
        srOriginal.bottom = 10;
        srOriginal.right = 10;
        const auto original = Viewport::FromInclusive(srOriginal);

        til::small_rect srRemove;
        srRemove.top = 3;
        srRemove.left = 3;
        srRemove.bottom = 6;
        srRemove.right = 6;
        const auto remove = Viewport::FromInclusive(srRemove);

        std::vector<Viewport> expected;
        // til::small_rect constructed as: Left, Top, Right, Bottom
        // Top View
        expected.emplace_back(Viewport::FromInclusive({ srOriginal.left, srOriginal.top, srOriginal.right, srRemove.top - 1 }));
        // Bottom View
        expected.emplace_back(Viewport::FromInclusive({ srOriginal.left, srRemove.bottom + 1, srOriginal.right, srOriginal.bottom }));
        // Left View
        expected.emplace_back(Viewport::FromInclusive({ srOriginal.left, srRemove.top, srRemove.left - 1, srRemove.bottom }));
        // Right View
        expected.emplace_back(Viewport::FromInclusive({ srRemove.right + 1, srRemove.top, srOriginal.right, srRemove.bottom }));

        const auto actual = Viewport::Subtract(original, remove);

        VERIFY_ARE_EQUAL(expected.size(), actual.size(), L"Same number of viewports in expected and actual");
        Log::Comment(L"Now validate that each viewport has the expected area.");
        for (size_t i = 0; i < expected.size(); i++)
        {
            const auto& exp = expected.at(i);
            const auto& act = actual.at(i);
            VERIFY_ARE_EQUAL(exp, act);
        }
    }

    TEST_METHOD(SubtractThree)
    {
        til::small_rect srOriginal;
        srOriginal.top = 0;
        srOriginal.left = 0;
        srOriginal.bottom = 10;
        srOriginal.right = 10;
        const auto original = Viewport::FromInclusive(srOriginal);

        til::small_rect srRemove;
        srRemove.top = 3;
        srRemove.left = 3;
        srRemove.bottom = 6;
        srRemove.right = 15;
        const auto remove = Viewport::FromInclusive(srRemove);

        std::vector<Viewport> expected;
        // til::small_rect constructed as: Left, Top, Right, Bottom
        // Top View
        expected.emplace_back(Viewport::FromInclusive({ srOriginal.left, srOriginal.top, srOriginal.right, srRemove.top - 1 }));
        // Bottom View
        expected.emplace_back(Viewport::FromInclusive({ srOriginal.left, srRemove.bottom + 1, srOriginal.right, srOriginal.bottom }));
        // Left View
        expected.emplace_back(Viewport::FromInclusive({ srOriginal.left, srRemove.top, srRemove.left - 1, srRemove.bottom }));

        const auto actual = Viewport::Subtract(original, remove);

        VERIFY_ARE_EQUAL(expected.size(), actual.size(), L"Same number of viewports in expected and actual");
        Log::Comment(L"Now validate that each viewport has the expected area.");
        for (size_t i = 0; i < expected.size(); i++)
        {
            const auto& exp = expected.at(i);
            const auto& act = actual.at(i);
            VERIFY_ARE_EQUAL(exp, act);
        }
    }

    TEST_METHOD(SubtractTwo)
    {
        til::small_rect srOriginal;
        srOriginal.top = 0;
        srOriginal.left = 0;
        srOriginal.bottom = 10;
        srOriginal.right = 10;
        const auto original = Viewport::FromInclusive(srOriginal);

        til::small_rect srRemove;
        srRemove.top = 3;
        srRemove.left = 3;
        srRemove.bottom = 15;
        srRemove.right = 15;
        const auto remove = Viewport::FromInclusive(srRemove);

        std::vector<Viewport> expected;
        // til::small_rect constructed as: Left, Top, Right, Bottom
        // Top View
        expected.emplace_back(Viewport::FromInclusive({ srOriginal.left, srOriginal.top, srOriginal.right, srRemove.top - 1 }));
        // Left View
        expected.emplace_back(Viewport::FromInclusive({ srOriginal.left, srRemove.top, srRemove.left - 1, srOriginal.bottom }));

        const auto actual = Viewport::Subtract(original, remove);

        VERIFY_ARE_EQUAL(expected.size(), actual.size(), L"Same number of viewports in expected and actual");
        Log::Comment(L"Now validate that each viewport has the expected area.");
        for (size_t i = 0; i < expected.size(); i++)
        {
            const auto& exp = expected.at(i);
            const auto& act = actual.at(i);
            VERIFY_ARE_EQUAL(exp, act);
        }
    }

    TEST_METHOD(SubtractOne)
    {
        til::small_rect srOriginal;
        srOriginal.top = 0;
        srOriginal.left = 0;
        srOriginal.bottom = 10;
        srOriginal.right = 10;
        const auto original = Viewport::FromInclusive(srOriginal);

        til::small_rect srRemove;
        srRemove.top = 3;
        srRemove.left = -12;
        srRemove.bottom = 15;
        srRemove.right = 15;
        const auto remove = Viewport::FromInclusive(srRemove);

        std::vector<Viewport> expected;
        // til::small_rect constructed as: Left, Top, Right, Bottom
        // Top View
        expected.emplace_back(Viewport::FromInclusive({ srOriginal.left, srOriginal.top, srOriginal.right, srRemove.top - 1 }));

        const auto foo = expected.cbegin();

        const auto actual = Viewport::Subtract(original, remove);

        VERIFY_ARE_EQUAL(expected.size(), actual.size(), L"Same number of viewports in expected and actual");
        Log::Comment(L"Now validate that each viewport has the expected area.");
        for (size_t i = 0; i < expected.size(); i++)
        {
            const auto& exp = expected.at(i);
            const auto& act = actual.at(i);
            VERIFY_ARE_EQUAL(exp, act);
        }
    }

    TEST_METHOD(SubtractZero)
    {
        til::small_rect srOriginal;
        srOriginal.top = 0;
        srOriginal.left = 0;
        srOriginal.bottom = 10;
        srOriginal.right = 10;
        const auto original = Viewport::FromInclusive(srOriginal);

        til::small_rect srRemove;
        srRemove.top = 12;
        srRemove.left = 12;
        srRemove.bottom = 15;
        srRemove.right = 15;
        const auto remove = Viewport::FromInclusive(srRemove);

        std::vector<Viewport> expected;
        expected.emplace_back(original);

        const auto actual = Viewport::Subtract(original, remove);

        VERIFY_ARE_EQUAL(expected.size(), actual.size(), L"Same number of viewports in expected and actual");
        Log::Comment(L"Now validate that each viewport has the expected area.");
        for (size_t i = 0; i < expected.size(); i++)
        {
            const auto& exp = expected.at(i);
            const auto& act = actual.at(i);
            VERIFY_ARE_EQUAL(exp, act);
        }
    }

    TEST_METHOD(SubtractSame)
    {
        til::small_rect srOriginal;
        srOriginal.top = 0;
        srOriginal.left = 0;
        srOriginal.bottom = 10;
        srOriginal.right = 10;
        const auto original = Viewport::FromInclusive(srOriginal);
        const auto remove = original;

        const auto actual = Viewport::Subtract(original, remove);

        VERIFY_ARE_EQUAL(0u, actual.size(), L"There should be no viewports returned");
    }
};
