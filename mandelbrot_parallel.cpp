/*
 * Parallel mandelbrot implementation
 */
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <complex>
#include <fstream>
#include <iostream>

#include "thread_pool.hpp"
#include "write_csv.hpp"


void write_tga(const char *filename)
{
    // Write the mandelbrot to a TGA file with the given name.
    // Format specification: http://www.gamers.org/dEngine/quake3/TGA.txt
    // Copied from given example

    std::ofstream outfile(filename, std::ofstream::binary);

    uint8_t header[18] = {
            0, // no mandelbrot ID
            0, // no colour map
            2, // uncompressed 24-bit mandelbrot
            0, 0, 0, 0, 0, // empty colour map specification
            0, 0, // X origin
            0, 0, // Y origin
            static_cast<uint8_t>(image::x_res & 0xFF), static_cast<uint8_t>((image::x_res >> 8) & 0xFF), // width
            static_cast<uint8_t>(image::y_res & 0xFF), static_cast<uint8_t>((image::y_res >> 8) & 0xFF), // height
            24, // bits per pixel
            0, // mandelbrot descriptor
    };
    outfile.write((const char *)header, 18);

    for (auto & y : image::IMAGE)
    {
        for (unsigned int x : y)
        {
            uint8_t pixel[3] = {
                    static_cast<uint8_t>(x & 0xFF), // blue channel
                    static_cast<uint8_t>((x >> 8) & 0xFF), // green channel
                    static_cast<uint8_t>((x >> 16) & 0xFF), // red channel
            };
            outfile.write((const char *)pixel, 3);
        }
    }

    outfile.close();
    if (!outfile)
    {
        // An error has occurred at some point since we opened the file.
        std::cout << "Error writing to " << filename << std::endl;
        exit(1);
    }
}

int main(int argc, char* argv[]) {
    std::cout << "Starting\n";

    std::chrono::high_resolution_clock::time_point sys_start
            = std::chrono::high_resolution_clock::now();

    int runs = 1;
    std::vector<int> times;
    times.reserve(runs);

    mandelbrot mandelbrot_data{};
    mandelbrot_data.top = 0.118378;
    mandelbrot_data.bottom = 0.134488;
    mandelbrot_data.left = -0.751085;
    mandelbrot_data.right = -0.734975;
    mandelbrot_data.max_iter = 1024;

    for (int i = 0; i < runs; ++i) {
        // begin timer
        std::cout << "Beginning run " << i << " of " << runs << std::endl;
        std::chrono::high_resolution_clock::time_point start
                = std::chrono::high_resolution_clock::now();

        ThreadPool compute_pool(mandelbrot_data);

        compute_pool.close_threads();


        // end timer
        std::chrono::high_resolution_clock::time_point end
                = std::chrono::high_resolution_clock::now();
        auto time_taken = std::chrono::duration_cast<std::chrono::milliseconds>(
                end - start).count();
        times.emplace_back( static_cast<int>(time_taken));
        write_tga("output_para.tga");
    }

    std::chrono::high_resolution_clock::time_point sys_end
            = std::chrono::high_resolution_clock::now();

    write_csv(times, mandelbrot_data.max_iter);

    auto time_taken = std::chrono::duration_cast<std::chrono::milliseconds>(
            sys_end - sys_start).count();

    std::cout << "system runtime: " << time_taken << "ms\n";

    return 0;
}
