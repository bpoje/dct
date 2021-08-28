#include "MainWindow.h"
#include <QMdiArea>
#include <QMdiSubWindow>

MainWindow::MainWindow(QRect screenRect)
{
	//workspace = new QWorkspace; deprecated
	workspace = new QMdiArea;
	setCentralWidget(workspace);
	//connect(workspace, SIGNAL(windowActivated(QWidget *)),
	//        this, SLOT(updateMenus()));

	createActions();
	createMenus();
	createToolBars();
	createStatusBar();

	setWindowTitle(tr("DCT & FFT Editor"));
	setWindowIcon(QPixmap(":/images/app.png"));

	resize((screenRect.right() - screenRect.left())/2, (screenRect.bottom() - screenRect.top())/2);
	move((screenRect.right() - screenRect.left())/4, (screenRect.bottom() - screenRect.top())/4);
}

//The closeEvent() function is reimplemented to close all child windows, causing each child to
//receive a close event.
void MainWindow::closeEvent(QCloseEvent *event)
{
	//workspace->closeAllWindows(); deprecated
	workspace->closeAllSubWindows();
	if (activeImageDisplay())
	{
		event->ignore();
	}
	else
	{
		event->accept();
	}
}

//The activeEditor() private function returns the active child window as an ImageDisplay pointer,
//or a null pointer if there isn't one.
ImageDisplay *MainWindow::activeImageDisplay()
{
	//return qobject_cast<ImageDisplay *>(workspace->activeWindow()); deprecated
	return qobject_cast<ImageDisplay *>( (QObject*) workspace->activeSubWindow() );

	//QMdiSubWindow *test1 = workspace->activeSubWindow();
	//QObject *test2 = (QObject *) test1;
	//qobject_cast<ImageDisplay *>( test2 );

	//return qobject_cast<ImageDisplay *>(workspace->activeSubWindow());
	//return qobject_cast<ImageDisplay *>(workspace->activeSubWindow());
	//return qobject_cast<ImageDisplay *>(workspace->activeSubWindow());
	//return qobject_cast<QMdiSubWindow *>(workspace->activeSubWindow());
}

