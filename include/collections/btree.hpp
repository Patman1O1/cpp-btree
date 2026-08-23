#ifndef COLLECTIONS_BTREE_HPP
#define COLLECTIONS_BTREE_HPP

// ISO Includes
#include <cstddef>

// ISO C++ Includes
#include <memory>

namespace collections {
    template<typename T, typename Allocator = std::allocator<T>>
    class btree {
    public:
         // ── Aliases ────────────────────────────────────────────────────────
         using value_type = T;

         using size_type = std::size_t;

         using difference_type = std::ptrdiff_t;

         using reference = value_type&;

         using const_reference = const value_type&;

         using pointer = value_type*;

         using const_pointer = const value_type*;
    };
} // namespace collections

#endif // #ifndef COLLECTIONS_BTREE_HPP
