#include "ImageOperations.h"
#include <stdio.h>
#include <QMessageBox>

ImageOperations::ImageOperations(QImage image)
{
	ImageOperations::image = image;
}

ImageOperations::~ImageOperations()
{
}

QImage ImageOperations::FFTPowerSpectrum(Channels forColor)
{
	int height = image.height();
	int width = image.width();

	if (height == 0 || width == 0)
		return QImage();

	int newHeightAndWidth = firstPowerOfTwoOfLargerDimension(width, height);
	cArray2D array2D(newHeightAndWidth, newHeightAndWidth);

	int mean = getMeanValue(forColor);
	array2D.fill(complex(mean,0));

	cImageToArray2D(&array2D, forColor);

	cExecuteFFTOnArray2D(&array2D);

	cPowerSpectrumToRealPart(&array2D);

	double min = (array2D.findMin()).re();
	double max = (array2D.findMax()).re();
	cScaleRealPartLogarithmically(&array2D, min, max);

	cSwapBlocks(&array2D);

	QImage newImage = cRealPartToImage(&array2D);
	return newImage;
}

void ImageOperations::cImageToArray2D(cArray2D *pArray, Channels forColor)
{
	if (image.format() != QImage::Format_ARGB32)
		image = image.convertToFormat(QImage::Format_ARGB32);

	int newHeight = pArray->getHeight();
	int newWidth = pArray->getWidth();

	int positionY = 0;
	int positionX = 0;
	for (int i=0; i < image.height(); i++)
	{
		QRgb *line = (QRgb*)image.scanLine(i);

		for (int j=0; j < image.width(); j++)
		{
			positionY = i + (newHeight - image.height()) / 2;
			positionX = j + (newWidth - image.width())/2;
			pArray->setAt(positionY, positionX, getIntensity(line[j], forColor));
		}
	}
}

int ImageOperations::getIntensity(QRgb color, Channels forColor)
{
	int intensity = 0;
	switch (forColor)
	{
	case Red:
		intensity = qRed(color);
		break;
	case Green:
		intensity = qGreen(color);
		break;
	case Blue:
		intensity = qBlue(color);
		break;
	}

	return intensity;
}

int ImageOperations::getMeanValue(Channels forColor)
{
	double all = (double)image.height() * (double)image.width();
	double sum = 0;

	for (int i=0; i < image.height(); i++)
	{

		QRgb *line = (QRgb*)image.scanLine(i);

		for (int j=0; j < image.width(); j++)
		{
			sum += (double)getIntensity(line[j], forColor);
		}
	}

	double mean = sum / all;
	int fl = (int)floor(mean);

	return fl;
}

void ImageOperations::cExecuteFFTOnArray2D(cArray2D *pArray)
{
	int newHeight = pArray->getHeight();
	int newWidth = pArray->getWidth();

	cArray1D row(newWidth);

	for (int i=0; i < newHeight; i++)
	{
		for (int j = 0; j < newWidth; j++)
			row.setAt(j, pArray->getAt(i, j));

		FFT::Forward(&row);

		for (int j = 0; j < newWidth; j++)
			pArray->setAt(i, j, row.getAt(j));
	}

	cArray1D column(newHeight);

	for (int j=0; j < newWidth; j++)
	{
		for (int i = 0; i < newHeight; i++)
			column.setAt(i, pArray->getAt(i,j));

		FFT::Forward(&column);

		for (int i = 0; i < newHeight; i++)
			pArray->setAt(i, j, column.getAt(i));

	}
}

void ImageOperations::cPowerSpectrumToRealPart(cArray2D *pArray)
{
	int newHeight = pArray->getHeight();
	int newWidth = pArray->getWidth();

	for (int i = 0; i < newHeight; i++)
		for (int j = 0; j < newWidth; j++)
		{
			double value = pArray->getAt(i, j).power();
			pArray->setAt(i, j, value);
		}
}

void ImageOperations::cScaleRealPartLogarithmically(cArray2D *pArray, double min, double max)
{
	int newHeight = pArray->getHeight();
	int newWidth = pArray->getWidth();

	double A = (double)255 / log(max - min + 1);
	double B = min - 1;

	for (int i = 0; i < newHeight; i++)
		for (int j =0 ; j < newWidth; j++)
		{
			double rezultat = floor( A * log( pArray->getAt(i,j).re() - B));
			pArray->setAt(i, j, rezultat);
		}
}