void MainWindow::createActions()
{


	openAction = new QAction(QIcon(":/images/open.png"), tr("&Open..."),
	                         this);
	openAction->setShortcut(tr("Ctrl+O"));
	openAction->setStatusTip(tr("Open an existing file"));
	connect(openAction, SIGNAL(triggered()), this, SLOT(open()));

	saveAction = new QAction(QIcon(":/images/save.png"), tr("&Save"),
	                         this);
	saveAction->setShortcut(tr("Ctrl+S"));
	saveAction->setStatusTip(tr("Save the file to disk"));
	connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));

	saveAsAction = new QAction(tr("Save &As..."), this);
	saveAsAction->setStatusTip(tr("Save the file under a new name"));
	connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAs()));

	exitAction = new QAction(tr("E&xit"), this);
	exitAction->setShortcut(tr("Ctrl+Q"));
	exitAction->setStatusTip(tr("Exit the application"));
	connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

	closeAction = new QAction(tr("Cl&ose"), this);
	closeAction->setShortcut(tr("Ctrl+F4"));
	closeAction->setStatusTip(tr("Close the active window"));
	connect(closeAction, SIGNAL(triggered()),
	        workspace, SLOT(closeActiveSubWindow()));

	closeAllAction = new QAction(tr("Close &All"), this);
	closeAllAction->setStatusTip(tr("Close all windows"));
	connect(closeAllAction, SIGNAL(triggered()),
	        workspace, SLOT(closeAllSubWindows()));

	tileAction = new QAction(tr("&Tile"), this);
	tileAction->setStatusTip(tr("Tile the windows"));
	//connect(tileAction, SIGNAL(triggered()), workspace, SLOT(tile()));
	connect(tileAction, SIGNAL(triggered()), workspace, SLOT(tileSubWindows()));

	cascadeAction = new QAction(tr("&Cascade"), this);
	cascadeAction->setStatusTip(tr("Cascade the windows"));
	//connect(cascadeAction, SIGNAL(triggered()),
	//        workspace, SLOT(cascade()));
	connect(cascadeAction, SIGNAL(triggered()),
	        workspace, SLOT(cascadeSubWindows()));

	nextAction = new QAction(tr("Ne&xt"), this);
	nextAction->setShortcut(tr("Ctrl+F6"));
	nextAction->setStatusTip(tr("Move the focus to the next window"));
	//connect(nextAction, SIGNAL(triggered()),
	//        workspace, SLOT(activateNextWindow()));
	connect(nextAction, SIGNAL(triggered()),
                workspace, SLOT(activateNextSubWindow()));

	previousAction = new QAction(tr("Pre&vious"), this);
	previousAction->setShortcut(tr("Ctrl+Shift+F6"));
	previousAction->setStatusTip(tr("Move the focus to the previous "
	                                "window"));
	//connect(previousAction, SIGNAL(triggered()),
	//        workspace, SLOT(activatePreviousWindow()));
	connect(previousAction, SIGNAL(triggered()),
                workspace, SLOT(activatePreviousSubWindow()));

	separatorAction = new QAction(this);
	separatorAction->setSeparator(true);

	aboutAction = new QAction(tr("&About"), this);
	aboutAction->setStatusTip(tr("Show the application's About box"));
	connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

	powerSpectrumFFTAction = new QAction(tr("Power spectrum with &FFT"), this);
	powerSpectrumFFTAction->setIcon(QIcon(":/images/fft.png"));
	powerSpectrumFFTAction->setShortcut(tr("Ctrl+F"));
	powerSpectrumFFTAction->setStatusTip(tr("Get power spectrum using FFT"));
	connect(powerSpectrumFFTAction, SIGNAL(triggered()), this, SLOT(getPowerSpectrumUsingFFT()));

	powerSpectrumDCTAction = new QAction(tr("Power spectrum with &DCT"), this);
	powerSpectrumDCTAction->setIcon(QIcon(":/images/dct.png"));
	powerSpectrumDCTAction->setShortcut(tr("Ctrl+D"));
	powerSpectrumDCTAction->setStatusTip(tr("Get power spectrum using DCT"));
	connect(powerSpectrumDCTAction, SIGNAL(triggered()), this, SLOT(getPowerSpectrumUsingDCT()));

	linearConvolutionFFTAction = new QAction(tr("Convolution using FFT"), this);
	linearConvolutionFFTAction->setIcon(QIcon(":/images/cfft.png"));
	linearConvolutionFFTAction->setShortcut(tr("Ctrl+L"));
	linearConvolutionFFTAction->setStatusTip(tr("Convolution using FFT"));
	connect(linearConvolutionFFTAction, SIGNAL(triggered()), this, SLOT(convolutionFFTDialog()));


	linearConvolutionDCTAction = new QAction(tr("Convolution using DCT"), this);
	linearConvolutionDCTAction->setIcon(QIcon(":/images/cdct.png"));
	linearConvolutionDCTAction->setShortcut(tr("Ctrl+K"));
	linearConvolutionDCTAction->setStatusTip(tr("Convolution using DCT"));
	connect(linearConvolutionDCTAction, SIGNAL(triggered()), this, SLOT(convolutionDCTDialog()));

	windowActionGroup = new QActionGroup(this);
}

void MainWindow::createMenus()
{
	fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(openAction);
	fileMenu->addAction(saveAction);
	fileMenu->addAction(saveAsAction);
	fileMenu->addSeparator();
	fileMenu->addAction(exitAction);


	calcMenu = menuBar()->addMenu(tr("&Calculations"));
	calcMenu->addAction(powerSpectrumFFTAction);
	calcMenu->addAction(powerSpectrumDCTAction);
	calcMenu->addSeparator();
	calcMenu->addAction(linearConvolutionFFTAction);
	calcMenu->addSeparator();
	calcMenu->addAction(linearConvolutionDCTAction);

	windowMenu = menuBar()->addMenu(tr("&Window"));
	windowMenu->addAction(closeAction);
	windowMenu->addAction(closeAllAction);
	windowMenu->addSeparator();
	windowMenu->addAction(tileAction);
	windowMenu->addAction(cascadeAction);
	windowMenu->addSeparator();
	windowMenu->addAction(nextAction);
	windowMenu->addAction(previousAction);
	windowMenu->addAction(separatorAction);

	menuBar()->addSeparator();

	helpMenu = menuBar()->addMenu(tr("&Help"));
	helpMenu->addAction(aboutAction);
}

