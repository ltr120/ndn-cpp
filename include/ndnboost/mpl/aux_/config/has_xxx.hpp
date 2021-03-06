
#ifndef NDNBOOST_MPL_AUX_CONFIG_HAS_XXX_HPP_INCLUDED
#define NDNBOOST_MPL_AUX_CONFIG_HAS_XXX_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2002-2004
// Copyright David Abrahams 2002-2003
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id: has_xxx.hpp 63518 2010-07-02 08:32:03Z agurtovoy $
// $Date: 2010-07-02 01:32:03 -0700 (Fri, 02 Jul 2010) $
// $Revision: 63518 $

#include <ndnboost/mpl/aux_/config/overload_resolution.hpp>
#include <ndnboost/mpl/aux_/config/workaround.hpp>

// agurt, 11/jan/03: signals a stub-only 'has_xxx' implementation

#if !defined(NDNBOOST_MPL_CFG_NO_HAS_XXX) \
    && (   defined(NDNBOOST_MPL_CFG_BROKEN_OVERLOAD_RESOLUTION) \
        || NDNBOOST_WORKAROUND(__GNUC__, <= 2) \
        || NDNBOOST_WORKAROUND(__DMC__, NDNBOOST_TESTED_AT(0x840)) \
        )

#   define NDNBOOST_MPL_CFG_NO_HAS_XXX
#   define NDNBOOST_MPL_CFG_NO_HAS_XXX_TEMPLATE

#endif

#endif // NDNBOOST_MPL_AUX_CONFIG_HAS_XXX_HPP_INCLUDED
