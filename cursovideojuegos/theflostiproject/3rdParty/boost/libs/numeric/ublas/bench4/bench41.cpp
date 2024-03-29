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

#include <iostream>
#include <string>

#include <boost/numeric/interval.hpp>
#include <boost/numeric/interval/io.hpp>

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>

#include <boost/timer.hpp>

#include "bench4.hpp"

template<class T, int N>
struct bench_c_inner_prod {
    typedef T value_type;

    void operator () (int runs) const {
        try {
            static typename c_vector_traits<T, N>::type v1, v2;
            initialize_c_vector<T, N> () (v1);
            initialize_c_vector<T, N> () (v2);
            boost::timer t;
            for (int i = 0; i < runs; ++ i) {
                static value_type s (0);
                for (int j = 0; j < N; ++ j) {
                    s += v1 [j] * v2 [j];
                }
//                sink_scalar (s);
            }
            footer<value_type> () (N, N - 1, runs, t.elapsed ());
        }
        catch (std::exception &e) {
            std::cout << e.what () << std::endl;
        }
        catch (...) {
            std::cout << "unknown exception" << std::endl;
        }
    }
};
template<class V, int N>
struct bench_my_inner_prod {
    typedef typename V::value_type value_type;

    void operator () (int runs) const {
        try {
            static V v1 (N), v2 (N);
            initialize_vector (v1);
            initialize_vector (v2);
            boost::timer t;
            for (int i = 0; i < runs; ++ i) {
                static value_type s (0);
                s = ublas::inner_prod (v1, v2);
//                sink_scalar (s);
            }
            footer<value_type> () (N, N - 1, runs, t.elapsed ());
        }
        catch (std::exception &e) {
            std::cout << e.what () << std::endl;
        }
        catch (...) {
            std::cout << "unknown exception" << std::endl;
        }
    }
};
template<class V, int N>
struct bench_cpp_inner_prod {
    typedef typename V::value_type value_type;

    void operator () (int runs) const {
        try {
            static V v1 (N), v2 (N);
            initialize_vector (v1);
            initialize_vector (v2);
            boost::timer t;
            for (int i = 0; i < runs; ++ i) {
                static value_type s (0);
                s = (v1 * v2).sum ();
//                sink_scalar (s);
            }
            footer<value_type> () (N, N - 1, runs, t.elapsed ());
        }
        catch (std::exception &e) {
            std::cout << e.what () << std::endl;
        }
        catch (...) {
            std::cout << "unknown exception" << std::endl;
        }
    }
};

template<class T, int N>
struct bench_c_vector_add {
    typedef T value_type;

    void operator () (int runs) const {
        try {
            static typename c_vector_traits<T, N>::type v1, v2, v3;
            initialize_c_vector<T, N> () (v1);
            initialize_c_vector<T, N> () (v2);
            boost::timer t;
            for (int i = 0; i < runs; ++ i) {
                for (int j = 0; j < N; ++ j) {
                    v3 [j] = - (v1 [j] + v2 [j]);
                }
//                sink_c_vector<T, N> () (v3);
            }
            footer<value_type> () (0, 2 * N, runs, t.elapsed ());
        }
        catch (std::exception &e) {
            std::cout << e.what () << std::endl;
        }
        catch (...) {
            std::cout << "unknown exception" << std::endl;
        }
    }
};
template<class V, int N>
struct bench_my_vector_add {
    typedef typename V::value_type value_type;

    void operator () (int runs, safe_tag) const {
        try {
            static V v1 (N), v2 (N), v3 (N);
            initialize_vector (v1);
            initialize_vector (v2);
            boost::timer t;
            for (int i = 0; i < runs; ++ i) {
                v3 = - (v1 + v2);
//                sink_vector (v3);
            }
            footer<value_type> () (0, 2 * N, runs, t.elapsed ());
        }
        catch (std::exception &e) {
            std::cout << e.what () << std::endl;
        }
        catch (...) {
            std::cout << "unknown exception" << std::endl;
        }
    }
    void operator () (int runs, fast_tag) const {
        try {
            static V v1 (N), v2 (N), v3 (N);
            initialize_vector (v1);
            initialize_vector (v2);
            boost::timer t;
            for (int i = 0; i < runs; ++ i) {
                v3.assign (- (v1 + v2));
//                sink_vector (v3);
            }
            footer<value_type> () (0, 2 * N, runs, t.elapsed ());
        }
        catch (std::exception &e) {
            std::cout << e.what () << std::endl;
        }
        catch (...) {
            std::cout << "unknown exception" << std::endl;
        }
    }
};
template<class V, int N>
struct bench_cpp_vector_add {
    typedef typename V::value_type value_type;

    void operator () (int runs) const {
        try {
            static V v1 (N), v2 (N), v3 (N);
            initialize_vector (v1);
            initialize_vector (v2);
            boost::timer t;
            for (int i = 0; i < runs; ++ i) {
                v3 = - (v1 + v2);
//                sink_vector (v3);
            }
            footer<value_type> () (0, 2 * N, runs, t.elapsed ());
        }
        catch (std::exception &e) {
            std::cout << e.what () << std::endl;
        }
        catch (...) {
            std::cout << "unknown exception" << std::endl;
        }
    }
};

