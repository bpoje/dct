#pragma once

#include <QtWidgets/QMainWindow>
//#include <QWorkspace> deprecated
#include <QtWidgets/QMdiArea>
#include <QCloseEvent>
#include <QtWidgets/QMenu>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include "ImageDisplay.h"
#include "ImageOperations.h"
#include "ConvolutionDialog.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QRect screenRect);

public slots:
	void newFile();
	void performConvolutionFFT(ImageDisplay *final);
	void performConvolutionDCT(ImageDisplay *final);
	void showMessageOnStatusBar( QString & message, int timeout = 0 );

protected:
	void closeEvent(QCloseEvent *event);

private slots:

	void open();
	void save();
	void saveAs();
	//void cut();
	//void copy();
	//void paste();
	void about();

	void updateStatusBar();

	void getPowerSpectrumUsingFFT();
	void getPowerSpectrumUsingDCT();
	void convolutionFFTDialog();
	void convolutionDCTDialog();

private:
	void createActions();
	void createMenus();
	void createToolBars();
	void createStatusBar();
	QString imageFormatToString(int format);

	ImageDisplay *createImageDisplay(int maxCurrentHeight, int maxCurrentWidth);
	ImageDisplay *activeImageDisplay();

	//QWorkspace *workspace; deprecated
	QMdiArea *workspace;

	QMenu *fileMenu;
	QMenu *editMenu;
	QMenu *calcMenu;
	QMenu *windowMenu;
	QMenu *helpMenu;

	QLabel *imageFormatLabel;
	QLabel *imageSizeLabel;

	QToolBar *fileToolBar;
	QToolBar *calcToolBar;

	QActionGroup *windowActionGroup;
	QAction *newAction;
	QAction *openAction;
	QAction *saveAction;
	QAction *saveAsAction;
	QAction *exitAction;
	QAction *cutAction;
	QAction *copyAction;
	QAction *pasteAction;
	QAction *closeAction;
	QAction *closeAllAction;
	QAction *tileAction;
	QAction *cascadeAction;
	QAction *nextAction;
	QAction *previousAction;
	QAction *separatorAction;
	QAction *aboutAction;
	QAction *aboutQtAction;

	QAction *powerSpectrumFFTAction;
	QAction *powerSpectrumDCTAction;
	QAction *linearConvolutionFFTAction;
	QAction *linearConvolutionDCTAction;
};
