#pragma once

#define DEBUG false

#include <QImage>
#include <math.h>
#include "complex.h"
#include "Array1D.h"
#include "Array2D.h"
#include "FFT.h"
#include "DCT.h"
#include <QColor>
#include <QFile>

enum Channels { Red, Green, Blue };

class ImageOperations
{
public:
	ImageOperations(QImage image);
	~ImageOperations();
	QImage FFTPowerSpectrum(Channels forColor);
	QImage FFTConvolution(QImage mask);
	QImage DCTPowerSpectrum(Channels forColor);
	QImage DCTConvolution(QImage mask);

	static int nextPowerOfTwo(int currentDimension);
	static bool isAPowerOfTwo(unsigned int x);
	static int firstPowerOfTwoOfLargerDimension(int width, int height);

private:
	QImage image;

	void cImageToArray2D(cArray2D *pArray, Channels forColor);
	int getIntensity(QRgb color, Channels forColor);
	int getMeanValue(Channels forColor);
	void cExecuteFFTOnArray2D(cArray2D *pArray);
	void cPowerSpectrumToRealPart(cArray2D *pArray);
	void cScaleRealPartLogarithmically(cArray2D *pArray, double min, double max);
	void cSwapBlocks(cArray2D *pArray);
	QImage cRealPartToImage(cArray2D *pArray);
	void cDebugToFile(QString fileName, cArray2D *pArray, int height, int width);
	void cRealIntensityCrop(cArray2D *pArray);
	QImage cRealPartToImageWithCrop(cArray2D *pArrayRed, cArray2D *pArrayGreen, cArray2D *pArrayBlue);
	void cMaskToArray2D(cArray2D *pArray, QImage mask, Channels forColor);
	void cPointwiseProduct(cArray2D *pArrayA, cArray2D *pArrayB);
	void cExecuteIFFTOnArray2D(cArray2D *pArray);
	void cScaleRealPartLinearly(cArray2D *pArray, double min, double max);

	void dImageToArray2D(dArray2D *pArray, Channels forColor);
	void dExecuteDCTOnArray2D(dArray2D *pArray);
	void dExecuteIDCTOnArray2D(dArray2D *pArray);
	void dPowerSpectrum(dArray2D *pArray);
	void dScaleLogarithmically(dArray2D *pArray, double min, double max);
	dArray2D DCTPowerSpectrumArray(Channels forColor);
	QImage DCTPowerSpectrumQImage(dArray2D array2D);

	QImage dArrayToImage(dArray2D *pArray);
	void dSwapBlocks(dArray2D *pArray);
	void dIntensityRound(dArray2D *pArray);
	void dIntensityCrop(dArray2D *pArray);
	QImage dImageWithCrop(dArray2D *pArrayRed, dArray2D *pArrayGreen, dArray2D *pArrayBlue);
	void dMaskToArray2D(dArray2D *pArray, QImage mask, Channels forColor);
	void dPointwiseProduct(dArray2D *pArrayA, dArray2D *pArrayB);
	void dDebugToFile(QString fileName, dArray2D *pArray, int height, int width);
	double round(double x);
};
