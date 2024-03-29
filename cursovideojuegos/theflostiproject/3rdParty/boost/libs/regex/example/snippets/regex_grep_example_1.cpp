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
  *   FILE         regex_grep_example_1.cpp
  *   VERSION      see <boost/version.hpp>
  *   DESCRIPTION: regex_grep example 1: searches a cpp file for class definitions.
  */

#include <string>
#include <map>
#include <boost/regex.hpp>

// purpose:
// takes the contents of a file in the form of a string
// and searches for all the C++ class definitions, storing
// their locations in a map of strings/int's

typedef std::map<std::string, std::string::difference_type, std::less<std::string> > map_type;

const char* re = 
   // possibly leading whitespace:   
   "^[[:space:]]*" 
   // possible template declaration:
   "(template[[:space:]]*<[^;:{]+>[[:space:]]*)?"
   // class or struct:
   "(class|struct)[[:space:]]*" 
   // leading declspec macros etc:
   "("
      "\\<\\w+\\>"
      "("
         "[[:blank:]]*\\([^)]*\\)"
      ")?"
      "[[:space:]]*"
   ")*" 
   // the class name
   "(\\<\\w*\\>)[[:space:]]*" 
   // template specialisation parameters
   "(<[^;:{]+>)?[[:space:]]*"
   // terminate in { or :
   "(\\{|:[^;\\{()]*\\{)";

boost::regex expression(re);

class IndexClassesPred
{
   map_type& m;
   std::string::const_iterator base;
public:
   IndexClassesPred(map_type& a, std::string::const_iterator b) : m(a), base(b) {}
   bool operator()(const boost::match_results<std::string::const_iterator>& what)
   {
      // what[0] contains the whole string
      // what[5] contains the class name.
      // what[6] contains the template specialisation if any.
      // add class name and position to map:
      m[std::string(what[5].first, what[5].second) + std::string(what[6].first, what[6].second)] = 
               what[5].first - base;
      return true;
   }
};

void IndexClasses(map_type& m, const std::string& file)
{
   std::string::const_iterator start, end;
   start = file.begin();
   end = file.end();
   boost::regex_grep(IndexClassesPred(m, start), start, end, expression);
}


#include <fstream>
#include <iostream>

using namespace std;

void load_file(std::string& s, std::istream& is)
{
   s.erase();
   if(is.bad()) return;
   s.reserve(is.rdbuf()->in_avail());
   char c;
   while(is.get(c))
   {
      if(s.capacity() == s.size())
         s.reserve(s.capacity() * 3);
      s.append(1, c);
   }
}

int main(int argc, const char** argv)
{
   std::string text;
   for(int i = 1; i < argc; ++i)
   {
      cout << "Processing file " << argv[i] << endl;
      map_type m;
      std::ifstream fs(argv[i]);
      load_file(text, fs);
      fs.close();
      IndexClasses(m, text);
      cout << m.size() << " matches found" << endl;
      map_type::iterator c, d;
      c = m.begin();
      d = m.end();
      while(c != d)
      {
         cout << "class \"" << (*c).first << "\" found at index: " << (*c).second << endl;
         ++c;
      }
   }
   return 0;
}







