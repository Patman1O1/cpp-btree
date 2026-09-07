#ifndef COLLECTIONS_LIST_HPP
#define COLLECTIONS_LIST_HPP

// ISO C Includes
#include <cstddef>

// ISO C++ Includes
#include <concepts>
#include <ranges>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <type_traits>

namespace collections {
    template<typename T, typename Allocator = std::allocator<T>>
    class btree {
    public:
        // ── Forward Declarations ────────────────────────────────────────────
        class iterator;

        class const_iterator;
    private:
        // ── Forward Declarations ────────────────────────────────────────────
        struct node;

    public:
        // ── Aliases ─────────────────────────────────────────────────────────
        using value_type = T;

        using allocator_type = Allocator;

        using size_type = std::size_t;

        using difference_type = std::ptrdiff_t;

        using pointer = std::allocator_traits<allocator_type>::pointer;

        using const_pointer = std::allocator_traits<
            allocator_type
        >::const_pointer;

        using reference = value_type&;

        using const_reference = const value_type&;

        using reverse_iterator = std::reverse_iterator<iterator>;

        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    private:
        // ── node ────────────────────────────────────────────────────────────
        struct node {
            // ── Fields ──────────────────────────────────────────────────────
            value_type value;

            struct node* parent;

            struct node* left;

            struct node* right;
        };

        // ── Aliases ─────────────────────────────────────────────────────────
        using node_allocator_type = std::allocator_traits<
            allocator_type
        >::template rebind_alloc<struct node>;

        // ── Fields ──────────────────────────────────────────────────────────
        struct node* root_;

        size_type sz_;

        [[no_unique_address]]
        allocator_type alloc_;

    public:
        // ── iterator ────────────────────────────────────────────────────────
        class iterator {
        public:
            // ── Aliases ─────────────────────────────────────────────────────
            using iterator_category = std::bidirectional_iterator_tag;

            using iterator_concept = std::bidirectional_iterator_tag;

            using value_type = typename btree::value_type;

            using size_type = typename btree::size_type;

            using difference_type = typename btree::difference_type;
            
            using reference = value_type&;

            using const_reference = const value_type&;

            using pointer = std::allocator_traits<
                btree::allocator_type
            >::pointer;

            using const_pointer = std::allocator_traits<
                btree::allocator_type
            >::const_pointer;

        private:
            // ── Friends ─────────────────────────────────────────────────────
            friend class btree;

            // ── Fields ──────────────────────────────────────────────────────
            struct node* pos_;

        public:
            // ── Constructors ────────────────────────────────────────────────
            // TODO: Need to test
            constexpr iterator() noexcept : pos_(nullptr) {}

            // TODO: Need to test
            explicit constexpr iterator(
                const btree& tree
            ) : pos_(tree.root_) {}

            // TODO: Need to test
            constexpr iterator(const iterator&) = default;

            // TODO: Need to test
            constexpr iterator(iterator&& other) noexcept : pos_(other.pos_) {
                other.pos_ = nullptr;
            }

            // ── Destructor ──────────────────────────────────────────────────
            constexpr ~iterator() noexcept = default;

            // ── Overloaded Operators ────────────────────────────────────────
            constexpr auto operator=(const iterator&) -> iterator& = default;

            constexpr auto operator=(
                iterator&&
            ) noexcept -> iterator& = default;

            [[nodiscard]]
            constexpr auto operator==(const iterator&) const -> bool = default;

            [[nodiscard]]
            constexpr auto operator!=(const iterator&) const -> bool = default;

            // TODO: Need to test
            [[nodiscard]]
            constexpr auto operator*() const noexcept -> reference {
                return this->pos_->value;
            }

            // TODO: Need to test
            [[nodiscard]]
            constexpr auto operator->() const noexcept -> pointer {
                return std::addressof(this->pos_->value);
            }

            // TODO: Need to implement
            constexpr auto operator++() noexcept -> iterator&;

            // TODO: Need to test
            constexpr auto operator++(int) noexcept -> iterator& {
                iterator tmp = *this;
                ++(*this);
                return *this;
            }

            // TODO: Need to implement
            constexpr auto operator--() noexcept -> iterator&;

            // TODO: Need to test
            constexpr auto operator--(int) noexcept -> iterator& {
                iterator tmp = *this;
                --(*this);
                return tmp;
            }
        };

        // ── const_iterator ──────────────────────────────────────────────────
        class const_iterator {
        public:
            // ── Aliases ─────────────────────────────────────────────────────
            using iterator_category = std::bidirectional_iterator_tag;

            using iterator_concept = std::bidirectional_iterator_tag;

            using value_type = typename btree::value_type;

            using size_type = typename btree::size_type;

