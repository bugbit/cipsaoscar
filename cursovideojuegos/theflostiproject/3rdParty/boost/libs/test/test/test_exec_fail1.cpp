//  (C) Copyright Gennadiy Rozental 2001-2004.
//  (C) Copyright Beman Dawes 2000.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile: test_exec_fail1.cpp,v $
//
//  Version     : $Revision: 1.12 $
//
//  Description : test failure reported via non-zero return value. Should fail 
//  during run.
// ***************************************************************************

// Boost.Test
#include <boost/test/test_tools.hpp>

int test_main( int, char* [] )  // note the name
{
    return 1;
}

//____________________________________________________________________________//

// ***************************************************************************
//  Revision History :
//  
//  $Log: test_exec_fail1.cpp,v $
//  Revision 1.12  2004/05/21 06:26:10  rogeeff
//  licence update
//
//  Revision 1.11  2003/12/01 00:42:38  rogeeff
//  prerelease cleaning
//

// ***************************************************************************

// EOF
