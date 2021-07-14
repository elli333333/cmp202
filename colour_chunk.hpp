//
// Created by smacdonald on 20/04/2021.
//

#ifndef MANDELBROT_COLOUR_CHUNK_HPP
#define MANDELBROT_COLOUR_CHUNK_HPP

#endif //MANDELBROT_COLOUR_CHUNK_HPP

uint rgb(double ratio) {
    // derived from https://stackoverflow.com/a/40639903/12454856
    int normalized = int(ratio * 256 * 6);

    int x = normalized % 256;

    int red = 0;
    int green = 0;
    int blue = 0;

    switch (normalized / 256) {
        case 0:
            red = 255;
            green = x;
            blue = 0;
            break;
        case 1:
            red = 255 - x;
            green = 255;
            blue = 0;
            break;
        case 2:
            red = 0;
            green = 255;
            blue = x;
            break;
        case 3:
            red = 0;
            green = 255 - x;
            blue = 255;
            break;
        case 4:
            red = x;
            green = 0;
            blue = 255;
            break;
        case 5:
            red = 255;
            green = 0;
            blue = 255 - x;
            break;
    }

    return red + (green << 8) + (blue << 16);
}

void colour_chunk(chunk &chunk_boundaries, mandelbrot &image_params) {
    for (uint y = chunk_boundaries.min_y; y < chunk_boundaries.max_y; ++y) {
        for (uint x = chunk_boundaries.min_x; x < chunk_boundaries.max_x; ++x) {
            if (image::IMAGE[y][x] == image_params.max_iter) {
                // z didn't escape from the circle.
                // This point is in the Mandelbrot set.
                image::IMAGE[y][x] = 0x000000; // black
            } else {
                // z escaped within less than MAX_ITER
                // iterations. This point isn't in the set.
                image::IMAGE[y][x] = rgb(double(image::IMAGE[y][x]) / image_params.max_iter);
            }
        }
    }
}
