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

// Test matrix expression templates
template<class M, int N>
struct test_my_matrix {
    typedef typename M::value_type value_type;

    template<class MP>
    void test_with (MP &m1, MP &m2, MP &m3) const {
        {
            value_type t;

            // Default Construct
            MP default_constuct;
            initialize_matrix (default_constuct);
            std::cout << "default construct = " << default_constuct << std::endl;
            
            // Copy and swap
            initialize_matrix (m1);
            initialize_matrix (m2);
            m1 = m2;
            std::cout << "m1 = m2 = " << m1 << std::endl;
            m1.assign_temporary (m2);
            std::cout << "m1.assign_temporary (m2) = " << m1 << std::endl;
            m1.swap (m2);
            std::cout << "m1.swap (m2) = " << m1 << " " << m2 << std::endl;

#ifndef BOOST_NO_FUNCTION_TEMPLATE_ORDERING
            // Project range and slice
            initialize_matrix (m1);
            initialize_matrix (m2);
            project (m1, ublas::range(0,1),ublas::range(0,1)) = project (m2, ublas::range(0,1),ublas::range(0,1));
            project (m1, ublas::range(0,1),ublas::range(0,1)) = project (m2, ublas::slice(0,1,1),ublas::slice(0,1,1));
            project (m1, ublas::slice(2,-1,2),ublas::slice(2,-1,2)) = project (m2, ublas::slice(0,1,2),ublas::slice(0,1,2));
            project (m1, ublas::slice(2,-1,2),ublas::slice(2,-1,2)) = project (m2, ublas::range(0,2),ublas::range(0,2));
            std::cout << "m1 = range/slice " << m1 << std::endl;
#endif

            // Unary matrix operations resulting in a matrix
            initialize_matrix (m1);
            m2 = - m1;
            std::cout << "- m1 = " << m2 << std::endl;
            m2 = ublas::conj (m1);
            std::cout << "conj (m1) = " << m2 << std::endl;

            // Binary matrix operations resulting in a matrix
            initialize_matrix (m1);
            initialize_matrix (m2);
            m3 = m1 + m2;
            std::cout << "m1 + m2 = " << m3 << std::endl;
            m3 = m1 - m2;
            std::cout << "m1 - m2 = " << m3 << std::endl;
            m3 = ublas::element_prod (m1, m2);
            std::cout << "element_prod (m1, m2) = " << m3 << std::endl;

            // Scaling a matrix
            t = N;
            initialize_matrix (m1);
            m2 = value_type (1.) * m1;
            std::cout << "1. * m1 = " << m2 << std::endl;
            m2 = t * m1;
            std::cout << "N * m1 = " << m2 << std::endl;
            initialize_matrix (m1);
            m2 = m1 * value_type (1.);
            std::cout << "m1 * 1. = " << m2 << std::endl;
            m2 = m1 * t;
            std::cout << "m1 * N = " << m2 << std::endl;

            // Some assignments
            initialize_matrix (m1);
            initialize_matrix (m2);
            m2 += m1;
            std::cout << "m2 += m1 = " << m2 << std::endl;
            m2 -= m1;
            std::cout << "m2 -= m1 = " << m2 << std::endl;
            m2 = m2 + m1;
            std::cout << "m2 = m2 + m1 = " << m2 << std::endl;
            m2 = m2 - m1;
            std::cout << "m2 = m2 - m1 = " << m2 << std::endl;
            m1 *= value_type (1.);
            std::cout << "m1 *= 1. = " << m1 << std::endl;
            m1 *= t;
            std::cout << "m1 *= N = " << m1 << std::endl;

            // Transpose
            initialize_matrix (m1);
            m2 = ublas::trans (m1);
            std::cout << "trans (m1) = " << m2 << std::endl;

            // Hermitean
            initialize_matrix (m1);
            m2 = ublas::herm (m1);
            std::cout << "herm (m1) = " << m2 << std::endl;

            // Matrix multiplication
            initialize_matrix (m1);
            initialize_matrix (m2);
            m3 = ublas::prod (m1, m2);
            std::cout << "prod (m1, m2) = " << m3 << std::endl;
        }
    }
    void operator () () const {
        {
            M m1 (N, N), m2 (N, N), m3 (N, N);
            test_with (m1, m2, m3);

#ifdef USE_RANGE
            ublas::matrix_range<M> mr1 (m1, ublas::range (0, N), ublas::range (0, N)),
                                   mr2 (m2, ublas::range (0, N), ublas::range (0, N)),
                                   mr3 (m3, ublas::range (0, N), ublas::range (0, N));
            test_with (mr1, mr2, mr3);
#endif

#ifdef USE_SLICE
            ublas::matrix_slice<M> ms1 (m1, ublas::slice (0, 1, N), ublas::slice (0, 1, N)),
                                   ms2 (m2, ublas::slice (0, 1, N), ublas::slice (0, 1, N)),
                                   ms3 (m3, ublas::slice (0, 1, N), ublas::slice (0, 1, N));
            test_with (ms1, ms2, ms3);
#endif
        }
    }
};

