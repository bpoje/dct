#pragma once

#include <QWidget>
#include <QLabel>
#include <QScrollArea>
#include <QAction>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QAction>
#include <QFileDialog>
#include <QCloseEvent>

class ImageDisplay : public QWidget
{
	Q_OBJECT

public:
	ImageDisplay( QWidget *parent = 0 , int maxCurrentHeight = 0, int maxCurrentWidth = 0);

	void newFile();
	void newFile(QString curFile);
	bool open();
	bool save();
	bool saveAs();

	QImage getImage();
	QString getCurFile();
	QString getCurFileWithStrippedName();
	QAction *windowMenuAction() const
	{
		return action;
	}

public slots:
	void setImage( QImage image );

private slots:
	void imageWasModified();

protected:
	void closeEvent(QCloseEvent *event);

signals:
	void imageChanged ( const  QImage image );

private:
	bool openFile(const QString &fileName);
	bool readFile(const QString &fileName);
	bool writeFile(const QString &fileName);
	QString strippedName(const QString &fullFileName);
	void setCurrentFile(const QString &fileName);
	bool okToContinue();

	QVBoxLayout *vLayout;
	QGroupBox *groupBox;
	QLabel *imageLabel;
	QScrollArea *scrollArea;

	QImage m_image;
	double scaleFactor;

	QString curFile;
	bool isUntitled;
	QString openFileFilters;
	QString saveFileFilters;
	QAction *action;

	int maxCurHeight;
	int maxCurWidth;
};
