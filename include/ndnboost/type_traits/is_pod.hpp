
//  (C) Copyright Steve Cleary, Beman Dawes, Howard Hinnant & John Maddock 2000.
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.

#ifndef NDNBOOST_TT_IS_POD_HPP_INCLUDED
#define NDNBOOST_TT_IS_POD_HPP_INCLUDED

#include <ndnboost/type_traits/config.hpp>
#include <ndnboost/type_traits/is_void.hpp>
#include <ndnboost/type_traits/is_scalar.hpp>
#include <ndnboost/type_traits/detail/ice_or.hpp>
#include <ndnboost/type_traits/intrinsics.hpp>

#include <cstddef>

// should be the last #include
#include <ndnboost/type_traits/detail/bool_trait_def.hpp>

#ifndef NDNBOOST_IS_POD
#define NDNBOOST_INTERNAL_IS_POD(T) false
#else
#define NDNBOOST_INTERNAL_IS_POD(T) NDNBOOST_IS_POD(T)
#endif

namespace ndnboost {

// forward declaration, needed by 'is_pod_array_helper' template below
template< typename T > struct is_POD;

namespace detail {

#ifndef NDNBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

template <typename T> struct is_pod_impl
{ 
    NDNBOOST_STATIC_CONSTANT(
        bool, value =
        (::ndnboost::type_traits::ice_or<
            ::ndnboost::is_scalar<T>::value,
            ::ndnboost::is_void<T>::value,
            NDNBOOST_INTERNAL_IS_POD(T)
         >::value));
};

#if !defined(NDNBOOST_NO_ARRAY_TYPE_SPECIALIZATIONS)
template <typename T, std::size_t sz>
struct is_pod_impl<T[sz]>
    : public is_pod_impl<T>
{
};
#endif

#else

template <bool is_array = false>
struct is_pod_helper
{
    template <typename T> struct result_
    {
        NDNBOOST_STATIC_CONSTANT(
            bool, value =
            (::ndnboost::type_traits::ice_or<
                ::ndnboost::is_scalar<T>::value,
                ::ndnboost::is_void<T>::value,
                NDNBOOST_INTERNAL_IS_POD(T)
            >::value));
    };
};

template <bool b>
struct bool_to_yes_no_type
{
    typedef ::ndnboost::type_traits::no_type type;
};

template <>
struct bool_to_yes_no_type<true>
{
    typedef ::ndnboost::type_traits::yes_type type;
};

template <typename ArrayType>
struct is_pod_array_helper
{
    enum { is_pod = ::ndnboost::is_POD<ArrayType>::value }; // MSVC workaround
    typedef typename bool_to_yes_no_type<is_pod>::type type;
    type instance() const;
};

template <typename T>
is_pod_array_helper<T> is_POD_array(T*);

template <>
struct is_pod_helper<true>
{
    template <typename T> struct result_
    {
        static T& help();
        NDNBOOST_STATIC_CONSTANT(bool, value =
            sizeof(is_POD_array(help()).instance()) == sizeof(::ndnboost::type_traits::yes_type)
            );
    };
};


template <typename T> struct is_pod_impl
{ 
   NDNBOOST_STATIC_CONSTANT(
       bool, value = (
           ::ndnboost::detail::is_pod_helper<
              ::ndnboost::is_array<T>::value
           >::template result_<T>::value
           )
       );
};

#endif // NDNBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

// the following help compilers without partial specialization support:
NDNBOOST_TT_AUX_BOOL_TRAIT_IMPL_SPEC1(is_pod,void,true)

#ifndef NDNBOOST_NO_CV_VOID_SPECIALIZATIONS
NDNBOOST_TT_AUX_BOOL_TRAIT_IMPL_SPEC1(is_pod,void const,true)
NDNBOOST_TT_AUX_BOOL_TRAIT_IMPL_SPEC1(is_pod,void volatile,true)
NDNBOOST_TT_AUX_BOOL_TRAIT_IMPL_SPEC1(is_pod,void const volatile,true)
#endif

} // namespace detail

NDNBOOST_TT_AUX_BOOL_TRAIT_DEF1(is_pod,T,::ndnboost::detail::is_pod_impl<T>::value)
// is_POD is the old depricated name for this trait, do not use this as it may
// be removed in future without warning!!
NDNBOOST_TT_AUX_BOOL_TRAIT_DEF1(is_POD,T,::ndnboost::is_pod<T>::value)

} // namespace ndnboost

#include <ndnboost/type_traits/detail/bool_trait_undef.hpp>

#undef NDNBOOST_INTERNAL_IS_POD

#endif // NDNBOOST_TT_IS_POD_HPP_INCLUDED
