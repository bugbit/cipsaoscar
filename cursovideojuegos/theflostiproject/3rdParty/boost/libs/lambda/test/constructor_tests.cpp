//  constructor_tests.cpp  -- The Boost Lambda Library ------------------
//
// Copyright (C) 2000-2003 Jaakko J�rvi (jaakko.jarvi@cs.utu.fi)
// Copyright (C) 2000-2003 Gary Powell (powellg@amazon.com)
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see www.boost.org

// -----------------------------------------------------------------------


#include <boost/test/minimal.hpp>    // see "Header Implementation Option"


#include "boost/lambda/lambda.hpp"
#include "boost/lambda/bind.hpp"

#include "boost/lambda/construct.hpp"

#include <iostream>
#include <algorithm>
#include <vector>

using namespace boost::lambda;
using namespace std;

template<class T>
bool check_tuple(int n, const T& t) 
{
  return (t.get_head() == n) && check_tuple(n+1, t.get_tail()); 
}

template <>
bool check_tuple(int n, const null_type& ) { return true; }


void constructor_all_lengths() 
{
  bool ok;
  ok = check_tuple(
    1, 
    bind(constructor<tuple<int> >(),
       1)()
  );
  BOOST_TEST(ok);

  ok = check_tuple(
    1, 
    bind(constructor<tuple<int, int> >(),
       1, 2)()
  );
  BOOST_TEST(ok);

  ok = check_tuple(
    1, 
    bind(constructor<tuple<int, int, int> >(),
       1, 2, 3)()
  );
  BOOST_TEST(ok);

  ok = check_tuple(
    1, 
    bind(constructor<tuple<int, int, int, int> >(),
       1, 2, 3, 4)()
  );
  BOOST_TEST(ok);

  ok = check_tuple(
    1, 
    bind(constructor<tuple<int, int, int, int, int> >(),
       1, 2, 3, 4, 5)()
  );
  BOOST_TEST(ok);

  ok = check_tuple(
    1, 
    bind(constructor<tuple<int, int, int, int, int, int> >(),
       1, 2, 3, 4, 5, 6)()
  );
  BOOST_TEST(ok);

  ok = check_tuple(
    1, 
    bind(constructor<tuple<int, int, int, int, int, int, int> >(),
       1, 2, 3, 4, 5, 6, 7)()
  );
  BOOST_TEST(ok);

  ok = check_tuple(
    1, 
    bind(constructor<tuple<int, int, int, int, int, int, int, int> >(),
       1, 2, 3, 4, 5, 6, 7, 8)()
  );
  BOOST_TEST(ok);

  ok = check_tuple(
    1, 
    bind(constructor<tuple<int, int, int, int, int, int, int, int, int> >(),
       1, 2, 3, 4, 5, 6, 7, 8, 9)()
  );
  BOOST_TEST(ok);

}

void new_ptr_all_lengths() 
{
  bool ok;
  ok = check_tuple(
    1, 
    *(bind(new_ptr<tuple<int> >(),
       1))()
  );
  BOOST_TEST(ok);

  ok = check_tuple(
    1, 
    *(bind(new_ptr<tuple<int, int> >(),
       1, 2))()
  );
  BOOST_TEST(ok);

  ok = check_tuple(
    1, 
    *(bind(new_ptr<tuple<int, int, int> >(),
       1, 2, 3))()
  );
  BOOST_TEST(ok);

  ok = check_tuple(
    1, 
    *(bind(new_ptr<tuple<int, int, int, int> >(),
       1, 2, 3, 4))()
  );
  BOOST_TEST(ok);

  ok = check_tuple(
    1, 
    *(bind(new_ptr<tuple<int, int, int, int, int> >(),
       1, 2, 3, 4, 5))()
  );
  BOOST_TEST(ok);

  ok = check_tuple(
    1, 
    *(bind(new_ptr<tuple<int, int, int, int, int, int> >(),
       1, 2, 3, 4, 5, 6))()
  );
  BOOST_TEST(ok);

  ok = check_tuple(
    1, 
    *(bind(new_ptr<tuple<int, int, int, int, int, int, int> >(),
       1, 2, 3, 4, 5, 6, 7))()
  );
  BOOST_TEST(ok);

  ok = check_tuple(
    1, 
    *(bind(new_ptr<tuple<int, int, int, int, int, int, int, int> >(),
       1, 2, 3, 4, 5, 6, 7, 8))()
  );
  BOOST_TEST(ok);

  ok = check_tuple(
    1, 
    *(bind(new_ptr<tuple<int, int, int, int, int, int, int, int, int> >(),
       1, 2, 3, 4, 5, 6, 7, 8, 9))()
  );
  BOOST_TEST(ok);

}

class is_destructor_called {
  bool& b;
public: 
  is_destructor_called(bool& bb) : b(bb) { b = false; }
  ~is_destructor_called() { b = true; }
};

void test_destructor ()
{ 
  char space[sizeof(is_destructor_called)];
  bool flag;

  is_destructor_called* idc = new(space) is_destructor_called(flag);
  BOOST_TEST(flag == false);
  bind(destructor(), _1)(idc);
  BOOST_TEST(flag == true);

  idc = new(space) is_destructor_called(flag);
  BOOST_TEST(flag == false);
  bind(destructor(), _1)(*idc);
  BOOST_TEST(flag == true);
}


class count_deletes {
public:
  static int count;
  ~count_deletes() { ++count; }
};

int count_deletes::count = 0;

void test_news_and_deletes ()
{
  int* i[10];
  for_each(i, i+10, _1 = bind(new_ptr<int>(), 2));
  int count_errors = 0;

  for_each(i, i+10, (*_1 == 2) || ++var(count_errors));
  BOOST_TEST(count_errors == 0);


  count_deletes* ct[10];
  for_each(ct, ct+10, _1 = bind(new_ptr<count_deletes>()));
  count_deletes::count = 0;
  for_each(ct, ct+10, bind(delete_ptr(), _1));
  BOOST_TEST(count_deletes::count == 10);
   
}

void test_array_new_and_delete()
{
  count_deletes* c;
  (_1 = bind(new_array<count_deletes>(), 5))(c);
  count_deletes::count = 0;

  bind(delete_array(), _1)(c);
  BOOST_TEST(count_deletes::count == 5);
}


void delayed_construction()
{
  vector<int> x(3);
  vector<int> y(3);

  fill(x.begin(), x.end(), 0);
  fill(y.begin(), y.end(), 1);
  
  vector<pair<int, int> > v;

  transform(x.begin(), x.end(), y.begin(), back_inserter(v),
            bind(constructor<pair<int, int> >(), _1, _2) );
}

int test_main(int, char *[]) {

  constructor_all_lengths();
  new_ptr_all_lengths();
  delayed_construction();
  test_destructor();
  test_news_and_deletes();  
  test_array_new_and_delete();
  
  return 0;
}
