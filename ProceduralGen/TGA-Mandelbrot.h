#pragma once
#include <complex>
#include "TGA-Image.h"

class Mandelbrot : public TGA::Image
{
private:
	double minReal, maxReal;
	double minImg, maxImg;

	static const int MAX_ITERATIONS = 75;
	
public:
	Mandelbrot(const short& width, const short& height, const string& name,
		       const double& minReal = -2.0, const double& maxReal = 1,
			   const double& minImg  = -1.2 //no maxImg to preserve aspect ratio
		       );

	void gennerate();
	
	static inline complex<double> mandelFunc(const complex<double>& z, const complex<double>& c) { return z*z + c; }

private:

	int numOfIterations(const short& x, const short& y);
	complex<double> startingComplexFor(const short& x, const short& y);



};
