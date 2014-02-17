#pragma once

#include <QDialog>
#include <QLabel>
#include <QGroupBox>
#include <QComboBox>
#include <QList>
#include <QPushButton>
#include <QMessageBox>
#include "ImageDisplay.h"

class ConvolutionDialog : public QDialog
{
	Q_OBJECT

public:
	ConvolutionDialog(QWidget *parent = 0);
	void addSelectCandidate(ImageDisplay* candidate);
	void defineTargetMaskSize(int height, int width);

public slots:
	void refreshComboBox();

private slots:
	void cancel();
	void ok();
	void setFinal(ImageDisplay *final);
	void setItemIndex(int index);

signals:
	void finalSelected(ImageDisplay *final);

private:
	QList<ImageDisplay*> candidates;
	ImageDisplay *m_final;
	int itemIndex;

	QVBoxLayout *vLayoutWindow;
	QVBoxLayout *vLayout2;
	QHBoxLayout *hLayout;
	QGroupBox *groupBox1;
	QGroupBox *groupBox2;
	QLabel *label1;
	QLabel *label2;
	QComboBox *comboBox;
	QPushButton *buttonOK;
	QPushButton *buttonCancel;

	QString targetMaskSize;

	QString strippedName(const QString &fullFileName);
};
