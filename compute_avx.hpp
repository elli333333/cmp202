//
// Created by smacdonald on 23/04/2021.
//

#ifndef MANDELBROT_COMPUTE_AVX_HPP
#define MANDELBROT_COMPUTE_AVX_HPP

#endif //MANDELBROT_COMPUTE_AVX_HPP

#include <immintrin.h>
#include <cmath>
#include "avx_support.hpp"

void compute_avx(chunk &chunk_data, mandelbrot &mandelbrot_data) {
    /*
     * the other mandelbrot compute function, only with avx
     * i dont know why im doing this
     * this doesnt work and i dont know why...
     * it only draws a diagonal line
     */
    for (uint i = chunk_data.min_y; i < chunk_data.max_y; ++i) {
        for (uint ii = chunk_data.min_x; ii < chunk_data.max_x; ii+=4) {

            __m256i x = _mm256_set_epi64x((chunk_data.min_x + 3),
                                          (chunk_data.min_x + 2),
                                          (chunk_data.min_x + 1),
                                          chunk_data.min_x);
            __m256i y = _mm256_set_epi64x((i + 3), (i + 2), (i + 1), i);

            __m256d left = mm256_set_all_pd(mandelbrot_data.left);
            __m256d right = mm256_set_all_pd(mandelbrot_data.right);
            __m256d top = mm256_set_all_pd(mandelbrot_data.top);
            __m256d bottom = mm256_set_all_pd(mandelbrot_data.bottom);

            __m256i x_res = mm256_set_all_epi64(image::x_res);
            __m256i y_res = mm256_set_all_epi64(image::y_res);

            vec_complex<__m256d> z{};
            z.real = _mm256_setzero_pd();
            z.imag = _mm256_setzero_pd();

            vec_complex<__m256d> c{};
            c.real = _mm256_add_pd(left, _mm256_div_pd(
                    _mm256_mul_pd(x, _mm256_sub_pd(right, left)), x_res));
            c.imag = _mm256_add_pd(top, _mm256_div_pd(
                    _mm256_mul_pd(y, _mm256_sub_pd(bottom, top)), y_res));

            __m256d iterations = _mm256_setzero_pd();
            __m256d max_iterations = mm256_set_all_pd(
                    mandelbrot_data.max_iter);
            __m256d msk_it = _mm256_setzero_pd();
            __m256d ones = mm256_set_all_pd(1);
            __m256d msk_z;
            __m256d comp = mm256_set_all_pd(2.0);

            while (true) {
                int br = 0;
                msk_z = _mm256_cmp_pd(vec_complex_abs_pd(z), comp, 0x0d);
                z = vec_complex_blendv(
                        vec_complex_add_pd(vec_complex_mul_pd(z), c), z,
                        msk_z);

                for (int iii = 0; iii < 4; ++iii) {
                    if (std::isnan(msk_z[iii])) {
                        msk_it[iii] = std::numeric_limits<double>::quiet_NaN();
                    }
                }

                msk_it = _mm256_cmpeq_epi64(iterations, max_iterations);
                iterations = _mm256_blendv_pd(_mm256_add_pd(iterations, ones),
                                              iterations, msk_it);
                for (int iii = 0; iii < 4; ++iii) {
                    if (std::isnan(msk_it[iii])) {
                        ++br;
                    }
                }
                if (br == 4) { break; }
            }
            for (int j = 0; j < 4; ++j) {
                image::IMAGE[i][(i + j)] = iterations[j];
            }
        }
    }
}