void ImageOperations::cSwapBlocks(cArray2D *pArray)
{
	int newHeight = pArray->getHeight();
	int newWidth = pArray->getWidth();

	complex temp;
	for (int i =0 ; i < newHeight/2; i++)
	{
		for (int j = 0; j < newWidth/2; j++)
		{
			temp = pArray->getAt(i, j);
			pArray->setAt(i, j, pArray->getAt(i + newHeight/2, j + newWidth/2));
			pArray->setAt(i + newHeight/2, j + newWidth/2, temp);
		}
	}

	for (int i = newHeight/2; i < newHeight; i++)
	{
		for (int j = 0; j < newWidth/2; j++)
		{
			temp = pArray->getAt(i, j);
			pArray->setAt(i, j, pArray->getAt(i - newHeight/2, j + newWidth/2));
			pArray->setAt(i - newHeight/2, j + newWidth/2, temp);
		}
	}
}

QImage ImageOperations::cRealPartToImage(cArray2D *pArray)
{
	int newHeight = pArray->getHeight();
	int newWidth = pArray->getWidth();

	//QImage::QImage ( int width, int height, Format format )
	QImage newImage(newWidth, newHeight, QImage::Format_RGB32);

	for (int i = 0; i < newHeight; i++)
	{
		QRgb *line = (QRgb*)newImage.scanLine(i);

		for (int j = 0; j < newWidth; j++)
		{
			int col = (int)pArray->getAt(i,j).re();

			//QColor ( int r, int g, int b, int a = 255 )
			QColor color( col, col, col, 255 );

			//QRgb QColor::rgba () const
			line[j] = color.rgb();
		}
	}

	return newImage;
}

int ImageOperations::nextPowerOfTwo(int currentDimension)
{
	double potenca = (int)ceil( log((double)currentDimension)/log((double)2) );
	return (int)pow( 2, potenca );
}

bool ImageOperations::isAPowerOfTwo(unsigned int x)
{
	return ((x != 0) && ((x & (~x + 1)) == x));
}

int ImageOperations::firstPowerOfTwoOfLargerDimension(int width, int height)
{
	int newHeightAndWidth = 0;
	if (height > width)
	{
		if (!isAPowerOfTwo(height))
			height = nextPowerOfTwo( height );

		newHeightAndWidth = height;
	}
	else
	{
		if (!isAPowerOfTwo(width))
			width = nextPowerOfTwo( width );

		newHeightAndWidth = width;
	}

	return newHeightAndWidth;
}

void ImageOperations::cDebugToFile(QString fileName, cArray2D *pArray, int height, int width)
{
	if (DEBUG)
	{
		QString temp;
		fileName += " " + temp.setNum( height, 10 ) + "x" + temp.setNum( width, 10 ) + ".txt";
		QFile file( fileName );
		file.open(QIODevice::ReadWrite);

		QString re;
		QString im;
		QString msg;

		for (int i=0; i < height; i++)
		{
			//QString & QString::setNum ( int n, int base = 10 )
			temp.setNum( i, 10 );
			temp = "Row: " + temp + "\n";
			file.write(temp.toAscii(), temp.length());

			for (int j=0; j < width; j++)
			{
				//QString & QString::setNum ( double n, char format = 'g', int precision = 6 )
				re.setNum( pArray->getAt(i,j).re(), 'g', 6);
				im.setNum( pArray->getAt(i,j).im(), 'g', 6);

				msg = "(" + re + ", " + im + ")\n";
				file.write(msg.toAscii(), msg.length());
			}

			file.write("\n", qstrlen("\n"));
		}

		file.close();
	}
}

