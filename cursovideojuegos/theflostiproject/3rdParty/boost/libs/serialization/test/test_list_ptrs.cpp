/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// test_list.cpp

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com . 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// should pass compilation and execution

#include <fstream>

#include <cstdio> // remove
#include <boost/config.hpp>
#if defined(BOOST_NO_STDC_NAMESPACE)
namespace std{ 
    using ::remove;
}
#endif

#include <boost/type_traits/is_pointer.hpp>
#include <boost/static_assert.hpp>
#include <boost/checked_delete.hpp>

#include <boost/serialization/list.hpp>
#ifdef BOOST_HAS_SLIST
#include <boost/serialization/slist.hpp>
#endif
#include <boost/serialization/nvp.hpp>

#include <boost/archive/archive_exception.hpp>
#include "test_tools.hpp"

#include "A.hpp"

template<class T>
struct ptr_equal_to : public std::binary_function<T, T, bool> 
{
    BOOST_STATIC_ASSERT(::boost::is_pointer<T>::value);
    bool operator()(T const _Left, T const _Right) const
    {
        if(NULL == _Left && NULL == _Right)
            return true;
        if(typeid(*_Left) != typeid(*_Right))
            return false;
        return *_Left == *_Right;
    }
};

int test_main( int /* argc */, char* /* argv */[] )
{
    const char * testfile = boost::archive::tmpnam(NULL);
    BOOST_REQUIRE(NULL != testfile);

    std::list<A *> alist;
    alist.push_back(new A);
    alist.push_back(new A);
    {   
        test_ostream os(testfile, TEST_STREAM_FLAGS);
        test_oarchive oa(os);
        alist.push_back(new A);
        alist.push_back(new A);
        oa << boost::serialization::make_nvp("alist", alist);
    }
    std::list<A *> alist1;
    {
        test_istream is(testfile, TEST_STREAM_FLAGS);
        test_iarchive ia(is);
        ia >> boost::serialization::make_nvp("alist", alist1);
    }
    BOOST_CHECK(alist.size() == alist1.size() &&
        std::equal(alist.begin(),alist.end(),alist1.begin(),ptr_equal_to<A *>())
    );
    std::for_each(
        alist.begin(), 
        alist.end(), 
        boost::checked_deleter<A>()
    );
    std::for_each(
        alist1.begin(), 
        alist1.end(), 
        boost::checked_deleter<A>()
    );
    
    #ifdef BOOST_HAS_SLIST
    std::list<A *> aslist;
    aslist.push_back(new A);
    aslist.push_back(new A);
    {   
        test_ostream os(testfile, TEST_STREAM_FLAGS);
        test_oarchive oa(os);
        aslist.push_back(new A);
        aslist.push_back(new A);
        oa << boost::serialization::make_nvp("aslist", aslist);
    }
    std::list<A *> aslist1;{
        test_istream is(testfile, TEST_STREAM_FLAGS);
        test_iarchive ia(is);
        ia >> boost::serialization::make_nvp("aslist", aslist1);
    }
    BOOST_CHECK(aslist.size() == aslist1.size() &&
        std::equal(aslist.begin(),aslist.end(),aslist1.begin(),ptr_equal_to<A *>())
    );
    std::for_each(
        aslist.begin(), 
        aslist.end(), 
        boost::checked_deleter<A>()
    );
    std::for_each(
        aslist1.begin(), 
        aslist1.end(), 
        boost::checked_deleter<A>()
    );  
    #endif
    std::remove(testfile);
    return boost::exit_success;
}

// EOF
