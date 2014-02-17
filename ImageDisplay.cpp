#include "ImageDisplay.h"

ImageDisplay::ImageDisplay( QWidget* parent, int maxCurrentHeight, int maxCurrentWidth) : QWidget(parent)
{
	maxCurHeight = maxCurrentHeight;
	maxCurWidth = maxCurrentWidth;

	action = new QAction(this);
	action->setCheckable(true);
	connect(action, SIGNAL(triggered()), this, SLOT(show()));
	connect(action, SIGNAL(triggered()), this, SLOT(setFocus()));

	isUntitled = true;
	openFileFilters = tr("Image files (*.ppm *.png *.pgm *.jpg *.jpeg *.gif *.xpm *.bmp *.xbm *.tif)\n"
	                     "All files (*)");
	saveFileFilters = tr("Image files (*.bmp *.jpg *.jpeg *.png *.ppm *.tif *.xbm *.xpm)\n"
	                     "All files (*)");

	setWindowIcon(QPixmap(":/images/document.png"));
	setAttribute(Qt::WA_DeleteOnClose);

	vLayout = new QVBoxLayout(this);
	imageLabel = new QLabel();
	scrollArea = new QScrollArea();
	scrollArea->setBackgroundRole(QPalette::Dark);
	scrollArea->setWidget(imageLabel);
	vLayout->addWidget(scrollArea);
}

void ImageDisplay::setImage(QImage image)
{
	if ( m_image == image )
		return;

	m_image = image;

	imageLabel->setPixmap(QPixmap::fromImage(image));
	imageLabel->adjustSize();

	bool resized = false;
	int newHeight = imageLabel->height();
	int newWidth = imageLabel->width();
	if (imageLabel->width() >= maxCurWidth)
	{
		newWidth = maxCurWidth-10;
		resize(newWidth, newHeight);
		resized = true;
	}

	if (imageLabel->height() >= maxCurHeight)
	{
		newHeight = maxCurHeight-44;
		resize(newWidth, newHeight);
		resized = true;
	}

	if (!resized)
		resize(imageLabel->width() + 20, imageLabel->height() + 20);

	emit imageChanged( m_image );
}

void ImageDisplay::closeEvent(QCloseEvent *event)
{
	if (okToContinue())
	{
		event->accept();
	}
	else
	{
		event->ignore();
	}
}

void ImageDisplay::imageWasModified()
{
	setWindowModified(true);
}

QString ImageDisplay::strippedName(const QString &fullFileName)
{
	return QFileInfo(fullFileName).fileName();
}

void ImageDisplay::setCurrentFile(const QString &fileName)
{
	curFile = fileName;
	isUntitled = false;
	action->setText(strippedName(curFile));

	setWindowTitle(strippedName(curFile) + "[*]");
	setWindowModified(false);
}

bool ImageDisplay::okToContinue()
{
	if (isWindowModified())
	{
		int r = QMessageBox::warning(this, tr("MDI Editor"),
		                             tr("File %1 has been modified.\n"
		                                "Do you want to save your changes?")
		                             .arg(strippedName(curFile)),
		                             QMessageBox::Yes | QMessageBox::Default,
		                             QMessageBox::No,
		                             QMessageBox::Cancel | QMessageBox::Escape);
		if (r == QMessageBox::Yes)
		{
			return save();
		}
		else if (r == QMessageBox::Cancel)
		{
			return false;
		}
	}
	return true;
}

void ImageDisplay::newFile()
{
	static int imageNumber = 1;

	curFile = tr("Untitled%1.jpg").arg(imageNumber);
	setWindowTitle(curFile + "[*]");
	action->setText(curFile);
	isUntitled = true;
	ImageDisplay::curFile = curFile;
	setWindowModified(true);
	++imageNumber;
}

void ImageDisplay::newFile(QString curFile)
{
	setWindowTitle(curFile + "[*]");
	action->setText(curFile);
	isUntitled = true;
	ImageDisplay::curFile = curFile;
	setWindowModified(true);
}

bool ImageDisplay::open()
{
	QString fileName =
	    QFileDialog::getOpenFileName(this, tr("Open"), ".",
	                                 openFileFilters);
	if (fileName.isEmpty())
		return false;

	return openFile(fileName);
}

bool ImageDisplay::save()
{
	if (isUntitled)
	{
		return saveAs();
	}
	else
	{
		return writeFile(curFile);
	}
}

bool ImageDisplay::saveAs()
{
	QString fileName =
	    QFileDialog::getSaveFileName(this, tr("Save As"), curFile,
	                                 saveFileFilters);
	if (fileName.isEmpty())
		return false;

	return writeFile(fileName);
}

bool ImageDisplay::openFile(const QString &fileName)
{
	if (readFile(fileName))
	{
		setCurrentFile(fileName);
		return true;
	}
	else
	{
		return false;
	}
}

bool ImageDisplay::readFile(const QString &fileName)
{
	QImage image(fileName);
	if (image.isNull())
	{
		QMessageBox::information(this, tr("Image Viewer"),
		                         tr("Cannot load %1.").arg(fileName));
		return false;
	}

	setImage(image);

	return true;
}

bool ImageDisplay::writeFile(const QString &fileName)
{
	if (m_image.save(fileName, 0, -1))
	{
		setCurrentFile(fileName);
		return true;
	}
	else
	{
		return false;
	}
}

QImage ImageDisplay::getImage()
{
	return m_image;
}

QString ImageDisplay::getCurFile()
{
	return curFile;
}

QString ImageDisplay::getCurFileWithStrippedName()
{
	return strippedName(curFile);
}
