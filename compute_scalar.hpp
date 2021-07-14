//
// Created by smacdonald on 13/04/2021.
//

#ifndef MANDELBROT_COMPUTE_CHUNK_HPP
#define MANDELBROT_COMPUTE_CHUNK_HPP

#endif //MANDELBROT_COMPUTE_CHUNK_HPP


void compute_scalar(chunk &chunk_data, mandelbrot &mandelbrot_data) {
    for (uint y = chunk_data.min_y; y < chunk_data.max_y; ++y) {
        for (uint x = chunk_data.min_x; x < chunk_data.max_x; ++x) {
            // Work out the point in the complex plane that
            // corresponds to this pixel in the output mandelbrot.
            std::complex<double> c(mandelbrot_data.left + (x * (mandelbrot_data.right - mandelbrot_data.left) / image::x_res),
                                   mandelbrot_data.top + (y * (mandelbrot_data.bottom - mandelbrot_data.top) / image::y_res));

            // Start off z at (0, 0).
            std::complex<double> z(0.0, 0.0);

            // Iterate z = z^2 + c until z moves more than 2 units
            // away from (0, 0), or we've iterated too many times.
            int iterations = 0;
            while (abs(z) < 2.0 && iterations < mandelbrot_data.max_iter) {
                z = (z * z) + c;

                ++iterations;
            }

            image::IMAGE[y][x] = iterations;
        }
    }
}
