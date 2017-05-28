#include <QDebug>
#include <QUrlQuery>
#include <arpa/inet.h>

#include "fitsio.h"

#include "config.h"
#include "rts2q_fits.h"

QFitsImage::QFitsImage()
{
	data = NULL;
	image = NULL;
	imageReply = NULL;

	receivedLen = 0;
}

QFitsImage::~QFitsImage()
{
	delete image;
	delete[] data;
}

void QFitsImage::loadFITS(const char *fn)
{
	fitsfile *fptr;
	int status = 0;
	fits_open_file(&fptr, fn, READONLY, &status);

	fits_get_img_param(fptr, 5, &bitpix, &naxis, naxes, &status);

	char err_text[30];

	if (status != 0)
	{
		fits_get_errstatus(status, err_text);
		qDebug() << "QFitsImage::loadFITS opening FITS file failed:" << err_text;
		return;
	}

	if (naxis != 2)
	{
		qDebug() << "QFitsImage::loadFITS unable to handle non 2D images";
		return;
	}

	delete image;
	delete[] data;

	image = new QImage(naxes[1], naxes[0], QImage::Format_Grayscale8);

	long fpixel[2];
	fpixel[0] = fpixel[1] = 1;

	data = new uint16_t[naxes[0] * naxes[1]];

	fits_read_pix(fptr, TUSHORT, fpixel, naxes[0] * naxes[1], NULL, data, NULL, &status);

	if (status != 0)
	{
		fits_get_errstatus(status, err_text);
		qDebug() << "QFitsImage::loadFITS reading image failed:" << err_text;
		return;
	}
}

void QFitsImage::exposeImage(const char *device)
{
	QUrl rurl(Config::getInstance().baseurl);
	rurl.setPath("/api/exposedata");

	QUrlQuery query;
	query.addQueryItem("ccd", device);
	rurl.setQuery(query);

	request.setUrl(rurl);

	delete data;
	delete image;
	receivedLen = 0;

	data = new uint16_t[sizeof(imghdr) / 2];
	image = NULL;

	imageReply = Config::getInstance().networkManager.get(request);
	connect(imageReply, SIGNAL(readyRead()), this, SLOT(imageReadyRead()));
	connect(imageReply, SIGNAL(finished()), this, SLOT(imageFinished()));
}

void QFitsImage::scaleData(float min, float max, ScaleType type)
{
	if (data == NULL)
		return;

	uint8_t line[naxes[0]];

	uint16_t *pp = data;

	for (long i = 0; i < naxes[1]; i++)
	{
		for (long j = 0; j < naxes[0]; j++, pp++)
		{
			if (*pp < min)
			{
				line[j] = 0;
			}
			else if (*pp > max)
			{
				line[j] = 0xFF;
			}
			else
			{
				line[j] = 0xFF * (((float) *pp - min) / (max - min));
			}
		}
		memcpy(image->scanLine(i), line, image->bytesPerLine());
	}
}

void QFitsImage::drawImage(QPainter *painter, float x, float y)
{
	if (image == NULL)
	{
		qWarning() << "QFitsImage::drawImage image not loaded";
		return;
	}
	painter->drawImage(x, y, *image);
}

void QFitsImage::imageReadyRead()
{
	if (receivedLen < sizeof(imghdr))
	{
		qint64 rs = imageReply->read((char *) data + receivedLen, sizeof(imghdr) - receivedLen);
		receivedLen += rs;
		if (receivedLen <  sizeof(imghdr))
			return;
		imghdr *hdr = (imghdr *) data;
		if (ntohs (hdr->naxes) != 2)
		{
			qDebug() << "QFitsImage::imageReadyRead unsuported naxes" << hdr->naxes;
			imageReply->close();
			imageReply->deleteLater();
		}
		naxes[0] = ntohl (hdr->sizes[0]);
		naxes[1] = ntohl (hdr->sizes[1]);

		qDebug() << QString("QFitsImage::imageReadyRead naxes %1x%2").arg(naxes[0]).arg(naxes[1]);

		delete data;
		data = new uint16_t[naxes[0] * naxes[1]];
		image = new QImage(naxes[0], naxes[1], QImage::Format_Grayscale8);
	}

	if (receivedLen >= sizeof(imghdr))
	{
		qint64 rs = imageReply->read((char *) data + receivedLen, naxes[0] * naxes[1] * 2 - (receivedLen - sizeof(imghdr)));
		receivedLen += rs;
	}
}

void QFitsImage::imageFinished()
{
	scaleData(200,1500,LINEAR);
	imageReply->deleteLater();
}
