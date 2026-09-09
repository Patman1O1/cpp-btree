#ifndef COLLECTIONS_LIST_HPP
#define COLLECTIONS_LIST_HPP

// ISO C Includes
#include <cstddef>

// ISO C++ Includes
#include <concepts>
#include <ranges>
#include <iterator>
#include <memory>
#include <optional>
#include <stdexcept>
#include <type_traits>
#include <utility>

namespace collections {
    template<
        typename T,
        typename Compare = std::less<T>,
        typename Allocator = std::allocator<T>
    >
    class btree {
    public:
        // ── Forward Declarations ────────────────────────────────────────────
        class iterator;

        class const_iterator;

        // ── Aliases ─────────────────────────────────────────────────────────
        using value_type = T;

        using allocator_type = Allocator;

        using size_type = std::size_t;

        using difference_type = std::ptrdiff_t;

        using compare = Compare;

        using ator_traits = std::allocator_traits<allocator_type>;

        using pointer = ator_traits::pointer;

        using const_pointer = ator_traits::const_pointer;

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
        using node_allocator_type = ator_traits::template rebind_alloc<
            struct node
        >;

        using node_ator_traits = std::allocator_traits<node_allocator_type>;

        // ── Fields ──────────────────────────────────────────────────────────
        struct node* root_;

        size_type sz_;

        [[no_unique_address]]
        allocator_type alloc_;

    public:
        // ── node_type ───────────────────────────────────────────────────────
        class node_type {
        public:
            // ── Aliases ─────────────────────────────────────────────────────
            using value_type = btree::value_type;

            using allocator_type = btree::node_allocator_type;

            using container_node_type = struct btree::node;

            using ator_traits = std::allocator_traits<allocator_type>;

            using reference = value_type&;

            using pointer = typename ator_traits::template rebind_traits<
                container_node_type
            >::pointer;
        
        private:
            // ── Friends ─────────────────────────────────────────────────────
            friend class btree;

            // ── Fields ──────────────────────────────────────────────────────
            pointer ptr_;

            std::optional<allocator_type> alloc_;
        
        public:
            // ── Constructors ────────────────────────────────────────────────
            // TODO: Need to test
            constexpr node_type() noexcept : ptr_(nullptr),
                                             alloc_(std::nullopt) {}

            constexpr node_type(const node_type&) noexcept = delete;

            // TODO: Need to test
            constexpr node_type(
                node_type&& other
            ) noexcept : ptr_(std::move(other.ptr_)), 
                         alloc_(std::move(other.alloc_)) {
                other.ptr_ = nullptr;
                other.alloc_ = std::nullopt;
            }

            // ── Destructor ──────────────────────────────────────────────────
            // TODO: Need to test
            constexpr ~node_type() noexcept {
                if (this->ptr_ == nullptr) [[unlikely]] {
                    return;
                }

                ator_traits::destroy(this->alloc_.value(), this->ptr_);
                ator_traits::deallocate(this->alloc_.value(), this->ptr_);
                this->ptr_ = nullptr;
            }

            // ── Overloaded Operators ────────────────────────────────────────
            constexpr auto operator=(
                const node_type&
            ) noexcept -> node_type& = delete;
            
            // TODO: Need to test
            constexpr auto operator=(
                node_type&& rhs
            ) noexcept -> node_type& {
                this->~node_type();

                this->ptr_ = std::move(rhs.ptr_);
                rhs.ptr_ = nullptr;

                this->alloc_ = std::move(rhs.alloc_);
                rhs.alloc_ = std::nullopt;

                return *this;
            }

            // TODO: Need to test
            [[nodiscard]]
            constexpr auto operator==(
                const node_type& rhs
            ) const noexcept -> bool { return this->ptr_ == rhs.ptr_; }

            // TODO: Need to test
            [[nodiscard]]
            constexpr auto operator!=(
                const node_type& rhs
            ) const noexcept -> bool { return this->ptr_ != rhs.ptr_; }

            // TODO: Need to test
            [[nodiscard]]
            constexpr auto operator*() const noexcept -> reference {
                return *this->ptr_;
            }

            // TODO: Need to test
            [[nodiscard]]
            explicit constexpr operator bool() const noexcept {
                return this->ptr_ != nullptr;
            }

