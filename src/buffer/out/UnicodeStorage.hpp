/*++
Copyright (c) Microsoft Corporation
Licensed under the MIT license.

Module Name:
- UnicodeStorage.hpp

Abstract:
- dynamic storage location for glyphs that can't normally fit in the output buffer

Author(s):
- Austin Diviness (AustDi) 02-May-2018
--*/

#pragma once

#include <vector>
#include <unordered_map>
#include <climits>

// std::unordered_map needs help to know how to hash a til::coord
namespace std
{
    template<>
    struct hash<til::coord>
    {
        // Routine Description:
        // - hashes a coord. coord will be hashed by storing the x and y values consecutively in the lower
        // bits of a size_t.
        // Arguments:
        // - coord - the coord to hash
        // Return Value:
        // - the hashed coord
        constexpr size_t operator()(const til::coord& coord) const noexcept
        {
            size_t retVal = coord.y;
            const size_t xCoord = coord.x;
            retVal |= xCoord << (sizeof(coord.y) * CHAR_BIT);
            return retVal;
        }
    };
}

class UnicodeStorage final
{
public:
    using key_type = typename til::coord;
    using mapped_type = typename std::vector<wchar_t>;

    UnicodeStorage() noexcept;

    const mapped_type& GetText(const key_type key) const;

    void StoreGlyph(const key_type key, const mapped_type& glyph);

    void Erase(const key_type key) noexcept;

    void Remap(const std::unordered_map<til::CoordType, til::CoordType>& rowMap, const std::optional<til::CoordType> width);

private:
    std::unordered_map<key_type, mapped_type> _map;

#ifdef UNIT_TESTING
    friend class UnicodeStorageTests;
    friend class TextBufferTests;
#endif
};
