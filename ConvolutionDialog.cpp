#include "ConvolutionDialog.h"

ConvolutionDialog::ConvolutionDialog(QWidget *parent) : QDialog(parent)
{
	m_final = NULL;

	itemIndex = -1;

	setWindowTitle(tr("Please select mask"));

	targetMaskSize = "";

	vLayoutWindow = new QVBoxLayout(this);

	groupBox1 = new QGroupBox(tr("Masks"));
	groupBox2 = new QGroupBox();
	label1 = new QLabel(targetMaskSize);
	label2 = new QLabel(tr("Opened images that can be used as masks:"));
	comboBox = new QComboBox();
	buttonOK = new QPushButton(tr("OK"));
	buttonCancel = new QPushButton(tr("Cancel"));

	vLayout2 = new QVBoxLayout(groupBox1);
	vLayout2->addWidget(label1);
	vLayout2->addWidget(label2);
	vLayout2->addWidget(comboBox);

	hLayout = new QHBoxLayout(groupBox2);
	hLayout->addWidget(buttonCancel);
	hLayout->addWidget(buttonOK);

	vLayoutWindow->addWidget(groupBox1);
	vLayoutWindow->addWidget(groupBox2);

	connect(buttonOK, SIGNAL(clicked()), this, SLOT(ok()));
	connect(buttonCancel, SIGNAL(clicked()), this, SLOT(cancel()));

	connect(comboBox, SIGNAL(currentIndexChanged(int)),
	        this, SLOT(setItemIndex(int)));
}

void ConvolutionDialog::addSelectCandidate(ImageDisplay* candidate)
{
	candidates << candidate;
}

void ConvolutionDialog::defineTargetMaskSize(int height, int width)
{
	targetMaskSize = tr("Required mask dimensions: %1 x %2").arg(width).arg(height);
}

void ConvolutionDialog::refreshComboBox()
{
	comboBox->clear();

	QList<ImageDisplay*>::const_iterator stlIter;
	for ( stlIter = candidates.begin(); stlIter != candidates.end(); ++stlIter )
	{
		comboBox->addItem(strippedName((*stlIter)->getCurFile()) ,"");
	}

	label1->setText(targetMaskSize);
}

void ConvolutionDialog::ok()
{
	if (itemIndex == -1)
	{
		QMessageBox::critical ( this, tr("No mask selected!"), "No mask selected!", QMessageBox::Ok, QMessageBox::NoButton );
		reject();
		return;
	}

	ImageDisplay *imageDisplay = candidates.at(itemIndex);
	setFinal(imageDisplay);
	accept();
}

void ConvolutionDialog::cancel()
{
	reject();
}

void ConvolutionDialog::setFinal(ImageDisplay *final)
{
	if ( m_final == final )
		return;

	m_final = final;
	emit finalSelected( m_final );
}

void ConvolutionDialog::setItemIndex(int index)
{
	itemIndex = index;
}

QString ConvolutionDialog::strippedName(const QString &fullFileName)
{
	return QFileInfo(fullFileName).fileName();
}
