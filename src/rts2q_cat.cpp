#include <QWidget>
#include <QPainter>


#include "rts2q_cat.h"

Rts2QCat::Rts2QCat(double _ra, double _dec, QWidget *parent): QWidget (parent)
{
	ra = _ra;
	dec = _dec;

	background = QBrush(QColor(0,0,255));
}

void Rts2QCat::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setBrush(background);
	painter.drawRect(QRect(10, 20, 80, 60));
}
