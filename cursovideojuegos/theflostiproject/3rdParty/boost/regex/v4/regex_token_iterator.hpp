/*
 *
 * Copyright (c) 2003
 * Dr John Maddock
 *
 * Use, modification and distribution are subject to the 
 * Boost Software License, Version 1.0. (See accompanying file 
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 */

 /*
  *   LOCATION:    see http://www.boost.org for most recent version.
  *   FILE         regex_token_iterator.hpp
  *   VERSION      see <boost/version.hpp>
  *   DESCRIPTION: Provides regex_token_iterator implementation.
  */

#ifndef BOOST_REGEX_V4_REGEX_TOKEN_ITERATOR_HPP
#define BOOST_REGEX_V4_REGEX_TOKEN_ITERATOR_HPP

#include <boost/shared_ptr.hpp>
#include <boost/detail/workaround.hpp>
#if (BOOST_WORKAROUND(__BORLANDC__, >= 0x560) && BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x570)))\
      || BOOST_WORKAROUND(BOOST_MSVC, < 1300) \
      || BOOST_WORKAROUND(__MWERKS__, BOOST_TESTED_AT(0x3003)) \
      || BOOST_WORKAROUND(__HP_aCC, BOOST_TESTED_AT(55500))
//
// Borland C++ Builder 6, and Visual C++ 6,
// can't cope with the array template constructor
// so we have a template member that will accept any type as 
// argument, and then assert that is really is an array:
//
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_array.hpp>
#endif

namespace boost{

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_PREFIX
#endif
#if BOOST_WORKAROUND(BOOST_MSVC, > 1300)
#  pragma warning(push)
#  pragma warning(disable:4700)
#endif

template <class BidirectionalIterator,
          class charT,
          class traits,
          class Allocator>
class regex_token_iterator_implementation 
{
   typedef basic_regex<charT, traits, Allocator> regex_type;
   typedef sub_match<BidirectionalIterator>      value_type;

   match_results<BidirectionalIterator> what;   // current match
   BidirectionalIterator                end;    // end of search area
   const regex_type*                    pre;    // the expression
   match_flag_type                      flags;  // match flags
   value_type                           result; // the current string result
   int                                  N;      // the current sub-expression being enumerated
   std::vector<int>                     subs;   // the sub-expressions to enumerate

public:
   regex_token_iterator_implementation(const regex_type* p, BidirectionalIterator last, int sub, match_flag_type f)
      : end(last), pre(p), flags(f){ subs.push_back(sub); }
   regex_token_iterator_implementation(const regex_type* p, BidirectionalIterator last, const std::vector<int>& v, match_flag_type f)
      : end(last), pre(p), flags(f), subs(v){}
#if (BOOST_WORKAROUND(__BORLANDC__, >= 0x560) && BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x570)))\
      || BOOST_WORKAROUND(BOOST_MSVC, < 1300) \
      || BOOST_WORKAROUND(__MWERKS__, BOOST_TESTED_AT(0x3003)) \
      || BOOST_WORKAROUND(__HP_aCC, BOOST_TESTED_AT(55500))
   template <class T>
   regex_token_iterator_implementation(const regex_type* p, BidirectionalIterator last, const T& submatches, match_flag_type f)
      : end(last), pre(p), flags(f)
   {
      // assert that T really is an array:
      BOOST_STATIC_ASSERT(::boost::is_array<T>::value);
      const std::size_t array_size = sizeof(T) / sizeof(submatches[0]);
      for(std::size_t i = 0; i < array_size; ++i)
      {
         subs.push_back(submatches[i]);
      }
   }
#else
   template <std::size_t CN>
   regex_token_iterator_implementation(const regex_type* p, BidirectionalIterator last, const int (&submatches)[CN], match_flag_type f)
      : end(last), pre(p), flags(f)
   {
      for(std::size_t i = 0; i < CN; ++i)
      {
         subs.push_back(submatches[i]);
      }
   }
#endif