            using difference_type = typename btree::difference_type;
            
            using reference = value_type&;

            using const_reference = const value_type&;

            using pointer = std::allocator_traits<
                btree::allocator_type
            >::pointer;

            using const_pointer = std::allocator_traits<
                btree::allocator_type
            >::const_pointer;

        private:
            // ── Friends ─────────────────────────────────────────────────────
            friend class list;

            // ── Fields ──────────────────────────────────────────────────────
            struct node* pos_;

        public:
            // ── Constructors ────────────────────────────────────────────────
            // TODO: Need to test
            constexpr const_iterator() noexcept : pos_(nullptr) {}

            // TODO: Need to test
            explicit constexpr const_iterator(
                const btree& tree
            ) : pos_(tree.root_) {}

            constexpr const_iterator(const const_iterator&) = default;

            constexpr const_iterator(const_iterator&&) noexcept = default;

            // ── Destructor ──────────────────────────────────────────────────
            constexpr ~const_iterator() noexcept = default;

            // ── Overloaded Operators ────────────────────────────────────────
            constexpr auto operator=(
                const const_iterator&
            ) -> const_iterator& = default;

            constexpr auto operator=(
                const_iterator&&
            ) noexcept -> const_iterator& = default;

            constexpr auto operator==(
                const const_iterator&
            ) const noexcept -> bool = default;

            constexpr auto operator!=(
                const const_iterator&
            ) const noexcept -> bool = default;

            // TODO: Need to test
            constexpr auto operator*() const noexcept -> reference {
                return this->pos_->value;
            }

            // TODO: Need to test
            constexpr auto operator->() const noexcept -> pointer {
                return std::addressof(this->pos_->value);
            }

            // TODO: Need to implement
            constexpr auto operator++() noexcept -> iterator&;

            // TODO: Need to test
            constexpr auto operator++(int) noexcept -> const_iterator& {
                const_iterator tmp = this;
                ++(*this);
                return tmp;
            }

            // TODO: Need to implement
            constexpr auto operator--() noexcept -> const_iterator&;

            // TODO: Need to test
            constexpr auto operator--(int) noexcept -> const_iterator& {
                const_iterator tmp = *this;
                --(*this);
                return tmp;
            }
        };

        // ── Constructors ────────────────────────────────────────────────────
        // TODO: Need to test
        constexpr btree()
            : root_(nullptr),
              sz_(0),
              alloc_(allocator_type()) {}

        // TODO: Need to test
        explicit constexpr btree(
            const allocator_type& alloc
        ) : root_(nullptr), sz_(0), alloc_(alloc) {}

        // TODO: Need to implement
        explicit constexpr btree(
            const size_type count,
            const allocator_type& alloc = allocator_type()
        ) : sz_(count), alloc_(alloc) {
            throw std::runtime_error("Not implemented");
        }

        // TODO: Need to implement
        constexpr btree(
            const size_type count,
            const_reference value,
            const allocator_type& alloc = allocator_type()
        ) : sz_(count), alloc_(alloc) {
            throw std::runtime_error("Not implemented");
        }

        // TODO: Need to implement
        template<std::input_iterator InputIt>
        constexpr btree(
            InputIt first,
            InputIt last,
            const allocator_type& alloc = allocator_type()
        ) : sz_(std::distance(first, last)), alloc_(alloc) {
            throw std::runtime_error("Not implemented");
        }
        
        // TODO: Need to implement
        template<std::ranges::input_range R> requires(
            std::convertible_to<std::ranges::range_reference_t<R>, value_type>
        )
        constexpr list(
            std::from_range_t,
            R&& rg,
            const allocator_type& alloc = allocator_type()
        ) : alloc_(alloc) {
            throw std::runtime_error("Not implemented");
        }

        // TODO: Need to implement
        constexpr list(const list& other) {
            throw std::runtime_error("Not implemented");
        }

        // TODO: Need to implement
        constexpr list(list&& other) noexcept {
            throw std::runtime_error("Not implemented");
        }

        // TODO: Need to implement
        constexpr list(
            const list& other,
            const std::type_identity_t<allocator_type>& alloc
        ) : alloc_(alloc) {
            throw std::runtime_error("Not implemented");
        }

        // TODO: Need to implement
        constexpr list(
            list&& other,
            const std::type_identity_t<allocator_type>& alloc
        ) : alloc_(alloc) {
            throw std::runtime_error("Not implemented");
        }

        // TODO: Need to implement
        constexpr list(
            std::initializer_list<value_type> values,
            const allocator_type& alloc = allocator_type()
        ) : sz_(values.size()), alloc_(alloc) {
            throw std::runtime_error("Not implemented");
        }

