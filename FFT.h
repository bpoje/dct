//   fft.h - declaration of class
//   of fast Fourier transform - FFT
//
//   The code is property of LIBROW
//   You can use it on your own
//   When utilizing credit LIBROW site

#pragma once

#include <math.h>
#include "complex.h"
#include "Array1D.h"

class FFT
{
public:
	//FORWARD FOURIER TRANSFORM, INPLACE VERSION
	//Data - both input data and output
	static bool Forward(cArray1D *data);

	//INVERSE FOURIER TRANSFORM, INPLACE VERSION
	//Data  - both input data and output
	//Scale - if to scale result
	static bool Inverse(cArray1D *data, const bool Scale = true);

protected:
	//Rearrange function and its inplace version
	static void Rearrange(cArray1D *data);

	//FFT implementation
	static void Perform(cArray1D *data, const bool Inverse = false);

	//Scaling of inverse FFT result
	static void Scale(cArray1D *data);
};