   bool init(BidirectionalIterator first)
   {
      N = 0;
      if(regex_search(first, end, what, *pre, flags) == true)
      {
         N = 0;
         result = ((subs[N] == -1) ? what.prefix() : what[(int)subs[N]]);
         return true;
      }
      else if((subs[N] == -1) && (first != end))
      {
         result.first = first;
         result.second = end;
         result.matched = (first != end);
         return true;
      }
      return false;
   }
   bool compare(const regex_token_iterator_implementation& that)
   {
      if(this == &that) return true;
      return (pre == that.pre) 
         && (end == that.end) 
         && (flags == that.flags) 
         && (N == that.N) 
         && (what[0].first == that.what[0].first) 
         && (what[0].second == that.what[0].second);
   }
   const value_type& get()
   { return result; }
   bool next()
   {
      if(N == -1)
         return false;
      if(N+1 < (int)subs.size())
      {
         ++N;
         result =((subs[N] == -1) ? what.prefix() : what[subs[N]]);
         return true;
      }
      if(what.prefix().first != what[0].second)
         flags |= match_prev_avail;
      BidirectionalIterator last_end(what[0].second);
      if(regex_search(last_end, end, what, *pre, ((what[0].first == what[0].second) ? flags | regex_constants::match_not_initial_null : flags)))
      {
         N =0;
         result =((subs[N] == -1) ? what.prefix() : what[subs[N]]);
         return true;
      }
      else if((last_end != end) && (subs[0] == -1))
      {
         N =-1;
         result.first = last_end;
         result.second = end;
         result.matched = (last_end != end);
         return true;
      }
      return false;
   }
};

template <class BidirectionalIterator, 
          class charT = BOOST_DEDUCED_TYPENAME re_detail::regex_iterator_traits<BidirectionalIterator>::value_type,
          class traits = regex_traits<charT>,
          class Allocator = BOOST_DEFAULT_ALLOCATOR(charT) >
class regex_token_iterator 
#ifndef BOOST_NO_STD_ITERATOR
   : public std::iterator<
         std::forward_iterator_tag, 
         sub_match<BidirectionalIterator>,
         typename re_detail::regex_iterator_traits<BidirectionalIterator>::difference_type,
         const sub_match<BidirectionalIterator>*,
         const sub_match<BidirectionalIterator>& >         
#endif
{
private:
   typedef regex_token_iterator_implementation<BidirectionalIterator, charT, traits, Allocator> impl;
   typedef shared_ptr<impl> pimpl;
public:
   typedef          basic_regex<charT, traits, Allocator>                   regex_type;
   typedef          sub_match<BidirectionalIterator>                        value_type;
   typedef typename re_detail::regex_iterator_traits<BidirectionalIterator>::difference_type 
                                                                            difference_type;
   typedef          const value_type*                                       pointer;
   typedef          const value_type&                                       reference; 
   typedef          std::forward_iterator_tag                               iterator_category;
   
   regex_token_iterator(){}
   regex_token_iterator(BidirectionalIterator a, BidirectionalIterator b, const regex_type& re, 
                        int submatch = 0, match_flag_type m = match_default)
                        : pdata(new impl(&re, b, submatch, m))
   {
      if(!pdata->init(a))
         pdata.reset();
   }
   regex_token_iterator(BidirectionalIterator a, BidirectionalIterator b, const regex_type& re, 
                        const std::vector<int>& submatches, match_flag_type m = match_default)
                        : pdata(new impl(&re, b, submatches, m))
   {
      if(!pdata->init(a))
         pdata.reset();
   }
#if (BOOST_WORKAROUND(__BORLANDC__, >= 0x560) && BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x570)))\
      || BOOST_WORKAROUND(BOOST_MSVC, < 1300) \
      || BOOST_WORKAROUND(__MWERKS__, BOOST_TESTED_AT(0x3003)) \
      || BOOST_WORKAROUND(__HP_aCC, BOOST_TESTED_AT(55500))
   template <class T>
   regex_token_iterator(BidirectionalIterator a, BidirectionalIterator b, const regex_type& re,
                        const T& submatches, match_flag_type m = match_default)
                        : pdata(new impl(&re, b, submatches, m))
   {
      if(!pdata->init(a))
         pdata.reset();
   }
#else
   template <std::size_t N>
   regex_token_iterator(BidirectionalIterator a, BidirectionalIterator b, const regex_type& re,
                        const int (&submatches)[N], match_flag_type m = match_default)
                        : pdata(new impl(&re, b, submatches, m))
   {
      if(!pdata->init(a))
         pdata.reset();
   }
#endif
   regex_token_iterator(const regex_token_iterator& that)
      : pdata(that.pdata) {}
   regex_token_iterator& operator=(const regex_token_iterator& that)
   {
      pdata = that.pdata;
      return *this;
   }
   bool operator==(const regex_token_iterator& that)const
   { 
      if((pdata.get() == 0) || (that.pdata.get() == 0))
         return pdata.get() == that.pdata.get();
      return pdata->compare(*(that.pdata.get())); 
   }
   bool operator!=(const regex_token_iterator& that)const
   { return !(*this == that); }
   const value_type& operator*()const
   { return pdata->get(); }
   const value_type* operator->()const
   { return &(pdata->get()); }
   regex_token_iterator& operator++()
   {
      cow();
      if(0 == pdata->next())
      {
         pdata.reset();
      }
      return *this;
   }
   regex_token_iterator operator++(int)
   {
      regex_token_iterator result(*this);
      ++(*this);
      return result;
   }
private:

   pimpl pdata;

   void cow()
   {
      // copy-on-write
      if(pdata.get() && !pdata.unique())
      {
         pdata.reset(new impl(*(pdata.get())));
      }
   }
};

typedef regex_token_iterator<const char*> cregex_token_iterator;
typedef regex_token_iterator<std::string::const_iterator> sregex_token_iterator;
#ifndef BOOST_NO_WREGEX
typedef regex_token_iterator<const wchar_t*> wcregex_token_iterator;
typedef regex_token_iterator<std::wstring::const_iterator> wsregex_token_iterator;
#endif

#if BOOST_WORKAROUND(BOOST_MSVC, == 1310)
#  pragma warning(pop)
#endif
#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif

} // namespace boost

#endif // BOOST_REGEX_V4_REGEX_TOKEN_ITERATOR_HPP