QImage ImageOperations::FFTConvolution(QImage mask)
{
	int height = image.height();
	int width = image.width();

	int maskHeight = mask.height();
	int maskWidth = mask.width();

	if (height == 0 || width == 0 || maskHeight == 0 || maskWidth == 0)
		return QImage();

	int newHeightAndWidth = firstPowerOfTwoOfLargerDimension(width, height);

	if (newHeightAndWidth != maskHeight || newHeightAndWidth != maskWidth)
		return QImage();

	//Load image
	cArray2D array2DRed(newHeightAndWidth, newHeightAndWidth);
	cArray2D array2DGreen(newHeightAndWidth, newHeightAndWidth);
	cArray2D array2DBlue(newHeightAndWidth, newHeightAndWidth);

	int meanRed = getMeanValue(Red);
	array2DRed.fill(complex(meanRed,0));
	int meanGreen = getMeanValue(Green);
	array2DGreen.fill(complex(meanGreen,0));
	int meanBlue = getMeanValue(Blue);
	array2DBlue.fill(complex(meanBlue,0));

	cImageToArray2D(&array2DRed, Red);
	cImageToArray2D(&array2DGreen, Green);
	cImageToArray2D(&array2DBlue, Blue);

	//Load mask
	cArray2D array2DMaskRed(newHeightAndWidth, newHeightAndWidth);
	cArray2D array2DMaskGreen(newHeightAndWidth, newHeightAndWidth);
	cArray2D array2DMaskBlue(newHeightAndWidth, newHeightAndWidth);

	cMaskToArray2D(&array2DMaskRed, mask, Red);
	cMaskToArray2D(&array2DMaskGreen, mask, Green);
	cMaskToArray2D(&array2DMaskBlue, mask, Blue);

	//FFT on image
	cExecuteFFTOnArray2D(&array2DRed);
	cExecuteFFTOnArray2D(&array2DGreen);
	cExecuteFFTOnArray2D(&array2DBlue);

	cSwapBlocks(&array2DRed);
	cSwapBlocks(&array2DGreen);
	cSwapBlocks(&array2DBlue);

	cPointwiseProduct(&array2DRed, &array2DMaskRed);
	cPointwiseProduct(&array2DGreen, &array2DMaskGreen);
	cPointwiseProduct(&array2DBlue, &array2DMaskBlue);

	cSwapBlocks(&array2DRed);
	cSwapBlocks(&array2DGreen);
	cSwapBlocks(&array2DBlue);

	cExecuteIFFTOnArray2D(&array2DRed);
	cExecuteIFFTOnArray2D(&array2DGreen);
	cExecuteIFFTOnArray2D(&array2DBlue);

	/*Linear scale
	double minRed = (array2DRed.findMin()).re();
	double maxRed = (array2DRed.findMax()).re();
	double minGreen = (array2DGreen.findMin()).re();
	double maxGreen = (array2DGreen.findMax()).re();
	double minBlue = (array2DBlue.findMin()).re();
	double maxBlue = (array2DBlue.findMax()).re();

	cScaleRealPartLinearly(&array2DRed, minRed, maxRed);
	cScaleRealPartLinearly(&array2DGreen, minGreen, maxGreen);
	cScaleRealPartLinearly(&array2DBlue, minBlue, maxBlue);
	*/

	cRealIntensityCrop(&array2DRed);
	cRealIntensityCrop(&array2DGreen);
	cRealIntensityCrop(&array2DBlue);

	QImage newImage = cRealPartToImageWithCrop(&array2DRed, &array2DGreen, &array2DBlue);

	return newImage;
}

void ImageOperations::cRealIntensityCrop(cArray2D *pArray)
{
	for (int i = 0; i < pArray->getHeight(); i++)
		for (int j = 0; j < pArray->getWidth(); j++)
			if (pArray->getAt(i,j).re() > (double)255)
				pArray->setAt(i, j, (double)255);
			else if (pArray->getAt(i,j).re() < (double)0)
				pArray->setAt(i, j, (double)0);
}

