/* Simple program that uses the gregorian calendar to progress by exactly
 * one month, regardless of how many days are in that month.
 *
 * This method can be used as an alternative to iterators
 */

#include "boost/date_time/gregorian/gregorian.hpp"
#include <iostream>

int
main()
{

  using namespace boost::gregorian;
  typedef boost::date_time::month_functor<date> add_month;

  date d = day_clock::local_day();
  date d2 = d + months(1); 
  date d3 = d - months(1);
  std::cout << "Today is: " << to_simple_string(d) << ".\n"
            << "One month from today will be: " << to_simple_string(d2) 
            << "One month ago was: " << to_simple_string(d3)
            << std::endl;
  std::cout << "******** Warning read this ***********************\n";
  std::cout << "Be aware that adding a month is not exactly like regular numeric math.\n"
            << "Addition/Subtraction of months will 'snap to the end' of a month that\n"
            << "is shorter than the current month.  For example consider "
            << "Jan 31, 2004 + (1 month)\n";
  date d4(2004, Jan, 31);
  date d5 = d4 + months(1);
  std::cout << "Result is: " << to_simple_string(d5) 
            << std::endl;

  std::cout << "\nSo what does this mean?  It means the result of adding months is order\n"
            << "dependent, non-communitive, and may create problems for applications.\n"
            << "Consider: \n"
            << "Jan 30, 2004 + (1 month) + (1 month) != Jan 29, 2004 + (2 months)\n"
            << "Why not? Because Jan 30, 2004 + 1 month is Feb 29 + 1 month is Mar 29th.\n"
            << "while Jan 30, 2004 + 2 months is Mar 29th.\n"
            << "All of this clears up as long as all the starting dates before the 28th of\n"
            << "the month -- then all the behavior follows classical mathematical rules.\n";
  

  return 0;
}

/*  Copyright 2001-2004: CrystalClear Software, Inc
 *  http://www.crystalclearsoftware.com
 *
 *  Subject to the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE-1.0 or http://www.boost.org/LICENSE-1.0)
 */

