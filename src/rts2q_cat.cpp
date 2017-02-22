#include <QWidget>
#include <QPainter>


#include "rts2q_cat.h"

Rts2QCat::Rts2QCat(double _ra, double _dec, QWidget *parent): QWidget (parent)
{
	ra = _ra;
	dec = _dec;

	background = QBrush(QColor(0,0,255));

	connect(&viz, &Rts2QVizier::starAdded, this, &Rts2QCat::starAdded);

	viz.runQuery(ra,dec);
}

void Rts2QCat::starAdded()
{
	repaint();
}

void Rts2QCat::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setBrush(background);

	for (auto it = viz.stars.begin(); it != viz.stars.end(); it++)
	{
		qDebug() << "paint " << (ra - it->ra) * 360 << " " << (dec - it->dec) * 360;
		painter.drawEllipse(QPoint(100 + (ra - it->ra) * 3600, 100 + (dec - it->dec) * 3600), 5, 5);
	}
}