// Test matrix
void test_matrix () {
    std::cout << "test_matrix" << std::endl;

#ifdef USE_MATRIX
#ifdef USE_BOUNDED_ARRAY
#ifdef USE_FLOAT
    std::cout << "float, bounded_array" << std::endl;
    test_my_matrix<ublas::matrix<float, ublas::row_major, ublas::bounded_array<float, 3 * 3> >, 3> () ();
#endif

#ifdef USE_DOUBLE
    std::cout << "double, bounded_array" << std::endl;
    test_my_matrix<ublas::matrix<double, ublas::row_major, ublas::bounded_array<double, 3 * 3> >, 3> () ();
#endif

#ifdef USE_STD_COMPLEX
#ifdef USE_FLOAT
    std::cout << "std::complex<float>, bounded_array" << std::endl;
    test_my_matrix<ublas::matrix<std::complex<float>, ublas::row_major, ublas::bounded_array<std::complex<float>, 3 * 3> >, 3> () ();
#endif

#ifdef USE_DOUBLE
    std::cout << "std::complex<double>, bounded_array" << std::endl;
    test_my_matrix<ublas::matrix<std::complex<double>, ublas::row_major, ublas::bounded_array<std::complex<double>, 3 * 3> >, 3> () ();
#endif
#endif
#endif

#ifdef USE_UNBOUNDED_ARRAY
#ifdef USE_FLOAT
    std::cout << "float, unbounded_array" << std::endl;
    test_my_matrix<ublas::matrix<float, ublas::row_major, ublas::unbounded_array<float> >, 3> () ();
#endif

#ifdef USE_DOUBLE
    std::cout << "double, unbounded_array" << std::endl;
    test_my_matrix<ublas::matrix<double, ublas::row_major, ublas::unbounded_array<double> >, 3> () ();
#endif

#ifdef USE_STD_COMPLEX
#ifdef USE_FLOAT
    std::cout << "std::complex<float>, unbounded_array" << std::endl;
    test_my_matrix<ublas::matrix<std::complex<float>, ublas::row_major, ublas::unbounded_array<std::complex<float> > >, 3> () ();
#endif

#ifdef USE_DOUBLE
    std::cout << "std::complex<double>, unbounded_array" << std::endl;
    test_my_matrix<ublas::matrix<std::complex<double>, ublas::row_major, ublas::unbounded_array<std::complex<double> > >, 3> () ();
#endif
#endif
#endif

#ifdef USE_STD_VECTOR
#ifdef USE_FLOAT
    std::cout << "float, std::vector" << std::endl;
    test_my_matrix<ublas::matrix<float, ublas::row_major, std::vector<float> >, 3> () ();
#endif

#ifdef USE_DOUBLE
    std::cout << "double, std::vector" << std::endl;
    test_my_matrix<ublas::matrix<double, ublas::row_major, std::vector<double> >, 3> () ();
#endif

#ifdef USE_STD_COMPLEX
#ifdef USE_FLOAT
    std::cout << "std::complex<float>, std::vector" << std::endl;
    test_my_matrix<ublas::matrix<std::complex<float>, ublas::row_major, std::vector<std::complex<float> > >, 3> () ();
#endif

#ifdef USE_DOUBLE
    std::cout << "std::complex<double>, std::vector" << std::endl;
    test_my_matrix<ublas::matrix<std::complex<double>, ublas::row_major, std::vector<std::complex<double> > >, 3> () ();
#endif
#endif
#endif
#endif

#ifdef USE_BOUNDED_MATRIX
#ifdef USE_FLOAT
    std::cout << "float, bounded" << std::endl;
    test_my_matrix<ublas::bounded_matrix<float, 3, 3>, 3> () ();
#endif

#ifdef USE_DOUBLE
    std::cout << "double, bounded" << std::endl;
    test_my_matrix<ublas::bounded_matrix<double, 3, 3>, 3> () ();
#endif

#ifdef USE_STD_COMPLEX
#ifdef USE_FLOAT
    std::cout << "std::complex<float>, bounded" << std::endl;
    test_my_matrix<ublas::bounded_matrix<std::complex<float>, 3, 3>, 3> () ();
#endif

#ifdef USE_DOUBLE
    std::cout << "std::complex<double>, bounded" << std::endl;
    test_my_matrix<ublas::bounded_matrix<std::complex<double>, 3, 3>, 3> () ();
#endif
#endif
#endif

#ifdef USE_VECTOR_OF_VECTOR
#ifdef USE_BOUNDED_ARRAY
#ifdef USE_FLOAT
    std::cout << "float, bounded_array" << std::endl;
    test_my_matrix<ublas::vector_of_vector<float, ublas::row_major, ublas::bounded_array<ublas::bounded_array<float, 3>, 3 + 1> >, 3> () ();
#endif

#ifdef USE_DOUBLE
    std::cout << "double, bounded_array" << std::endl;
    test_my_matrix<ublas::vector_of_vector<double, ublas::row_major, ublas::bounded_array<ublas::bounded_array<double, 3>, 3 + 1> >, 3> () ();
#endif

#ifdef USE_STD_COMPLEX
#ifdef USE_FLOAT
    std::cout << "std::complex<float>, bounded_array" << std::endl;
    test_my_matrix<ublas::vector_of_vector<std::complex<float>, ublas::row_major, ublas::bounded_array<ublas::bounded_array<std::complex<float>, 3>, 3 + 1> >, 3> () ();
#endif

#ifdef USE_DOUBLE
    std::cout << "std::complex<double>, bounded_array" << std::endl;
    test_my_matrix<ublas::vector_of_vector<std::complex<double>, ublas::row_major, ublas::bounded_array<ublas::bounded_array<std::complex<double>, 3>, 3 + 1> >, 3> () ();
#endif
#endif
#endif

#ifdef USE_UNBOUNDED_ARRAY
#ifdef USE_FLOAT
    std::cout << "float, unbounded_array" << std::endl;
    test_my_matrix<ublas::vector_of_vector<float, ublas::row_major, ublas::unbounded_array<ublas::unbounded_array<float> > >, 3> () ();
#endif

#ifdef USE_DOUBLE
    std::cout << "double, unbounded_array" << std::endl;
    test_my_matrix<ublas::vector_of_vector<double, ublas::row_major, ublas::unbounded_array<ublas::unbounded_array<double> > >, 3> () ();
#endif

#ifdef USE_STD_COMPLEX
#ifdef USE_FLOAT
    std::cout << "std::complex<float>, unbounded_array" << std::endl;
    test_my_matrix<ublas::vector_of_vector<std::complex<float>, ublas::row_major, ublas::unbounded_array<ublas::unbounded_array<std::complex<float> > > >, 3> () ();
#endif

#ifdef USE_DOUBLE
    std::cout << "std::complex<double>, unbounded_array" << std::endl;
    test_my_matrix<ublas::vector_of_vector<std::complex<double>, ublas::row_major, ublas::unbounded_array<ublas::unbounded_array<std::complex<double> > > >, 3> () ();
#endif
#endif
#endif

#ifdef USE_STD_VECTOR
#ifdef USE_FLOAT
    std::cout << "float, std::vector" << std::endl;
    test_my_matrix<ublas::vector_of_vector<float, ublas::row_major, std::vector<std::vector<float > > >, 3> () ();
#endif

#ifdef USE_DOUBLE
    std::cout << "double, std::vector" << std::endl;
    test_my_matrix<ublas::vector_of_vector<double, ublas::row_major, std::vector<std::vector<double> > >, 3> () ();
#endif

#ifdef USE_STD_COMPLEX
#ifdef USE_FLOAT
    std::cout << "std::complex<float>, std::vector" << std::endl;
    test_my_matrix<ublas::vector_of_vector<std::complex<float>, ublas::row_major, std::vector<std::vector<std::complex<float> > > >, 3> () ();
#endif

#ifdef USE_DOUBLE
    std::cout << "std::complex<double>, std::vector" << std::endl;
    test_my_matrix<ublas::vector_of_vector<std::complex<double>, ublas::row_major, std::vector<std::vector<std::complex<double> > > >, 3> () ();
#endif
#endif
#endif
#endif
}
