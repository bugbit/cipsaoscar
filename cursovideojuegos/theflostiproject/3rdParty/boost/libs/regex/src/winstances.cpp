/*
 *
 * Copyright (c) 1998-2002
 * Dr John Maddock
 *
 * Use, modification and distribution are subject to the 
 * Boost Software License, Version 1.0. (See accompanying file 
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 */

 /*
  *   LOCATION:    see http://www.boost.org for most recent version.
  *   FILE:        winstances.cpp
  *   VERSION:     see <boost/version.hpp>
  *   DESCRIPTION: regex wide character template instances.
  */

#define BOOST_REGEX_SOURCE

#include <boost/regex/config.hpp>

#if !defined(BOOST_NO_WREGEX) && !defined(BOOST_REGEX_NO_EXTERNAL_TEMPLATES)
#define BOOST_REGEX_WIDE_INSTANTIATE

#ifdef __BORLANDC__
#pragma hrdstop
#endif

#include <boost/regex.hpp>

#endif