// Benchmark O (n)
template<class T, int N>
void bench_1<T, N>::operator () (int runs) {
    header ("bench_1");

    header ("inner_prod");

    header ("C array");
    bench_c_inner_prod<T, N> () (runs);

#ifdef USE_C_ARRAY
    header ("c_vector");
    bench_my_inner_prod<ublas::c_vector<T, N>, N> () (runs);
#endif

#ifdef USE_BOUNDED_ARRAY
    header ("vector<bounded_array>");
    bench_my_inner_prod<ublas::vector<T, ublas::bounded_array<T, N> >, N> () (runs);
#endif

#ifdef USE_UNBOUNDED_ARRAY
    header ("vector<unbounded_array>");
    bench_my_inner_prod<ublas::vector<T, ublas::unbounded_array<T> >, N> () (runs);
#endif

#ifdef USE_STD_VALARRAY
    header ("vector<std::valarray>");
    bench_my_inner_prod<ublas::vector<T, std::valarray<T> >, N> () ();
#endif

#ifdef USE_STD_VECTOR
    header ("vector<std::vector>");
    bench_my_inner_prod<ublas::vector<T, std::vector<T> >, N> () (runs);
#endif

#ifdef USE_STD_VALARRAY
    header ("std::valarray");
    bench_cpp_inner_prod<std::valarray<T>, N> () (runs);
#endif

    header ("vector + vector");

    header ("C array");
    bench_c_vector_add<T, N> () (runs);

#ifdef USE_C_ARRAY
    header ("c_vector safe");
    bench_my_vector_add<ublas::c_vector<T, N>, N> () (runs, safe_tag ());

    header ("c_vector fast");
    bench_my_vector_add<ublas::c_vector<T, N>, N> () (runs, fast_tag ());
#endif

#ifdef USE_BOUNDED_ARRAY
    header ("vector<bounded_array> safe");
    bench_my_vector_add<ublas::vector<T, ublas::bounded_array<T, N> >, N> () (runs, safe_tag ());

    header ("vector<bounded_array> fast");
    bench_my_vector_add<ublas::vector<T, ublas::bounded_array<T, N> >, N> () (runs, fast_tag ());
#endif

#ifdef USE_UNBOUNDED_ARRAY
    header ("vector<unbounded_array> safe");
    bench_my_vector_add<ublas::vector<T, ublas::unbounded_array<T> >, N> () (runs, safe_tag ());

    header ("vector<unbounded_array> fast");
    bench_my_vector_add<ublas::vector<T, ublas::unbounded_array<T> >, N> () (runs, fast_tag ());
#endif

#ifdef USE_STD_VALARRAY
    header ("vector<std::valarray> safe");
    bench_my_vector_add<ublas::vector<T, std::valarray<T> >, N> () (runs, safe_tag ());

    header ("vector<std::valarray> fast");
    bench_my_vector_add<ublas::vector<T, std::valarray<T> >, N> () (runs, fast_tag ());
#endif

#ifdef USE_STD_VECTOR
    header ("vector<std::vector> safe");
    bench_my_vector_add<ublas::vector<T, std::vector<T> >, N> () (runs, safe_tag ());

    header ("vector<std::vector> fast");
    bench_my_vector_add<ublas::vector<T, std::vector<T> >, N> () (runs, fast_tag ());
#endif

#ifdef USE_STD_VALARRAY
    header ("std::valarray");
    bench_cpp_vector_add<std::valarray<T>, N> () (runs);
#endif
}

#ifdef USE_FLOAT
template struct bench_1<boost::numeric::interval<float>, 3>;
template struct bench_1<boost::numeric::interval<float>, 10>;
template struct bench_1<boost::numeric::interval<float>, 30>;
template struct bench_1<boost::numeric::interval<float>, 100>;
#endif

#ifdef USE_DOUBLE
template struct bench_1<boost::numeric::interval<double>, 3>;
template struct bench_1<boost::numeric::interval<double>, 10>;
template struct bench_1<boost::numeric::interval<double>, 30>;
template struct bench_1<boost::numeric::interval<double>, 100>;
#endif

#ifdef USE_BOOST_COMPLEX
#ifdef USE_FLOAT
template struct bench_1<boost::complex<boost::numeric::interval<float> >, 3>;
template struct bench_1<boost::complex<boost::numeric::interval<float> >, 10>;
template struct bench_1<boost::complex<boost::numeric::interval<float> >, 30>;
template struct bench_1<boost::complex<boost::numeric::interval<float> >, 100>;
#endif

#ifdef USE_DOUBLE
template struct bench_1<boost::complex<boost::numeric::interval<double> >, 3>;
template struct bench_1<boost::complex<boost::numeric::interval<double> >, 10>;
template struct bench_1<boost::complex<boost::numeric::interval<double> >, 30>;
template struct bench_1<boost::complex<boost::numeric::interval<double> >, 100>;
#endif
#endif


