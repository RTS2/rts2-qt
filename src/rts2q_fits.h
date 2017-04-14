#pragma once

#include <QImage>
#include <QPainter>

typedef enum {
	LINEAR,
	LOG
} ScaleType;

class QFitsImage
{
	public:
		QFitsImage();
		~QFitsImage();

		void loadFITS(const char *fn);
		void scaleData(float min, float max, ScaleType type);

		void drawImage(QPainter *painter, float x, float y);

	private:
		int bitpix;
		int naxis;
		long naxes[5];

		uint16_t *data;
		QImage *image;
};
