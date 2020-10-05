#pragma once

#include <cassert>

#include <type_traits>

namespace Ubpa {
    template <typename T>
    constexpr bool IsPowerOfTwo(T val) noexcept {
        if constexpr (std::is_unsigned_v<T>)
            return (val & (val - 1)) == 0;
        else
            return val > 0 && (val & (val - 1)) == 0;
    }

    template <typename T>
    constexpr T Align(T val, T alignment) noexcept {
        assert(IsPowerOfTwo(alignment));
        //    ceil(val \div alignment)
        // == (val + alignment - 1) / alignment
        // == (val + (alignment - 1)) & ~(alignment - 1)
        return (val + (alignment - 1)) & ~(alignment - 1);
    }

    template <typename T>
    constexpr T AlignDown(T val, T alignment) noexcept {
        assert(IsPowerOfTwo(alignment));
        //    floor(val \div alignment)
        // == val / alignment
        // == val & ~(alignment - 1)
        return val & ~(alignment - 1);
    }
}
