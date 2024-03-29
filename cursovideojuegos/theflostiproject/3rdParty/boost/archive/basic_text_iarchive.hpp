#ifndef BOOST_ARCHIVE_BASIC_TEXT_IARCHIVE_HPP
#define BOOST_ARCHIVE_BASIC_TEXT_IARCHIVE_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// basic_text_iarchive.hpp

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com . 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

// archives stored as text - note these ar templated on the basic
// stream templates to accommodate wide (and other?) kind of characters
//
// note the fact that on libraries without wide characters, ostream is
// is not a specialization of basic_ostream which in fact is not defined
// in such cases.   So we can't use basic_ostream<IStream::char_type> but rather
// use two template parameters

#include <boost/config.hpp>
#include <boost/pfto.hpp>
#include <boost/detail/workaround.hpp>

#include <boost/archive/detail/interface_iarchive.hpp>
#include <boost/archive/detail/common_iarchive.hpp>

#include <boost/serialization/string.hpp>

namespace boost {
namespace archive {

/////////////////////////////////////////////////////////////////////////
// class basic_text_iarchive - read serialized objects from a input text stream
template<class Archive>
class basic_text_iarchive : public detail::common_iarchive<Archive>
{
#if BOOST_WORKAROUND(BOOST_MSVC, <= 1300)
public:
#elif defined(BOOST_MSVC)
    // for some inexplicable reason insertion of "class" generates compile erro
    // on msvc 7.1
    friend detail::interface_iarchive<Archive>;
protected:
#else
    friend class detail::interface_iarchive<Archive>;
protected:
#endif
    // intermediate level to support override of operators
    // fot templates in the absence of partial function 
    // template ordering
    template<class T>
    void load_override(T & t, BOOST_PFTO int){
        archive::load(* this->This(), t);
    }
    // text file don't include the optional information 
    void load_override(class_id_optional_type & /*t*/, int){}
    void load_override(class_name_type & t, int);
    basic_text_iarchive() : 
        detail::common_iarchive<Archive>()
    {}
};

} // namespace archive
} // namespace boost

#endif // BOOST_ARCHIVE_BASIC_TEXT_IARCHIVE_HPP
