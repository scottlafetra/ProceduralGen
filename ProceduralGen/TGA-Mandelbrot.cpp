#include "TGA-Mandelbrot.h"

#include <iostream>
using namespace std;

Mandelbrot::Mandelbrot(const short& width, const short& height, const string& name,
				   	   const double& minReal, const double& maxReal,
					   const double& minImg //no maxImg to preserve aspect ratio
	)
	:TGA::Image(width, height, name), minReal(minReal), maxReal(maxReal), 
									  minImg(minImg),   maxImg(minImg + ((maxReal - minReal)*height) / width) 
{
	//center the image vertically
	double mandelHeight = maxImg - minImg;
	Mandelbrot::minImg = -mandelHeight / 2;
	Mandelbrot::maxImg =  mandelHeight / 2;

	gennerate();
}

complex<double> Mandelbrot::startingComplexFor(const short& x, const short& y)
{
	return complex<double>(
							(x * (maxReal - minReal)) / width  + minReal,
							(y * (maxImg  - minImg))  / height + minImg
						  );
}

int Mandelbrot::numOfIterations(const short& x, const short& y)
{
	complex<double> c( startingComplexFor(x, y) );
	complex<double> z(c);

	bool isInside = true;

	int i = 0;
	for (; i < MAX_ITERATIONS; ++i)
	{
		if (z.real()*z.real() + z.imag()*z.imag() > 4)
		{
			isInside = false;
			break;
		}

		z = mandelFunc(z, c);
	}

	if (isInside)
	{
		return -1;
	}
	else
	{
		return i;
	}
}

void Mandelbrot::gennerate()
{
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			int iterations = numOfIterations(x, y);

			if (iterations == -1)
			{
				setPixel(x, y, TGA_COLOR_BLACK);
			}
			else
			{
				uint8_t value = (iterations * 0xff) / MAX_ITERATIONS;
				setPixel(x, y, TGA::Pixel{ 0, 0, value });
			}
			
		}
		cout << y * 100 / (float)height << "%" << endl;
	}
}