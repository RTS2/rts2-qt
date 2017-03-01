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
	selected = QBrush(QColor(0,255,0));

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

	painter.setBrush(origin);

	for (auto it = viz.stars.begin(); it != viz.stars.end(); it++)
	{
		double x, y;
		it->azimuthalEqualArea (&conditions, x, y);
		if (!std::isnan(it->mag))
		{
			double m = 22 - it->mag;
			if (selectedStars.contains(it->name))
				painter.setBrush(selected);
			else
				painter.setBrush(origin);

			painter.drawEllipse(QPointF(300 + x, 300 + y), m, m);
		}
	}

	painter.end();

	QWidget::paintEvent(event);
}

void Rts2QCat::mouseMoveEvent(QMouseEvent *event)
{
	struct ln_equ_posn pos;
	viz.inverseAzimuthalEqualArea(&conditions, event->pos().x() - 300, event->pos().y() - 300, pos.ra, pos.dec);
	//qDebug() << event->pos().x() << " " << event->pos().y() << " ra " << pos.ra << " dec " << pos.dec;
}

void Rts2QCat::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::MidButton)
	{
		struct ln_equ_posn pos;
		viz.inverseAzimuthalEqualArea(&conditions, event->pos().x() - 300, event->pos().y() - 300, pos.ra, pos.dec);
		Rts2QStar closest = viz.getClosest(pos.ra, pos.dec);
		QString title = tr("Star %1").arg(closest.name);
		QString status = tr("Star %1 RA %2 DEC %3 magnitude %4").arg(closest.name).arg(pos.ra).arg(pos.dec).arg(closest.mag);

		selectedStars.append(closest.name);

		QMessageBox::information(this, title, status);
	}
}

void Rts2QCat::wheelEvent(QWheelEvent *event)
{
	conditions.changeScale(event->delta() * 100);
	repaint();
}

void Rts2QCat::keyPressEvent(QKeyEvent *event)
{
	switch (event->key())
	{
		case Qt::Key_Plus:
			conditions.changeScale(100);
			repaint();
			break;
		case Qt::Key_Minus:
			conditions.changeScale(-100);
			repaint();
			break;
	}
}