        // ── Destructor ──────────────────────────────────────────────────────
        // TODO: Need to implement
        constexpr ~list() noexcept {
            if (this->head_ == nullptr) [[unlikely]] {
                return;
            }
            throw std::runtime_error("Not implemented");
        }
        
        // ── Overloaded Operators ────────────────────────────────────────────
        // TODO: Need to implement
        constexpr auto operator=(const list& rhs) -> list&;

        // TODO: Need to implement
        constexpr auto operator=(list&& rhs) noexcept -> list&;

        // TODO: Need to implement
        [[nodiscard]]
        constexpr auto operator==(const list& rhs) const -> bool;

        // TODO: Need to implement
        [[nodiscard]]
        constexpr auto operator<=>(const list& rhs) const;

        // ── Methods ─────────────────────────────────────────────────────────
        // TODO: Need to implement
        constexpr void assign(const size_type count, const_reference value);

        // TODO: Need to implement
        template<std::input_iterator InputIt>
        constexpr void assign(InputIt first, InputIt last);

        // TODO: Need to implement
        constexpr void assign(std::initializer_list<value_type> values);

        // TODO: Need to implement
        template<std::ranges::input_range R> requires(
            std::convertible_to<std::ranges::range_reference_t<R>, value_type>
        )
        constexpr void assign_range(R&& rg);

        // TODO: Need to implement
        [[nodiscard]]
        [[gnu::always_inline]]
        constexpr auto get_allocator() const noexcept -> allocator_type;

        // TODO: Need to implement
        constexpr auto front() -> reference;

        // TODO: Need to implement
        [[nodiscard]]
        constexpr auto front() const -> const_reference;

        // TODO: Need to implement
        constexpr auto back() -> reference;

        // TODO: Need to implement
        [[nodiscard]]
        constexpr auto back() const -> const_reference;

        // TODO: Need to implement
        constexpr auto begin() noexcept -> iterator;

        // TODO: Need to implement
        [[nodiscard]]
        constexpr auto begin() const noexcept -> const_iterator;

        // TODO: Need to implement
        [[nodiscard]]
        constexpr auto cbegin() const noexcept -> const_iterator;

        // TODO: Need to implement
        constexpr auto rbegin() noexcept -> reverse_iterator;

        // TODO: Need to implement
        [[nodiscard]]
        constexpr auto rbegin() const noexcept -> const_reverse_iterator;

        // TODO: Need to implement
        [[nodiscard]]
        constexpr auto crbegin() const noexcept -> const_reverse_iterator;

        // TODO: Need to implement
        constexpr auto end() noexcept -> iterator;

        // TODO: Need to implement
        [[nodiscard]]
        constexpr auto end() const noexcept -> const_iterator;

        // TODO: Need to implement
        [[nodiscard]]
        constexpr auto cend() const noexcept -> const_iterator;

        // TODO: Need to implement
        constexpr auto rend() noexcept -> reverse_iterator;

        // TODO: Need to implement
        [[nodiscard]]
        constexpr auto rend() const -> const_reverse_iterator;

        // TODO: Need to implement
        [[nodiscard]]
        constexpr auto crend() const noexcept -> const_reverse_iterator;

        // TODO: Need to implement
        [[nodiscard]]
        constexpr auto empty() const noexcept -> bool;

        // TODO: Need to implement
        [[nodiscard]]
        constexpr auto max_size() const noexcept -> size_type;

        // TODO: Need to implement
        constexpr void clear() noexcept;

        // TODO: Need to implement
        constexpr auto insert(
            const_iterator pos,
            const_reference value
        ) -> iterator;

        // TODO: Need to implement
        constexpr auto insert(
            const_iterator pos,
            value_type&& value
        ) -> iterator;

        // TODO: Need to implement
        constexpr auto insert(
            const_iterator pos,
            const size_type count,
            const_reference value
        ) -> iterator;

        // TODO: Need to implement
        template<std::input_iterator InputIt>
        constexpr auto insert(
            const_iterator pos,
            InputIt first,
            InputIt last
        ) -> iterator;

        // TODO: Need to implement
        constexpr auto insert(
            const_iterator pos,
            std::initializer_list<value_type> values
        ) -> iterator;

        // TODO: Need to implement
        template<typename... Args>
        constexpr auto emplace(
            const_iterator pos,
            Args&&... args
        ) -> iterator;

        // TODO: Need to implement
        template<std::ranges::input_range R> requires(
            std::convertible_to<std::ranges::range_reference_t<R>, value_type>
        )
        constexpr auto insert_range(
            const_iterator pos,
            R&& rg
        ) -> iterator;

        // TODO: Need to implement
        constexpr auto erase(const_iterator pos) -> iterator;

