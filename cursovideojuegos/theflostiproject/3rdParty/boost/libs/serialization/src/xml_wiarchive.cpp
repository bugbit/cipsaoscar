/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// xml_wiarchive.cpp:

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com . 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#include <boost/config.hpp>
#ifdef BOOST_NO_STD_WSTREAMBUF
#error "wide char i/o not supported on this platform"
#else

#include <boost/detail/workaround.hpp>

#if (defined _MSC_VER) && (_MSC_VER == 1200)
#  pragma warning (disable : 4786) // too long name, harmless warning
#endif

// the following works around an issue between spirit 1.61 and borland.
// it turns out the the certain spirit stuff must be defined before
// certain parts of mpl.  including this here makes sure that happens
#if BOOST_WORKAROUND(__BORLANDC__, <= 0x564 )
#include <boost/archive/impl/basic_xml_grammar.hpp>
#endif

#include <boost/archive/xml_wiarchive.hpp>

// explicitly instantiate for this type of xml stream
#include <boost/archive/impl/basic_xml_iarchive.ipp>
#include <boost/archive/impl/xml_wiarchive_impl.ipp>
#include <boost/archive/impl/archive_pointer_iserializer.ipp>

namespace boost {
namespace archive {

template class basic_xml_iarchive<xml_wiarchive> ;
template class xml_wiarchive_impl<xml_wiarchive> ;
template class detail::archive_pointer_iserializer<xml_wiarchive> ;

} // namespace archive
} // namespace boost

#endif // BOOST_NO_STD_WSTREAMBUF
