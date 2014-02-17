#include <QApplication>
#include <QDesktopWidget>
#include "MainWindow.h"

#include "FFT.h"
#include "DCT.h"
#include "Array1D.h"
#include "complex.h"
#include <stdio.h>
#include <math.h>

//#include <QtPlugin>
//Q_IMPORT_PLUGIN(qjpeg)
//Q_IMPORT_PLUGIN(qgif)

void testFFT()
{
	cArray1D tab(8);
	tab.setAt(0, 1);
	tab.setAt(1, 2);
	tab.setAt(2, 3);
	tab.setAt(3, 4);
	tab.setAt(4, 5);
	tab.setAt(5, 6);
	tab.setAt(6, 7);
	tab.setAt(7, 8);

	FFT::Forward(&tab);
	FFT::Inverse(&tab);

	printf("%f, %f\n", tab.getAt(0).re(), tab.getAt(0).im());
	printf("%f, %f\n", tab.getAt(1).re(), tab.getAt(1).im());
	printf("%f, %f\n", tab.getAt(2).re(), tab.getAt(2).im());
	printf("%f, %f\n", tab.getAt(3).re(), tab.getAt(3).im());
	printf("%f, %f\n", tab.getAt(4).re(), tab.getAt(4).im());
	printf("%f, %f\n", tab.getAt(5).re(), tab.getAt(5).im());
	printf("%f, %f\n", tab.getAt(6).re(), tab.getAt(6).im());
	printf("%f, %f\n", tab.getAt(7).re(), tab.getAt(7).im());
}

void testDCT()
{
	dArray1D tab1(8);
	tab1.setAt(0, 1);
	tab1.setAt(1, 2);
	tab1.setAt(2, 3);
	tab1.setAt(3, 4);
	tab1.setAt(4, 5);
	tab1.setAt(5, 6);
	tab1.setAt(6, 7);
	tab1.setAt(7, 8);

	DCT::Forward(&tab1);
	//DCT::Inverse(&tab1);

	printf("\n");
	printf("%f\n", tab1.getAt(0));
	printf("%f\n", tab1.getAt(1));
	printf("%f\n", tab1.getAt(2));
	printf("%f\n", tab1.getAt(3));
	printf("%f\n", tab1.getAt(4));
	printf("%f\n", tab1.getAt(5));
	printf("%f\n", tab1.getAt(6));
	printf("%f\n", tab1.getAt(7));
}

void testFindMinMax()
{
	cArray2D tab2(2,2);
	tab2.setAt(0,0, complex(1,0));
	tab2.setAt(0,1, complex(1.4,-3));
	tab2.setAt(1,0, complex(-2.3,1));
	tab2.setAt(1,1, complex(0.3,5));

	complex min = tab2.findMin();
	complex max = tab2.findMax();

	//printf("min: (%f, %f)", min.re(), min.im());
	//printf("max: (%f, %f)", max.re(), max.im());
}

int main(int argc, char *argv[])
{
	Q_INIT_RESOURCE(mdi);

	QApplication app(argc, argv);

	QDesktopWidget *desktopWidget = app.desktop();
	QRect screenRect = desktopWidget->screenGeometry();

	MainWindow mainWin(screenRect);
	mainWin.show();

	return app.exec();
}
