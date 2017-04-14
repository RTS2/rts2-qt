#pragma once

#include <QObject>
#include <QImage>
#include <QPainter>

typedef enum {
	LINEAR,
	LOG
} ScaleType;

class QFitsImage:public QObject
{
	Q_OBJECT
	public:
		QFitsImage();
		~QFitsImage();

		void loadFITS(const char *fn);
		void scaleData(float min, float max, ScaleType type);

		void drawImage(QPainter *painter, float x, float y);

		void showHistogram(QWidget *parent);

	private:
		int bitpix;
		int naxis;
		long naxes[5];

		uint16_t *data;
		QImage *image;
};