            // ── Methods ─────────────────────────────────────────────────────
            // TODO: Need to test
            [[nodiscard]]
            constexpr auto empty() const noexcept -> bool {
                return this->ptr_ == nullptr;
            }

            // TODO: Need to test
            [[nodiscard]]
            constexpr auto value() const -> std::optional<reference> {
                return this->ptr_ != nullptr ? *this->ptr_ : std::nullopt;
            }

            // TODO: Need to test
            constexpr void swap(node_type&& other) noexcept(
                ator_traits::propagate_on_container_swap::value ||
                ator_traits::is_always_equal::value
            ) {
                pointer tmp = this->ptr_;
                this->ptr_ = other.ptr_;
                other.ptr_ = tmp;
            }
        };

        // ── insert_return_type ──────────────────────────────────────────────
        template<typename Iterator = iterator, typename NodeType = node_type>
        struct insert_return_type {
            // ── Fields ──────────────────────────────────────────────────────
            Iterator position;

            bool inserted;

            NodeType node;
        };

        // ── iterator ────────────────────────────────────────────────────────
        class iterator {
        public:
            // ── Aliases ─────────────────────────────────────────────────────
            using iterator_category = std::bidirectional_iterator_tag;

            using iterator_concept = std::bidirectional_iterator_tag;

            using value_type = btree::value_type;

            using size_type = btree::size_type;

            using difference_type = btree::difference_type;
            
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

            using value_type = btree::value_type;

            using size_type = btree::size_type;

            using difference_type = btree::difference_type;
            
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
        constexpr btree(
            std::from_range_t,
            R&& rg,
            const allocator_type& alloc = allocator_type()
        ) : alloc_(alloc) {
            throw std::runtime_error("Not implemented");
        }

        // TODO: Need to implement
        constexpr btree(const btree& other);

        // TODO: Need to implement
        constexpr btree(btree&& other) noexcept;

        // TODO: Need to implement
        constexpr btree(
            const btree& other,
            const std::type_identity_t<allocator_type>& alloc
        ) : alloc_(alloc) {
            throw std::runtime_error("Not implemented");
        }

        // TODO: Need to implement
        constexpr btree(
            btree&& other,
            const std::type_identity_t<allocator_type>& alloc
        ) : alloc_(alloc) {
            throw std::runtime_error("Not implemented");
        }

        // TODO: Need to implement
        constexpr btree(
            std::initializer_list<value_type> values,
            const allocator_type& alloc = allocator_type()
        ) : sz_(values.size()), alloc_(alloc) {
            throw std::runtime_error("Not implemented");
        }

        // ── Destructor ──────────────────────────────────────────────────────
        // TODO: Need to implement
        constexpr ~btree() noexcept {
            if (this->root_ == nullptr) [[unlikely]] {
                return;
            }
            throw std::runtime_error("Not implemented");
        }
        
        // ── Overloaded Operators ────────────────────────────────────────────
        // TODO: Need to implement
        constexpr auto operator=(const btree& rhs) -> btree&;

        // TODO: Need to implement
        constexpr auto operator=(btree&& rhs) noexcept -> btree&;

        // TODO: Need to implement
        [[nodiscard]]
        constexpr auto operator==(const btree& rhs) const -> bool;

        // TODO: Need to implement
        [[nodiscard]]
        constexpr auto operator<=>(const btree& rhs) const;

        // ── Methods ───────────────────────────────────────────────────────── 
        // TODO: Need to implement
        [[nodiscard]]
        [[gnu::always_inline]]
        constexpr auto get_allocator() const noexcept -> allocator_type;
 
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
        constexpr auto size() const noexcept -> size_type;

        // TODO: Need to implement
        [[nodiscard]]
        constexpr auto max_size() const noexcept -> size_type;

        // TODO: Need to implement
        constexpr void clear() noexcept;

        // TODO: Need to implement
        constexpr auto insert(
            const_reference value
        ) -> std::pair<iterator, bool>;

        // TODO: Need to implement
        constexpr auto insert(value_type&& value) -> std::pair<iterator, bool>;

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
        template<std::input_iterator InputIt>
        constexpr void insert(InputIt first, InputIt last);

        // TODO: Need to implement
        constexpr void insert(std::initializer_list<value_type> values);