QImage ImageOperations::cRealPartToImageWithCrop(cArray2D *pArrayRed, cArray2D *pArrayGreen, cArray2D *pArrayBlue)
{
	int newHeight = pArrayRed->getHeight();
	int newWidth = pArrayRed->getWidth();

	//QImage::QImage ( int width, int height, Format format )
	QImage newImage(image.width(), image.height(), QImage::Format_RGB32);

	for (int i = 0; i < image.height(); i++)
	{
		QRgb *line = (QRgb*)newImage.scanLine(i);

		for (int j = 0; j < image.width(); j++)
		{
			int red = (int)pArrayRed->getAt(i + (newHeight - image.height())/2, j + (newWidth - image.width())/2).re();
			int green = (int)pArrayGreen->getAt(i + (newHeight - image.height())/2, j + (newWidth - image.width())/2).re();
			int blue = (int)pArrayBlue->getAt(i + (newHeight - image.height())/2, j + (newWidth - image.width())/2).re();

			//QColor ( int r, int g, int b, int a = 255 )
			QColor color( red, green, blue, 255 );

			//QRgb QColor::rgba () const
			line[j] = color.rgb();
		}
	}

	return newImage;
}

void ImageOperations::cMaskToArray2D(cArray2D *pArray, QImage mask, Channels forColor)
{
	if (mask.format() != QImage::Format_ARGB32)
		mask = mask.convertToFormat(QImage::Format_ARGB32);

	for (int i=0; i < mask.height(); i++)
	{
		QRgb *line = (QRgb*)mask.scanLine(i);

		for (int j=0; j < mask.width(); j++)
		{
			pArray->setAt(i, j, getIntensity(line[j], forColor) / (double)255);
		}
	}
}

void ImageOperations::cPointwiseProduct(cArray2D *pArrayA, cArray2D *pArrayB)
{
	for (int i = 0; i < pArrayA->getHeight(); i++)
		for (int j = 0; j < pArrayA->getWidth(); j++)
		{
			complex a = pArrayA->getAt(i,j);
			complex b = pArrayB->getAt(i,j);
			pArrayA->setAt(i,j, a * b);
		}
}

void ImageOperations::cExecuteIFFTOnArray2D(cArray2D *pArray)
{
	int newHeight = pArray->getHeight();
	int newWidth = pArray->getWidth();

	cArray1D row(newWidth);

	for (int i=0; i < newHeight; i++)
	{
		for (int j = 0; j < newWidth; j++)
			row.setAt(j, pArray->getAt(i, j));

		FFT::Inverse(&row);

		for (int j = 0; j < newWidth; j++)
			pArray->setAt(i, j, row.getAt(j));
	}

	cArray1D column(newHeight);

	for (int j=0; j < newWidth; j++)
	{
		for (int i = 0; i < newHeight; i++)
			column.setAt(i, pArray->getAt(i,j));

		FFT::Inverse(&column);

		for (int i = 0; i < newHeight; i++)
			pArray->setAt(i, j, column.getAt(i));

	}
}

void ImageOperations::cScaleRealPartLinearly(cArray2D *pArray, double min, double max)
{
	int newHeight = pArray->getHeight();
	int newWidth = pArray->getWidth();

	double k = (double)255 / (max - min);
	double n = (double)255 * min / (min - max);

	for (int i = 0; i < newHeight; i++)
		for (int j =0 ; j < newWidth; j++)
		{
			double rezultat = round( k * pArray->getAt(i,j).re() + n );
			pArray->setAt(i, j, rezultat);
		}
}

double ImageOperations::round(double x)
{
	double fl = floor(x);
	double decimals = x - fl;

	if(decimals >= 0.5)
		return ceil(x);
	else
		return floor(x);
}

QImage ImageOperations::DCTPowerSpectrum(Channels forColor)
{
	dArray2D array2D = DCTPowerSpectrumArray(forColor);
	if (array2D.getHeight() == 0 || array2D.getWidth() == 0)
		return QImage();

	dPowerSpectrum(&array2D);
	QImage newImage = DCTPowerSpectrumQImage(array2D);
	return newImage;
}

dArray2D ImageOperations::DCTPowerSpectrumArray(Channels forColor)
{
	int height = image.height();
	int width = image.width();

	if (height == 0 || width == 0)
		return dArray2D(0,0);

	int newHeightAndWidth = firstPowerOfTwoOfLargerDimension(width, height);
	dArray2D array2D(newHeightAndWidth, newHeightAndWidth);

	int mean = getMeanValue(forColor);
	array2D.fill(mean);

	dImageToArray2D(&array2D, forColor);

	//dDebugToFile("D_Image.txt", &array2D, newHeight, newWidth);

	dExecuteDCTOnArray2D(&array2D);

	//dDebugToFile("D_DCT.txt", &array2D, newHeightAndWidth, newHeightAndWidth);

	return array2D;
}

