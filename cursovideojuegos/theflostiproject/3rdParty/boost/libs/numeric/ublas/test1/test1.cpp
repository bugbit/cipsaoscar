//
//  Copyright (c) 2000-2002
//  Joerg Walter, Mathias Koch
//
//  Permission to use, copy, modify, distribute and sell this software
//  and its documentation for any purpose is hereby granted without fee,
//  provided that the above copyright notice appear in all copies and
//  that both that copyright notice and this permission notice appear
//  in supporting documentation.  The authors make no representations
//  about the suitability of this software for any purpose.
//  It is provided "as is" without express or implied warranty.
//
//  The authors gratefully acknowledge the support of
//  GeNeSys mbH & Co. KG in producing this work.
//

#include "test1.hpp"

#ifdef BOOST_MSVC
// Standard new handler is not standard compliant.
#include <new.h>
int __cdecl std_new_handler (unsigned) {
    throw std::bad_alloc ();
}
#endif

int main () {
#ifdef BOOST_MSVC
    _set_new_handler (std_new_handler);
#endif
    test_vector ();
    test_matrix_vector ();
    test_matrix ();
    return 0;
}


