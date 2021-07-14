//
// Created by smacdonald on 12/04/2021.
//

#include <cstdint>
#include <vector>
#include <functional>

#ifndef MANDELBROT_TYPE_DEFS_HPP
#define MANDELBROT_TYPE_DEFS_HPP

#endif //MANDELBROT_TYPE_DEFS_HPP

struct image {
    static int x_res;
    static int y_res;
    static std::vector<std::vector<uint32_t>> IMAGE;
};

struct mandelbrot {
    double left;
    double right;
    double top;
    double bottom;
    int max_iter;
};

struct chunk {
    uint min_x;
    uint max_x;
    uint min_y;
    uint max_y;
};

typedef std::function<void(chunk&, mandelbrot&)> mandelbrotFN;
typedef std::pair<mandelbrotFN, chunk> mandelbrotPair;