        // TODO: Need to implement
        constexpr auto insert(
            node_type&& node_handle
        ) -> struct insert_return_type<iterator, node_type>;

        // TODO: Need to implement
        constexpr auto insert(
            const_iterator pos,
            node_type&& node_handle
        ) -> iterator;

        // TODO: Need to implement
        template<std::ranges::input_range R> requires(
            std::convertible_to<std::ranges::range_reference_t<R>, value_type>
        )
        constexpr void insert_range(R&& rg);

        template<typename... Args>
        constexpr auto emplace(Args&&... args) -> std::pair<iterator, bool>;
       
        // TODO: Need to implement
        template<typename... Args>
        constexpr auto emplace(
            const_iterator pos,
            Args&&... args
        ) -> iterator;

        // TODO: Need to implement
        template<typename... Args>
        constexpr auto emplace_hint(
            const_iterator hint,
            Args&&... args
        ) -> iterator;

        // TODO: Need to implement
        constexpr auto erase(iterator pos) -> iterator requires(
            !std::same_as<iterator, const_iterator>
        );

        // TODO: Need to implement
        constexpr auto erase(const_iterator pos) -> iterator;

        // TODO: Need to implement
        constexpr auto erase(
            const_iterator first,
            const_iterator last
        ) -> iterator;
        
        // TODO: Need to implement
        constexpr void swap(btree& other) noexcept(
            std::allocator_traits<Allocator>::is_always_equal::value
        );

        // TODO: Need to implement
        constexpr auto extract(const_iterator position) -> node_type;

        // TODO: Need to implement
        constexpr void merge(btree& other);

        // TODO: Need to implement
        constexpr void merge(btree&& other);

        // TODO: Need to implement
        template<typename Comp> 
        constexpr void merge(btree& other, const Comp comp);

        // TODO: Need to implement
        template<typename Comp> 
        constexpr void merge(btree&& other, const Comp comp);

        // TODO: Need to implement
        [[nodiscard]]
        constexpr auto count(const_reference value) const -> size_type;

        // TODO: Need to implement
        constexpr auto equal_range(
            const_reference value
        ) -> std::pair<iterator, iterator>;


        // TODO: Need to implement
        [[nodiscard]]
        constexpr auto equal_range(
            const_reference value
        ) const -> std::pair<const_iterator, const_iterator>;

        // TODO: Need to implement
        constexpr auto lower_bound(const_reference value) -> iterator;

        // TODO: Need to implement
        [[nodiscard]]
        constexpr auto lower_bound(
            const_reference value
        ) const -> const_iterator;

        // TODO: Need to implement
        constexpr auto upper_bound(const_reference value) -> iterator;
        
        // TODO: Need to implement
        [[nodiscard]]
        constexpr auto upper_bound(
            const_reference value
        ) const -> const_iterator;

        // TODO: Need to implement
        [[nodiscard]]
        constexpr auto comp() const -> compare;

    };

    // ── Deduction Guides ────────────────────────────────────────────────────
    // TODO: Need to test
    template<
        std::input_iterator InputIt,
        typename Comp = std::less<
            typename std::iterator_traits<InputIt>::value_type
        >,
        typename Alloc = std::allocator<
            typename std::iterator_traits<InputIt>::value_type
        >
    >
    btree(
        InputIt,
        InputIt,
        Comp = Comp(),
        Alloc = Alloc()
    ) -> btree<
        typename std::iterator_traits<InputIt>::value_type, Comp, Alloc
    >;

    // ── Functions ───────────────────────────────────────────────────────────
    // TODO: Need to implement
    template<typename T, typename Comp, typename Alloc>
    constexpr void swap(
        btree<T, Comp, Alloc>& lhs,
        btree<T, Comp, Alloc>& rhs
    ) noexcept(noexcept(lhs.swap(rhs)));

    // TODO: Need to implement
    template<typename T, typename Alloc, typename U = T>
    constexpr btree<T, Alloc>::size_type erase(
        btree<T, Alloc>& list, const U& value
    );

    // TODO: Need to implement
    template<typename T, typename Alloc, typename Pred>
    constexpr btree<T, Alloc>::size_type erase_if(
        btree<T, Alloc>& list,
        const Pred pred
    );
} // namespace collections

#endif // #ifndef COLLECTIONS_LIST_HPP
