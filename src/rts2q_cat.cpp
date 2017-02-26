#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>
#include <libnova/libnova.h>

#include "rts2q_cat.h"

Rts2QCat::Rts2QCat(double _ra, double _dec, QWidget *parent): QWidget (parent), conditions (_ra, _dec)
{
	ra = _ra;
	dec = _dec;

	background = QBrush(QColor(0,0,255));
	origin = QBrush(QColor(255,0,0));

	connect(&viz, &Rts2QVizier::starAdded, this, &Rts2QCat::starAdded);

	setMouseTracking(true);

	viz.runQuery(ra,dec);
}

void Rts2QCat::starAdded()
{
	repaint();
}

void Rts2QCat::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);

	for (auto it = viz.stars.begin(); it != viz.stars.end(); it++)
	{
		double x, y;
		it->azimuthalEqualArea (&conditions, 20 * 3600, x, y);
		qDebug() << "paint " << (ra - it->ra) * 360 << " " << (dec - it->dec) * 360 << " " << 300 + x << " " << 300 + y;
		painter.setBrush(background);
		painter.drawEllipse(QPoint(300 + x, 300 + y), 5, 5);

		painter.setBrush(origin);
		painter.drawEllipse(QPoint(300 + (ra - it->ra) * 360 * -3.5, 300 + (dec - it->dec) * 360 * -3.5), 5, 5);
	}
}

void Rts2QCat::mouseMoveEvent(QMouseEvent *event)
{
	struct ln_equ_posn pos;
	viz.inverseAzimuthalEqualArea(&conditions, 20 * 3600, event->pos().x() - 300, event->pos().y() - 300, pos.ra, pos.dec);
	qDebug() << event->pos().x() << " " << event->pos().y() << " ra " << pos.ra << " dec " << pos.dec;
}

void Rts2QCat::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::MidButton)
	{
		struct ln_equ_posn pos;
		viz.inverseAzimuthalEqualArea(&conditions, 20 * 3600, event->pos().x() - 300, event->pos().y() - 300, pos.ra, pos.dec);
		QString status = QString("%1 %2").arg(QString::number(pos.ra), QString::number(pos.dec));
		QMessageBox::question(this, tr("Position"), status);
	}
}
