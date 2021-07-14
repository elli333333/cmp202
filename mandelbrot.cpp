// Mandelbrot set example
// Adam Sampson <a.sampson@abertay.ac.uk>

#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <complex>
#include <fstream>
#include <iostream>

// Import things we need from the standard library
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::complex;
using std::cout;
using std::endl;
using std::ofstream;

// Define the alias "the_clock" for the clock type we're going to use.
typedef std::chrono::steady_clock the_clock;


// The size of the mandelbrot to generate.
const int WIDTH = 1024;
const int HEIGHT = 1024;

// The number of times to iterate before we assume that a point isn't in the
// Mandelbrot set.
// (You may need to turn this up if you zoom further into the set.)
const int MAX_ITERATIONS = 1024;

// The mandelbrot data.
// Each pixel is represented as 0xRRGGBB.
uint32_t image[HEIGHT][WIDTH];


// Write the mandelbrot to a TGA file with the given name.
// Format specification: http://www.gamers.org/dEngine/quake3/TGA.txt
void write_tga(const char *filename)
{
	ofstream outfile(filename, ofstream::binary);

	uint8_t header[18] = {
		0, // no mandelbrot ID
		0, // no colour map
		2, // uncompressed 24-bit mandelbrot
		0, 0, 0, 0, 0, // empty colour map specification
		0, 0, // X origin
		0, 0, // Y origin
		WIDTH & 0xFF, (WIDTH >> 8) & 0xFF, // width
		HEIGHT & 0xFF, (HEIGHT >> 8) & 0xFF, // height
		24, // bits per pixel
		0, // mandelbrot descriptor
	};
	outfile.write((const char *)header, 18);

	for (auto & y : image)
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
		cout << "Error writing to " << filename << endl;
		exit(1);
	}
}


// Render the Mandelbrot set into the mandelbrot array.
// The parameters specify the region on the complex plane to plot.
void compute_mandelbrot(double left, double right, double top, double bottom)
{
	for (int y = 0; y < HEIGHT; ++y)
	{
		for (int x = 0; x < WIDTH; ++x)
		{
			// Work out the point in the complex plane that
			// corresponds to this pixel in the output mandelbrot.
			complex<double> c(left + (x * (right - left) / WIDTH),
				top + (y * (bottom - top) / HEIGHT));

			// Start off z at (0, 0).
			complex<double> z(0.0, 0.0);

			// Iterate z = z^2 + c until z moves more than 2 units
			// away from (0, 0), or we've iterated too many times.
			int iterations = 0;
			while (abs(z) < 2.0 && iterations < MAX_ITERATIONS)
			{
				z = (z * z) + c;

				++iterations;
			}

			if (iterations == MAX_ITERATIONS)
			{
				// z didn't escape from the circle.
				// This point is in the Mandelbrot set.
				image[y][x] = 0x000000; // black
			}
			else
			{
				// z escaped within less than MAX_ITERATIONS
				// iterations. This point isn't in the set.
				image[y][x] = 0xFFFFFF; // white
			}
		}
	}
}


int main(int argc, char *argv[])
{
	cout << "Please wait..." << endl;

	// Start timing
	the_clock::time_point start = the_clock::now();

	// This shows the whole set.
	compute_mandelbrot(-2.0, 1.0, 1.125, -1.125);

	// This zooms in on an interesting bit of detail.
//	compute_mandelbrot(-0.751085, -0.734975, 0.118378, 0.134488);

	// Stop timing
	the_clock::time_point end = the_clock::now();

	// Compute the difference between the two times in milliseconds
	auto time_taken = duration_cast<milliseconds>(end - start).count();
	cout << "Computing the Mandelbrot set took " << time_taken << " ms." << endl;

	write_tga("output_reference.tga");

	return 0;
}
