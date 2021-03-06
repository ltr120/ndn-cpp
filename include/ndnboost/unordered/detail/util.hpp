
// Copyright (C) 2003-2004 Jeremy B. Maitin-Shepard.
// Copyright (C) 2005-2011 Daniel James
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef NDNBOOST_UNORDERED_DETAIL_UTIL_HPP_INCLUDED
#define NDNBOOST_UNORDERED_DETAIL_UTIL_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <ndnboost/type_traits/is_convertible.hpp>
#include <ndnboost/type_traits/is_empty.hpp>
#include <ndnboost/iterator/iterator_categories.hpp>
#include <ndnboost/utility/enable_if.hpp>
#include <ndnboost/detail/select_type.hpp>
#include <ndnboost/move/move.hpp>
#include <ndnboost/preprocessor/seq/size.hpp>
#include <ndnboost/preprocessor/seq/enum.hpp>
#include <ndnboost/swap.hpp>

namespace ndnboost { namespace unordered { namespace detail {

    static const float minimum_max_load_factor = 1e-3f;
    static const std::size_t default_bucket_count = 11;
    struct move_tag {};
    struct empty_emplace {};

    ////////////////////////////////////////////////////////////////////////////
    // iterator SFINAE

    template <typename I>
    struct is_forward :
        ndnboost::is_convertible<
            typename ndnboost::iterator_traversal<I>::type,
            ndnboost::forward_traversal_tag>
    {};

    template <typename I, typename ReturnType>
    struct enable_if_forward :
        ndnboost::enable_if_c<
            ndnboost::unordered::detail::is_forward<I>::value,
            ReturnType>
    {};

    template <typename I, typename ReturnType>
    struct disable_if_forward :
        ndnboost::disable_if_c<
            ndnboost::unordered::detail::is_forward<I>::value,
            ReturnType>
    {};

    ////////////////////////////////////////////////////////////////////////////
    // primes

#define NDNBOOST_UNORDERED_PRIMES \
    (17ul)(29ul)(37ul)(53ul)(67ul)(79ul) \
    (97ul)(131ul)(193ul)(257ul)(389ul)(521ul)(769ul) \
    (1031ul)(1543ul)(2053ul)(3079ul)(6151ul)(12289ul)(24593ul) \
    (49157ul)(98317ul)(196613ul)(393241ul)(786433ul) \
    (1572869ul)(3145739ul)(6291469ul)(12582917ul)(25165843ul) \
    (50331653ul)(100663319ul)(201326611ul)(402653189ul)(805306457ul) \
    (1610612741ul)(3221225473ul)(4294967291ul)

    template<class T> struct prime_list_template
    {
        static std::size_t const value[];

#if !defined(SUNPRO_CC)
        static std::ptrdiff_t const length;
#else
        static std::ptrdiff_t const length
            = NDNBOOST_PP_SEQ_SIZE(NDNBOOST_UNORDERED_PRIMES);
#endif
    };

    template<class T>
    std::size_t const prime_list_template<T>::value[] = {
        NDNBOOST_PP_SEQ_ENUM(NDNBOOST_UNORDERED_PRIMES)
    };

#if !defined(SUNPRO_CC)
    template<class T>
    std::ptrdiff_t const prime_list_template<T>::length
        = NDNBOOST_PP_SEQ_SIZE(NDNBOOST_UNORDERED_PRIMES);
#endif

#undef NDNBOOST_UNORDERED_PRIMES

    typedef prime_list_template<std::size_t> prime_list;

    // no throw
    inline std::size_t next_prime(std::size_t num) {
        std::size_t const* const prime_list_begin = prime_list::value;
        std::size_t const* const prime_list_end = prime_list_begin +
            prime_list::length;
        std::size_t const* bound =
            std::lower_bound(prime_list_begin, prime_list_end, num);
        if(bound == prime_list_end)
            bound--;
        return *bound;
    }