        // TODO: Need to implement
        constexpr auto erase(
            const_iterator first,
            const_iterator last
        ) -> iterator;

        // TODO: Need to implement
        constexpr void push_back(const_reference value);

        // TODO: Need to implement
        constexpr void push_back(value_type&& value);

        // TODO: Need to implement
        template<typename... Args>
        constexpr auto emplace_back(Args&&... args) -> reference;

        // TODO: Need to implement
        template<std::ranges::input_range R> requires(
            std::convertible_to<std::ranges::range_reference_t<R>, value_type>
        )
        constexpr void append_range(R&& rg);

        // TODO: Need to implement
        constexpr void pop_back();

        // TODO: Need to implement
        constexpr void push_front(const_reference value);

        // TODO: Need to implement
        constexpr void push_front(value_type&& value);

        // TODO: Need to implement
        template<typename... Args >
        constexpr auto emplace_front(Args&&... args) -> reference;

        // TODO: Need to implement
        template<std::ranges::input_range R> requires(
            std::convertible_to<std::ranges::range_reference_t<R>, value_type>
        )
        constexpr void prepend_range(R&& rg);

        // TODO: Need to implement
        constexpr void pop_front();
        
        // TODO: Need to implement
        constexpr void resize(const size_type count);

        // TODO: Need to implement
        constexpr void resize(const size_type count, const_reference value);

        // TODO: Need to implement
        constexpr void swap(list& other) noexcept(
            std::allocator_traits<Allocator>::is_always_equal::value
        );

        // TODO: Need to implement
        constexpr void merge(list& other);

        // TODO: Need to implement
        constexpr void merge(list&& other);

        // TODO: Need to implement
        template<typename Compare> 
        constexpr void merge(list& other, const Compare comp);

        // TODO: Need to implement
        template<typename Compare> 
        constexpr void merge(list&& other, const Compare comp);

        // TODO: Need to implement
        constexpr void splice(const_iterator pos, list& other);

        // TODO: Need to implement
        constexpr void splice(const_iterator pos, list&& other);

        // TODO: Need to implement
        constexpr void splice(
            const_iterator pos,
            list& other,
            const_iterator it
        );

        // TODO: Need to implement
        constexpr void splice(
            const_iterator pos,
            list&& other,
            const_iterator it
        );

        // TODO: Need to implement
        constexpr void splice(
            const_iterator pos,
            list& other,
            const_iterator first,
            const_iterator last
        );

        // TODO: Need to implement
        constexpr void splice(
            const_iterator pos,
            list&& other,
            const_iterator first,
            const_iterator last
        );

        // TODO: Need to implement
        constexpr auto remove(const_reference value) -> size_type;

        // TODO: Need to implement
        template<typename UnaryPredicate>
        constexpr auto remove_if(const UnaryPredicate pred) -> size_type;

        // TODO: Need to implement
        constexpr void reverse() noexcept;
        
        // TODO: Need to implement
        constexpr auto unique() -> size_type;

        // TODO: Need to implement
        template<typename BinaryPredicate>
        constexpr auto unique(const BinaryPredicate pred) -> size_type;

        // TODO: Need to implement
        constexpr void sort();

        // TODO: Need to implement
        template<typename Compare>
        constexpr void sort(const Compare comp);
    };

    // ── Deduction Guides ────────────────────────────────────────────────────
    // TODO: Need to test
    template<
        std::input_iterator InputIt,
        typename Alloc = std::allocator<
              typename std::iterator_traits<InputIt>::value_type
        >
    >
    list(InputIt, InputIt, Alloc = Alloc()) -> list<
        typename std::iterator_traits<InputIt>::value_type, Alloc
    >;

    // TODO: Need to test
    template<
        std::ranges::input_range R,
        typename Alloc = std::allocator<std::ranges::range_value_t<R>>
    >
    list(
        std::from_range_t,
        R&&,
        Alloc = Alloc()
    ) -> list<std::ranges::range_value_t<R>, Alloc>;

    // ── Functions ───────────────────────────────────────────────────────────
    // TODO: Need to implement
    template<typename T, typename Alloc>
    constexpr void swap(list<T, Alloc>& lhs, list<T, Alloc>& rhs) noexcept(
        noexcept(lhs.swap(rhs))
    );

    // TODO: Need to implement
    template<typename T, typename Alloc, typename U = T>
    constexpr list<T, Alloc>::size_type erase(
        list<T, Alloc>& list, const U& value
    );

    // TODO: Need to implement
    template<typename T, typename Alloc, typename Pred>
    constexpr list<T, Alloc>::size_type erase_if(
        list<T, Alloc>& list,
        const Pred pred
    );
} // namespace collections

#endif // #ifndef COLLECTIONS_LIST_HPP
