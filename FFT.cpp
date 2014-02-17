//   fft.cpp - impelementation of class
//   of fast Fourier transform - FFT
//
//   The code is property of LIBROW
//   You can use it on your own
//   When utilizing credit LIBROW site

#include "FFT.h"

//FORWARD FOURIER TRANSFORM, INPLACE VERSION
//Data - both input data and output
//N    - length of both input data and result
bool FFT::Forward(cArray1D *data)
{
	int N = data->getLength();

	//Check input parameters
	if (!data || N < 1 || N & (N - 1))
		return false;

	//Rearrange
	Rearrange(data);

	//Call FFT implementation
	Perform(data);

	//Succeeded
	return true;
}

//INVERSE FOURIER TRANSFORM, INPLACE VERSION
//Data  - both input data and output
//N     - length of both input data and result
//Scale - if to scale result
bool FFT::Inverse(cArray1D *data, const bool Scale)
{
	int N = data->getLength();

	//Check input parameters
	if (!data || N < 1 || N & (N - 1))
		return false;

	//Rearrange
	Rearrange(data);

	//Call FFT implementation
	Perform(data, true);

	//Scale if necessary
	if (Scale)
		FFT::Scale(data);

	//Succeeded
	return true;
}

//Inplace version of rearrange function
void FFT::Rearrange(cArray1D *data)
{
	unsigned int N = (unsigned)data->getLength();

	//Swap position
	unsigned int Target = 0;

	//Process all positions of input signal
	for (unsigned int Position = 0; Position < (unsigned int)N; ++Position)
	{
		//Only for not yet swapped entries
		if (Target > Position)
		{
			//Swap entries
			complex Temp( data->getAt(Target) );
			data->setAt(Target, data->getAt(Position));
			data->setAt(Position, Temp);
		}

		//Bit mask
		unsigned int Mask = N;

		//While bit is set
		while (Target & (Mask >>= 1))
			//Drop bit
			Target &= ~Mask;

		//The current bit is 0 - set it
		Target |= Mask;
	}
}

//FFT implementation
void FFT::Perform(cArray1D *data, const bool Inverse)
{
	int N = data->getLength();

	const double pi = Inverse ? 3.14159265358979323846 : -3.14159265358979323846;
	//Iteration through dyads, quadruples, octads and so on...
	for (unsigned int Step = 1; Step < (unsigned int)N; Step <<= 1)
	{
		//Jump to the next entry of the same transform factor
		const unsigned int Jump = Step << 1;

		//Angle increment
		const double delta = pi / double(Step);

		//Auxiliary sin(delta / 2)
		const double Sine = sin(delta * .5);

		//Multiplier for trigonometric recurrence
		const complex Multiplier(-2. * Sine * Sine, sin(delta));

		//Start value for transform factor, fi = 0
		complex Factor(1.);

		//Iteration through groups of different transform factor
		for (unsigned int Group = 0; Group < Step; ++Group)
		{
			//Iteration within group
			for (unsigned int Pair = Group; Pair < (unsigned int)N; Pair += Jump)
			{
				//Match position
				const unsigned int Match = Pair + Step;

				//Second term of two-point transform
				//const complex Product(Factor * Data[Match]);
				const complex Product(Factor * data->getAt(Match));

				//Transform for fi + pi
				//Data[Match] = Data[Pair] - Product;
				data->setAt(Match, data->getAt(Pair) - Product);

				//Transform for fi
				//Data[Pair] += Product;
				data->setAt(Pair, data->getAt(Pair) + Product);
			}

			//Successive transform factor via trigonometric recurrence
			Factor = Multiplier * Factor + Factor;
		}
	}
}

//Scaling of inverse FFT result
void FFT::Scale(cArray1D *data)
{
	int N = data->getLength();

	const double Factor = 1. / double(N);

	//Scale all data entries
	for (unsigned int Position = 0; Position < (unsigned int)N; ++Position)
		data->setAt(Position, data->getAt(Position) * Factor);
}