    // no throw
    inline std::size_t prev_prime(std::size_t num) {
        std::size_t const* const prime_list_begin = prime_list::value;
        std::size_t const* const prime_list_end = prime_list_begin +
            prime_list::length;
        std::size_t const* bound =
            std::upper_bound(prime_list_begin,prime_list_end, num);
        if(bound != prime_list_begin)
            bound--;
        return *bound;
    }

    ////////////////////////////////////////////////////////////////////////////
    // insert_size/initial_size

#if !defined(NDNBOOST_NO_STD_DISTANCE)

    using ::std::distance;

#else

    template <class ForwardIterator>
    inline std::size_t distance(ForwardIterator i, ForwardIterator j) {
        std::size_t x;
        std::distance(i, j, x);
        return x;
    }

#endif

    template <class I>
    inline typename
        ndnboost::unordered::detail::enable_if_forward<I, std::size_t>::type
        insert_size(I i, I j)
    {
        return std::distance(i, j);
    }

    template <class I>
    inline typename
        ndnboost::unordered::detail::disable_if_forward<I, std::size_t>::type
        insert_size(I, I)
    {
        return 1;
    }

    template <class I>
    inline std::size_t initial_size(I i, I j,
        std::size_t num_buckets =
            ndnboost::unordered::detail::default_bucket_count)
    {
        // TODO: Why +1?
        return (std::max)(
            ndnboost::unordered::detail::insert_size(i, j) + 1,
            num_buckets);
    }

    ////////////////////////////////////////////////////////////////////////////
    // compressed

    template <typename T, int Index>
    struct compressed_base : private T
    {
        compressed_base(T const& x) : T(x) {}
        compressed_base(T& x, move_tag) : T(ndnboost::move(x)) {}

        T& get() { return *this; }
        T const& get() const { return *this; }
    };
    
    template <typename T, int Index>
    struct uncompressed_base
    {
        uncompressed_base(T const& x) : value_(x) {}
        uncompressed_base(T& x, move_tag) : value_(ndnboost::move(x)) {}

        T& get() { return value_; }
        T const& get() const { return value_; }
    private:
        T value_;
    };
    
    template <typename T, int Index>
    struct generate_base
      : ndnboost::detail::if_true<
            ndnboost::is_empty<T>::value
        >:: NDNBOOST_NESTED_TEMPLATE then<
            ndnboost::unordered::detail::compressed_base<T, Index>,
            ndnboost::unordered::detail::uncompressed_base<T, Index>
        >
    {};
    
    template <typename T1, typename T2>
    struct compressed
      : private ndnboost::unordered::detail::generate_base<T1, 1>::type,
        private ndnboost::unordered::detail::generate_base<T2, 2>::type
    {
        typedef typename generate_base<T1, 1>::type base1;
        typedef typename generate_base<T2, 2>::type base2;

        typedef T1 first_type;
        typedef T2 second_type;
        
        first_type& first() {
            return static_cast<base1*>(this)->get();
        }

        first_type const& first() const {
            return static_cast<base1 const*>(this)->get();
        }

        second_type& second() {
            return static_cast<base2*>(this)->get();
        }

        second_type const& second() const {
            return static_cast<base2 const*>(this)->get();
        }

        template <typename First, typename Second>
        compressed(First const& x1, Second const& x2)
            : base1(x1), base2(x2) {}

        compressed(compressed const& x)
            : base1(x.first()), base2(x.second()) {}

        compressed(compressed& x, move_tag m)
            : base1(x.first(), m), base2(x.second(), m) {}

        void assign(compressed const& x)
        {
            first() = x.first();
            second() = x.second();
        }

        void move_assign(compressed& x)
        {
            first() = ndnboost::move(x.first());
            second() = ndnboost::move(x.second());
        }
        
        void swap(compressed& x)
        {
            ndnboost::swap(first(), x.first());
            ndnboost::swap(second(), x.second());
        }

    private:
        // Prevent assignment just to make use of assign or
        // move_assign explicit.
        compressed& operator=(compressed const&);
    };
}}}

#endif
