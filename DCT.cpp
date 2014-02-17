#include "DCT.h"

bool DCT::Forward(dArray1D *pArray)
{
	int N = pArray->getLength();

	// Check input parameters
	if (!pArray || N < 1 || N & (N - 1))
		return false;

	cArray1D cData(2*N);

	for (int i = 0; i < N; i++)
	{
		cData.setAt(N - i - 1, pArray->getAt(i));
		cData.setAt(N + i, pArray->getAt(i));
	}

	FFT::Forward(&cData);

	double eksponent = 2 * pi * (N - 0.5) / (2 * N);
	double co = cos(eksponent);
	double si = sin(eksponent);
	complex shift = 1;
	for (int i = 0; i < 2 * N; i++)
	{
		cData.setAt(i, cData.getAt(i) * shift);
		shift = shift * complex(co,si);
	}

	for (int i = 0; i < N; i++)
		pArray->setAt(i, cData.getAt(i).re() / sqrt((double)2 * N));

	pArray->setAt(0, pArray->getAt(0) / sqrt((double)2));

	return true;
}

bool DCT::Inverse(dArray1D *pArray)
{
	int N = pArray->getLength();

	// Check input parameters
	if (!pArray || N < 1 || N & (N - 1))
		return false;

	pArray->setAt(0, pArray->getAt(0) * sqrt((double)2));

	cArray1D cData(2*N);

	for (int i = 0; i < N; i++)
		pArray->setAt(i, pArray->getAt(i) * sqrt((double)2 * N));

	for (int i = 1; i < N; i++)
	{
		cData.setAt(i, pArray->getAt(i));
		cData.setAt(2*N-i, -pArray->getAt(i));
	}
	cData.setAt(0, pArray->getAt(0));

	double eksponent = 2 * pi * (N - 0.5) / (2 * N);
	double co = cos(eksponent);
	double si = sin(eksponent);
	complex shift = 1;
	for (int i = 0; i < 2 * N; i++)
	{
		cData.setAt(i, cData.getAt(i) * shift);
		shift = shift * complex(co,si);
	}

	FFT::Inverse(&cData, true);

	for (int i = 0; i < N; i++)
	{
		pArray->setAt(i, cData.getAt(N-i).re());
	}

	return true;
}
