#include <QDebug>
#include "fitsio.h"

#include "rts2q_fits.h"

QFitsImage::QFitsImage()
{
	data = NULL;
	image = NULL;

	loadFITS("test.fits");
	scaleData(400, 2000, LINEAR);
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

void QFitsImage::scaleData(float min, float max, ScaleType type)
{
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