QImage ImageOperations::DCTPowerSpectrumQImage(dArray2D array2D)
{
	double min = array2D.findMin();
	double max = array2D.findMax();
	//printf("min: %f, max: %f\n", min, max);

	dScaleLogarithmically(&array2D, min, max);

	//dDebugToFile("D_Scaled.txt", &array2D, newHeight, newWidth);

	QImage newImage = dArrayToImage(&array2D);

	return newImage;
}

QImage ImageOperations::DCTConvolution(QImage mask)
{
	int height = image.height();
	int width = image.width();

	int maskHeight = mask.height();
	int maskWidth = mask.width();

	if (height == 0 || width == 0 || maskHeight == 0 || maskWidth == 0)
		return QImage();

	int newHeightAndWidth = firstPowerOfTwoOfLargerDimension(width, height);

	if (newHeightAndWidth != maskHeight || newHeightAndWidth != maskWidth)
		return QImage();

	//Load image
	dArray2D array2DRed(newHeightAndWidth, newHeightAndWidth);
	dArray2D array2DGreen(newHeightAndWidth, newHeightAndWidth);
	dArray2D array2DBlue(newHeightAndWidth, newHeightAndWidth);

	int meanRed = getMeanValue(Red);
	array2DRed.fill(meanRed);
	int meanGreen = getMeanValue(Green);
	array2DGreen.fill(meanGreen);
	int meanBlue = getMeanValue(Blue);
	array2DBlue.fill(meanBlue);

	dImageToArray2D(&array2DRed, Red);
	dImageToArray2D(&array2DGreen, Green);
	dImageToArray2D(&array2DBlue, Blue);

	//Load mask
	dArray2D array2DMaskRed(newHeightAndWidth, newHeightAndWidth);
	dArray2D array2DMaskGreen(newHeightAndWidth, newHeightAndWidth);
	dArray2D array2DMaskBlue(newHeightAndWidth, newHeightAndWidth);

	dMaskToArray2D(&array2DMaskRed, mask, Red);
	dMaskToArray2D(&array2DMaskGreen, mask, Green);
	dMaskToArray2D(&array2DMaskBlue, mask, Blue);

	//DCT on image
	dExecuteDCTOnArray2D(&array2DRed);
	dExecuteDCTOnArray2D(&array2DGreen);
	dExecuteDCTOnArray2D(&array2DBlue);

	//Pointwise product
	dPointwiseProduct(&array2DRed, &array2DMaskRed);
	dPointwiseProduct(&array2DGreen, &array2DMaskGreen);
	dPointwiseProduct(&array2DBlue, &array2DMaskBlue);

	//Swap blocks?

	//IDCT on image
	dExecuteIDCTOnArray2D(&array2DRed);
	dExecuteIDCTOnArray2D(&array2DGreen);
	dExecuteIDCTOnArray2D(&array2DBlue);

	dIntensityCrop(&array2DRed);
	dIntensityCrop(&array2DGreen);
	dIntensityCrop(&array2DBlue);

	QImage newImage = dImageWithCrop(&array2DRed, &array2DGreen, &array2DBlue);
	return newImage;
}