void MainWindow::createToolBars()
{
	fileToolBar = addToolBar(tr("File"));
	fileToolBar->addAction(openAction);
	fileToolBar->addAction(saveAction);

	calcToolBar = addToolBar(tr("Calculations"));
	calcToolBar->addAction(powerSpectrumFFTAction);
	calcToolBar->addAction(powerSpectrumDCTAction);
	calcToolBar->addAction(linearConvolutionFFTAction);
	calcToolBar->addAction(linearConvolutionDCTAction);
}

void MainWindow::createStatusBar()
{
	imageFormatLabel = new QLabel("");
	imageFormatLabel->setAlignment(Qt::AlignHCenter);
	imageFormatLabel->setMinimumSize(imageFormatLabel->sizeHint());

	imageSizeLabel = new QLabel("");

	statusBar()->addWidget(imageFormatLabel);
	statusBar()->addWidget(imageSizeLabel, 1);
	statusBar()->setBackgroundRole(QPalette::Light);

	//connect(workspace, SIGNAL(windowActivated(QWidget *)),
	//        this, SLOT(updateStatusBar()));
	connect(workspace, SIGNAL(subWindowActivated(QMdiSubWindow *)),
	        this, SLOT(updateStatusBar()));

	updateStatusBar();
}

void MainWindow::updateStatusBar()
{
	if (activeImageDisplay())
	{
		int format = (int)activeImageDisplay()->getImage().format();
		QString string = imageFormatToString(format);
		imageFormatLabel->setText(string);

		int height = (int)activeImageDisplay()->getImage().height();
		int width = (int)activeImageDisplay()->getImage().width();
		string = tr("Image size: %1 x %2").arg(width).arg(height);
		imageSizeLabel->setText(string);
	}
	else
	{
		imageFormatLabel->setText("");
		imageSizeLabel->setText("");
	}
}

ImageDisplay *MainWindow::createImageDisplay(int maxCurrentHeight, int maxCurrentWidth)
{
	ImageDisplay *imageDisplay = new ImageDisplay(this, maxCurrentHeight, maxCurrentWidth);

	printf("height: %d\n", workspace->height());

	//workspace->addWindow(imageDisplay); deprecated
	workspace->addSubWindow(imageDisplay);
	windowMenu->addAction(imageDisplay->windowMenuAction());
	windowActionGroup->addAction(imageDisplay->windowMenuAction());

	return imageDisplay;
}

void MainWindow::newFile()
{
	ImageDisplay *imageDisplay = createImageDisplay(workspace->height(), workspace->width());
	imageDisplay->newFile();
	imageDisplay->show();
}

void MainWindow::open()
{
	printf("height: %d\n", workspace->height());

	ImageDisplay *imageDisplay = createImageDisplay(workspace->height(), workspace->width());
	if (imageDisplay->open())
	{
		imageDisplay->show();
		statusBar()->showMessage(tr("File loaded"), 2000);
	}
	else
	{
		imageDisplay->close();
		statusBar()->showMessage(tr("Loading failed"), 2000);
	}
}

void MainWindow::save()
{
	if (activeImageDisplay())
	{
		bool success = activeImageDisplay()->save();

		if (success)
			statusBar()->showMessage(tr("File saved"), 2000);
		else
			statusBar()->showMessage(tr("Saving failed"), 2000);
	}

}

void MainWindow::saveAs()
{
	if (activeImageDisplay())
	{
		bool success = activeImageDisplay()->saveAs();

		if (success)
			statusBar()->showMessage(tr("File saved"), 2000);
		else
			statusBar()->showMessage(tr("Saving failed"), 2000);
	}
}

void MainWindow::about()
{
	QMessageBox::about(this, tr("About DCT & FFT Editor"),
	                   tr("<h2>DCT & FFT Editor 1.1</h2>"
	                      "<p>Copyright &copy; Blaz Poje"
	                      "<p>DCT & FFT Editor is a small application that demonstrates "
	                      "image processing in the Fourier Domain."));
}

QString MainWindow::imageFormatToString(int format)
{
	QString string(tr("Image format: "));

	switch (format)
	{
	case 0:
		string += tr("Format_Invalid");
		break;
	case 1:
		string += tr("Format_Mono");
		break;
	case 2:
		string += tr("Format_MonoLSB");
		break;
	case 3:
		string += tr("Format_Indexed8");
		break;
	case 5:
		string += tr("Format_ARGB32");
		break;
	default:
		string += tr("Format_RGB32");
		break;
	}

	return string;
}

