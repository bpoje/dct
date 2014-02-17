#pragma once

#include "FFT.h"
#include "complex.h"
#include <math.h>
#include "Array1D.h"

#define pi 3.14159265358979323846

class DCT
{
public:
	static bool Forward(dArray1D *pArray);
	static bool Inverse(dArray1D *pArray);
};