void ImageOperations::dImageToArray2D(dArray2D *pArray, Channels forColor)
{
	/*
	//Repeat image
	if (image.format() != QImage::Format_ARGB32)
		image = image.convertToFormat(QImage::Format_ARGB32);

	int height = image.height();
	int width = image.width();

	int newHeight = pArray->getHeight();
	int newWidth = pArray->getWidth();

	for (int i=0; i < height; i++)
	{
		QRgb *line = (QRgb*)image.scanLine(i);

		for (int j=0; j < width; j++)
		{
			pArray->setAt(i, j, getIntensity(line[j], forColor));

			if (j + width < newWidth)
			{
				pArray->setAt(i, j+width, getIntensity(line[j], forColor));
			}
			if (i + height < newHeight)
			{
				pArray->setAt(i+height, j, getIntensity(line[j], forColor));
			}
			if (j + width < newWidth && i + height < newHeight)
			{
				pArray->setAt(i+height, j+width, getIntensity(line[j], forColor));
			}
		}
	}
	*/

	//Image in the middle
	if (image.format() != QImage::Format_ARGB32)
		image = image.convertToFormat(QImage::Format_ARGB32);

	int newHeight = pArray->getHeight();
	int newWidth = pArray->getWidth();

	int positionY = 0;
	int positionX = 0;
	for (int i=0; i < image.height(); i++)
	{
		QRgb *line = (QRgb*)image.scanLine(i);

		for (int j=0; j < image.width(); j++)
		{
			positionY = i + (newHeight - image.height()) / 2;
			positionX = j + (newWidth - image.width())/2;
			pArray->setAt(positionY, positionX, getIntensity(line[j], forColor));
		}
	}

}

void ImageOperations::dExecuteDCTOnArray2D(dArray2D *pArray)
{
	int newHeight = pArray->getHeight();
	int newWidth = pArray->getWidth();

	dArray1D row(newWidth);

	for (int i=0; i < newHeight; i++)
	{
		for (int j = 0; j < newWidth; j++)
			row.setAt(j, pArray->getAt(i, j));

		DCT::Forward(&row);

		for (int j = 0; j < newWidth; j++)
			pArray->setAt(i, j, row.getAt(j));
	}

	dArray1D column(newHeight);

	for (int j=0; j < newWidth; j++)
	{
		for (int i = 0; i < newHeight; i++)
			column.setAt(i, pArray->getAt(i,j));

		DCT::Forward(&column);

		for (int i = 0; i < newHeight; i++)
			pArray->setAt(i, j, column.getAt(i));

	}
}

void ImageOperations::dExecuteIDCTOnArray2D(dArray2D *pArray)
{
	int newHeight = pArray->getHeight();
	int newWidth = pArray->getWidth();

	dArray1D row(newWidth);

	for (int i=0; i < newHeight; i++)
	{
		for (int j = 0; j < newWidth; j++)
			row.setAt(j, pArray->getAt(i, j));

		DCT::Inverse(&row);

		for (int j = 0; j < newWidth; j++)
			pArray->setAt(i, j, row.getAt(j));
	}

	dArray1D column(newHeight);

	for (int j=0; j < newWidth; j++)
	{
		for (int i = 0; i < newHeight; i++)
			column.setAt(i, pArray->getAt(i,j));

		DCT::Inverse(&column);

		for (int i = 0; i < newHeight; i++)
			pArray->setAt(i, j, column.getAt(i));

	}
}

void ImageOperations::dPowerSpectrum(dArray2D *pArray)
{
	int newHeight = pArray->getHeight();
	int newWidth = pArray->getWidth();

	for (int i = 0; i < newHeight; i++)
		for (int j = 0; j < newWidth; j++)
		{
			double value = pow(pArray->getAt(i, j), 2);
			pArray->setAt(i, j, value);
		}
}

void ImageOperations::dScaleLogarithmically(dArray2D *pArray, double min, double max)
{
	int newHeight = pArray->getHeight();
	int newWidth = pArray->getWidth();

	double A = (double)255 / log(max - min + 1);
	double B = min - 1;

	for (int i = 0; i < newHeight; i++)
		for (int j =0 ; j < newWidth; j++)
		{
			double rezultat = floor( A * log( pArray->getAt(i,j) - B));
			pArray->setAt(i, j, rezultat);
		}
}

QImage ImageOperations::dArrayToImage(dArray2D *pArray)
{
	int newHeight = pArray->getHeight();
	int newWidth = pArray->getWidth();

	QImage newImage(newWidth, newHeight, QImage::Format_RGB32);

	for (int i = 0; i < newHeight; i++)
	{
		QRgb *line = (QRgb*)newImage.scanLine(i);

		for (int j = 0; j < newWidth; j++)
		{
			int col = (int)pArray->getAt(i,j);

			QColor color( col, col, col, 255 );

			line[j] = color.rgb();
		}
	}

	return newImage;
}

