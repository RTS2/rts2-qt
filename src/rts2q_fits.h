#pragma once

#include <QObject>
#include <QImage>
#include <QPainter>

#include <QNetworkRequest>
#include <QNetworkReply>

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
		void exposeImage(const char *device);
		void scaleData(float min, float max, ScaleType type);

		void drawImage(QPainter *painter, float x, float y);

	protected slots:
		void imageReadyRead();

	private:
		int bitpix;
		int naxis;
		long naxes[5];

		uint16_t *data;
		QImage *image;

		QNetworkRequest request;
		QNetworkReply *imageReply;
};
