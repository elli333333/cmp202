//
// Created by smacdonald on 11/05/2021.
//

#ifndef MANDELBROT_WRITE_CSV_HPP
#define MANDELBROT_WRITE_CSV_HPP

#endif //MANDELBROT_WRITE_CSV_HPP

#include <fmt/core.h>
#include <fmt/format.h>

void write_csv(std::vector<int> & values, int & depth) {
    std::ofstream data_file;
    std::string file_name = fmt::format("data_file_{}.csv",image::y_res);
    data_file.open(file_name);

    data_file << "x_res,y_res,iteration depth,time(ms)\n";
    for (auto i : values) {
        data_file << fmt::format("{},{},{},{}\n", image::x_res, image::y_res, depth, i);
    }
    data_file.close();
    return;
}