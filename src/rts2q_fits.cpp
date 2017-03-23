#include <QDebug>

#include "rts2q_fits.h"

QFitsImage::QFitsImage():QImage(200, 200, QImage::Format_Grayscale8)
{
	uchar data[200];
	int i;
	for (i = 0; i < 200; i++)
		data[i] = i;

	qDebug() << "bytes per line" << bytesPerLine();
	
	for (i = 0; i < 200; i++)
	{
		memcpy(scanLine(i), data, bytesPerLine());
	}
}
