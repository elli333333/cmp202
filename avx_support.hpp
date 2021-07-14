//
// Created by smacdonald on 22/04/2021.
//

#ifndef MANDELBROT_AVX_SUPPORT_HPP
#define MANDELBROT_AVX_SUPPORT_HPP

#endif //MANDELBROT_AVX_SUPPORT_HPP

#include <immintrin.h>

template<typename T>
class vec_complex {
public:
    T real;
    T imag;
};


__m256d vec_complex_abs_pd(vec_complex<__m256d> a) {
    return _mm256_sqrt_pd(_mm256_add_pd(_mm256_mul_pd(a.real, a.real), _mm256_mul_pd(a.imag, a.imag)));
}


vec_complex<__m256d> vec_complex_mul_pd(vec_complex<__m256d> a, vec_complex<__m256d> b) {
    vec_complex<__m256d> out{};

    out.real = _mm256_mul_pd(a.real, b.real);
    out.imag = _mm256_add_pd(_mm256_mul_pd(a.imag, b.real), _mm256_mul_pd(a.imag, b.imag));


    return out;
}

vec_complex<__m256d> vec_complex_mul_pd(vec_complex<__m256d> a) {
    vec_complex<__m256d> out{};

    out.real = _mm256_mul_pd(a.real, a.real);
    out.imag = _mm256_add_pd(_mm256_mul_pd(a.imag, a.real), _mm256_mul_pd(a.imag, a.imag));


    return out;
}

vec_complex<__m256d> vec_complex_blendv(vec_complex<__m256d> a, vec_complex<__m256d> b, __m256 mask) {
    vec_complex<__m256d> out{};

    out.real = _mm256_blendv_pd(a.real, b.real, mask);
    out.imag = _mm256_blendv_pd(a.imag, b.imag, mask);

    return out;
}

vec_complex<__m256d> vec_complex_add_pd(vec_complex<__m256d> a, vec_complex<__m256d> b) {
    vec_complex<__m256d> out{};

    out.real = _mm256_add_pd(a.real, b.real);
    out.imag = _mm256_add_pd(a.imag, b.imag);

    return out;
}

__m256i mm256_set_all_epi64(int a) {
    return _mm256_set_epi64x(a, a, a, a);
}

__m256d mm256_set_all_pd(double a) {
    return _mm256_set_pd(a, a, a, a);
}