void ImageOperations::dSwapBlocks(dArray2D *pArray)
{
	int newHeight = pArray->getHeight();
	int newWidth = pArray->getWidth();

	double temp;
	for (int i =0 ; i < newHeight/2; i++)
	{
		for (int j = 0; j < newWidth/2; j++)
		{
			temp = pArray->getAt(i, j);
			pArray->setAt(i, j, pArray->getAt(i + newHeight/2, j + newWidth/2));
			pArray->setAt(i + newHeight/2, j + newWidth/2, temp);
		}
	}

	for (int i = newHeight/2; i < newHeight; i++)
	{
		for (int j = 0; j < newWidth/2; j++)
		{
			temp = pArray->getAt(i, j);
			pArray->setAt(i, j, pArray->getAt(i - newHeight/2, j + newWidth/2));
			pArray->setAt(i - newHeight/2, j + newWidth/2, temp);
		}
	}
}

void ImageOperations::dIntensityRound(dArray2D *pArray)
{
	for (int i = 0; i < pArray->getHeight(); i++)
		for (int j = 0; j < pArray->getWidth(); j++)
			pArray->setAt(i,j, round(pArray->getAt(i,j)));
}

void ImageOperations::dIntensityCrop(dArray2D *pArray)
{
	for (int i = 0; i < pArray->getHeight(); i++)
		for (int j = 0; j < pArray->getWidth(); j++)
			if (pArray->getAt(i,j) > (double)255)
				pArray->setAt(i, j, (double)255);
			else if (pArray->getAt(i,j) < (double)0)
				pArray->setAt(i, j, (double)0);
}

QImage ImageOperations::dImageWithCrop(dArray2D *pArrayRed, dArray2D *pArrayGreen, dArray2D *pArrayBlue)
{
	int newHeight = pArrayRed->getHeight();
	int newWidth = pArrayRed->getWidth();

	QImage newImage(image.width(), image.height(), QImage::Format_RGB32);

	for (int i = 0; i < image.height(); i++)
	{
		QRgb *line = (QRgb*)newImage.scanLine(i);

		for (int j = 0; j < image.width(); j++)
		{
			int red = (int)pArrayRed->getAt(i + (newHeight - image.height())/2, j + (newWidth - image.width())/2);
			int green = (int)pArrayGreen->getAt(i + (newHeight - image.height())/2, j + (newWidth - image.width())/2);
			int blue = (int)pArrayBlue->getAt(i + (newHeight - image.height())/2, j + (newWidth - image.width())/2);

			QColor color( red, green, blue, 255 );

			line[j] = color.rgb();
		}
	}

	return newImage;
}

void ImageOperations::dMaskToArray2D(dArray2D *pArray, QImage mask, Channels forColor)
{
	if (mask.format() != QImage::Format_ARGB32)
		mask = mask.convertToFormat(QImage::Format_ARGB32);

	for (int i=0; i < mask.height(); i++)
	{
		QRgb *line = (QRgb*)mask.scanLine(i);

		for (int j=0; j < mask.width(); j++)
		{
			pArray->setAt(i, j, getIntensity(line[j], forColor) / (double)255);
		}
	}

}

void ImageOperations::dPointwiseProduct(dArray2D *pArrayA, dArray2D *pArrayB)
{
	for (int i = 0; i < pArrayA->getHeight(); i++)
		for (int j = 0; j < pArrayA->getWidth(); j++)
		{
			double a = pArrayA->getAt(i,j);
			double b = pArrayB->getAt(i,j);
			pArrayA->setAt(i,j, a * b);
		}
}

void ImageOperations::dDebugToFile(QString fileName, dArray2D *pArray, int height, int width)
{
	if (DEBUG)
	{
		QString temp;
		fileName += " " + temp.setNum( height, 10 ) + "x" + temp.setNum( width, 10 ) + ".txt";
		QFile file( fileName );
		file.open(QIODevice::ReadWrite);

		QString re;
		QString msg;

		for (int i=0; i < height; i++)
		{
			for (int j=0; j < width; j++)
			{
				re.setNum( pArray->getAt(i,j), 'g', 6);

				msg = re + "\t";
				file.write(msg.toAscii(), msg.length());
			}

			file.write("\n", qstrlen("\n"));
		}

		file.close();
	}
}