void MainWindow::getPowerSpectrumUsingFFT()
{
	if (activeImageDisplay())
	{
		QImage image = activeImageDisplay()->getImage();
		QString curFile = activeImageDisplay()->getCurFileWithStrippedName();

		ImageOperations imageOperations(image);

		if (image.allGray())
		{
			QImage RImage = imageOperations.FFTPowerSpectrum(Red);

			if (RImage.height() == 0)
				return;

			ImageDisplay *redImageDisplay = createImageDisplay(workspace->height(), workspace->width());
			redImageDisplay->newFile("FFT Gray power spectrum of " + curFile);
			redImageDisplay->setImage(RImage);
			redImageDisplay->show();
		}
		else
		{
			QImage RImage = imageOperations.FFTPowerSpectrum(Red);
			QImage GImage = imageOperations.FFTPowerSpectrum(Green);
			QImage BImage = imageOperations.FFTPowerSpectrum(Blue);

			if (RImage.height() == 0 || GImage.height() == 0 || BImage.height() == 0)
				return;

			ImageDisplay *redImageDisplay = createImageDisplay(workspace->height(), workspace->width());
			redImageDisplay->newFile("FFT Red power spectrum of " + curFile);
			redImageDisplay->setImage(RImage);
			redImageDisplay->show();

			ImageDisplay *greenImageDisplay = createImageDisplay(workspace->height(),workspace->width());
			greenImageDisplay->newFile("FFT Green power spectrum of " + curFile);
			greenImageDisplay->setImage(GImage);
			greenImageDisplay->show();

			ImageDisplay *blueImageDisplay = createImageDisplay(workspace->height(), workspace->width());
			blueImageDisplay->newFile("FFT Blue power spectrum of " + curFile);
			blueImageDisplay->setImage(BImage);
			blueImageDisplay->show();
		}
	}
}

void MainWindow::getPowerSpectrumUsingDCT()
{
	if (activeImageDisplay())
	{
		QImage image = activeImageDisplay()->getImage();
		QString curFile = activeImageDisplay()->getCurFileWithStrippedName();

		ImageOperations imageOperations(image);

		if (image.allGray())
		{
			QImage RImage = imageOperations.DCTPowerSpectrum(Red);

			if (RImage.height() == 0)
				return;

			ImageDisplay *redImageDisplay = createImageDisplay(workspace->height(), workspace->width());
			redImageDisplay->newFile("DCT Gray power spectrum of " + curFile);
			redImageDisplay->setImage(RImage);
			redImageDisplay->show();
		}
		else
		{
			QImage RImage = imageOperations.DCTPowerSpectrum(Red);
			QImage GImage = imageOperations.DCTPowerSpectrum(Green);
			QImage BImage = imageOperations.DCTPowerSpectrum(Blue);

			if (RImage.height() == 0 || GImage.height() == 0 || BImage.height() == 0)
				return;

			ImageDisplay *redImageDisplay = createImageDisplay(workspace->height(), workspace->width());
			redImageDisplay->newFile("DCT Red power spectrum of " + curFile);
			redImageDisplay->setImage(RImage);
			redImageDisplay->show();

			ImageDisplay *greenImageDisplay = createImageDisplay(workspace->height(),workspace->width());
			greenImageDisplay->newFile("DCT Green power spectrum of " + curFile);
			greenImageDisplay->setImage(GImage);
			greenImageDisplay->show();

			ImageDisplay *blueImageDisplay = createImageDisplay(workspace->height(), workspace->width());
			blueImageDisplay->newFile("DCT Blue power spectrum of " + curFile);
			blueImageDisplay->setImage(BImage);
			blueImageDisplay->show();
		}
	}
}

void MainWindow::convolutionFFTDialog()
{
	if (activeImageDisplay())
	{
		ConvolutionDialog convolutionDialog(this);

		connect(&convolutionDialog, SIGNAL(finalSelected(ImageDisplay *)),
		        this, SLOT(performConvolutionFFT(ImageDisplay *)));

		QImage currentImage = activeImageDisplay()->getImage();

		int imageHeight = currentImage.height();
		int imageWidth = currentImage.width();

		int newHeightAndWidth = ImageOperations::firstPowerOfTwoOfLargerDimension(imageWidth, imageHeight);
		int newHeight = newHeightAndWidth;
		int newWidth = newHeightAndWidth;

		QList<QAction*> list = windowActionGroup->actions();

		QList<QAction*>::const_iterator stlIter1;
		for ( stlIter1 = list.begin(); stlIter1 != list.end(); ++stlIter1 )
		{
			QAction* action = (*stlIter1);

			//QWidget * QAction::parentWidget ()
			QWidget *widget = action->parentWidget();
			ImageDisplay *imageDisplay = (ImageDisplay*) widget;

			if (activeImageDisplay() != imageDisplay)
			{
				QImage image = imageDisplay->getImage();

				int maskHeight = image.height();
				int maskWidth = image.width();

				if (newHeight > 0 && newWidth > 0 && maskHeight > 0 && maskWidth > 0 &&
				        newHeight == maskHeight && newWidth == maskWidth)
				{
					convolutionDialog.addSelectCandidate(imageDisplay);
				}
			}
		}

		convolutionDialog.defineTargetMaskSize(newHeight, newWidth);
		convolutionDialog.refreshComboBox();
		int result = convolutionDialog.exec();

		if (result == QDialog::Rejected)
			return;
	}
}

void MainWindow::convolutionDCTDialog()
{
	if (activeImageDisplay())
	{
		ConvolutionDialog convolutionDialog(this);

		connect(&convolutionDialog, SIGNAL(finalSelected(ImageDisplay *)),
		        this, SLOT(performConvolutionDCT(ImageDisplay *)));

		QImage currentImage = activeImageDisplay()->getImage();

		int imageHeight = currentImage.height();
		int imageWidth = currentImage.width();

		int newHeightAndWidth = ImageOperations::firstPowerOfTwoOfLargerDimension(imageWidth, imageHeight);
		int newHeight = newHeightAndWidth;
		int newWidth = newHeightAndWidth;

		QList<QAction*> list = windowActionGroup->actions();

		QList<QAction*>::const_iterator stlIter1;
		for ( stlIter1 = list.begin(); stlIter1 != list.end(); ++stlIter1 )
		{
			QAction* action = (*stlIter1);

			//QWidget * QAction::parentWidget ()
			QWidget *widget = action->parentWidget();
			ImageDisplay *imageDisplay = (ImageDisplay*) widget;

			if (activeImageDisplay() != imageDisplay)
			{
				QImage image = imageDisplay->getImage();

				int maskHeight = image.height();
				int maskWidth = image.width();

				if (newHeight > 0 && newWidth > 0 && maskHeight > 0 && maskWidth > 0 &&
				        newHeight == maskHeight && newWidth == maskWidth)
				{
					convolutionDialog.addSelectCandidate(imageDisplay);
				}
			}
		}

		convolutionDialog.defineTargetMaskSize(newHeight, newWidth);
		convolutionDialog.refreshComboBox();
		int result = convolutionDialog.exec();

		if (result == QDialog::Rejected)
			return;
	}
}

void MainWindow::performConvolutionFFT(ImageDisplay *final)
{
	QString curFile = activeImageDisplay()->getCurFileWithStrippedName();
	QString maskFile = final->getCurFileWithStrippedName();

	QImage image = activeImageDisplay()->getImage();
	QImage mask = final->getImage();

	ImageOperations imageOperations(image);
	QImage retImage = imageOperations.FFTConvolution(mask);

	if (retImage.height() == 0)
		return;

	ImageDisplay *imageDisplay = createImageDisplay(workspace->height(), workspace->width());
	imageDisplay->newFile("FFT Convolution of " + curFile + " with mask " + maskFile);
	imageDisplay->setImage(retImage);
	imageDisplay->show();
}

void MainWindow::performConvolutionDCT(ImageDisplay *final)
{
	QString curFile = activeImageDisplay()->getCurFileWithStrippedName();
	QString maskFile = final->getCurFileWithStrippedName();

	QImage image = activeImageDisplay()->getImage();
	QImage mask = final->getImage();

	ImageOperations imageOperations(image);
	QImage retImage = imageOperations.DCTConvolution(mask);

	if (retImage.height() == 0)
		return;

	ImageDisplay *imageDisplay = createImageDisplay(workspace->height(), workspace->width());
	imageDisplay->newFile("DCT Convolution of " + curFile + " with mask " + maskFile);
	imageDisplay->setImage(retImage);
	imageDisplay->show();
}

void MainWindow::showMessageOnStatusBar( QString & message, int timeout )
{
	statusBar()->showMessage(message, timeout);
}
