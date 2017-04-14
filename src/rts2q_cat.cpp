#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>
#include <QDialog>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QDialogButtonBox>
#include <QPushButton>

#include <libnova/libnova.h>

#include "rts2q_cat.h"

Rts2QCat::Rts2QCat(double _ra, double _dec, QWidget *parent): QWidget(parent), conditions(_ra, _dec), contextMenu(this)
{
	ra = _ra;
	dec = _dec;

	background = QBrush(QColor(0,0,255));
	origin = QBrush(QColor(255,0,0));
	selected = QBrush(QColor(0,255,0));

	connect(&viz, &Rts2QVizier::starAdded, this, &Rts2QCat::starAdded);

	setMouseTracking(true);

	//viz.runQuery(ra,dec);

	imgMin = 400;
	imgMax = 2000;

	fi.loadFITS("test.fits");
	fi.scaleData(imgMin, imgMax, LINEAR);

	minSpin.setRange(0,0xFFFF);
	maxSpin.setRange(0,0xFFFF);

	QAction *ma = new QAction(tr("&Histogram"), this);

	contextMenu.addAction(ma);
	connect(ma, &QAction::triggered, this, &Rts2QCat::showHistogram);
}

void Rts2QCat::starAdded()
{
	repaint();
}

void Rts2QCat::showHistogram()
{
	QDialog *wind = new QDialog(this);
	wind->setWindowTitle(tr("Histogram"));

	QWidget *controlBox = new QWidget();

	QGridLayout *controlLayout = new QGridLayout();

	controlLayout->addWidget(new QLabel(tr("Min:")),0,0);
	controlLayout->addWidget(&minSpin,0,1);
	controlLayout->addWidget(new QLabel(tr("Max:")),0,2);
	controlLayout->addWidget(&maxSpin,0,3);

	minSpin.setValue(imgMin);
	maxSpin.setValue(imgMax);

	controlBox->setLayout(controlLayout);

	QVBoxLayout *mainBox = new QVBoxLayout();

	mainBox->addWidget(controlBox);

	QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel | QDialogButtonBox::Apply);
	mainBox->addWidget(buttonBox);

	QPushButton* applyButton = buttonBox->button(QDialogButtonBox::Apply);

	connect(applyButton, SIGNAL(clicked()), this, SLOT(histogramApply()));

	wind->setLayout(mainBox);

	wind->show();
}

void Rts2QCat::histogramApply()
{
	imgMin = minSpin.value();
	imgMax = maxSpin.value();

	fi.scaleData(minSpin.value(), maxSpin.value(), LINEAR);

	repaint();
}

void Rts2QCat::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);

	fi.drawImage(&painter, -200, -200);

	paintStars(&painter);
	paintAG(&painter);

	painter.end();

	QWidget::paintEvent(event);
}

void Rts2QCat::mouseMoveEvent(QMouseEvent *event)
{
	struct ln_equ_posn pos;
	viz.inverseAzimuthalEqualArea(&conditions, event->x() - 300, event->y() - 300, pos.ra, pos.dec);
	//qDebug() << event->x() << " " << event->y() << " b " << event->buttons() << " ra " << pos.ra << " dec " << pos.dec;
	switch (event->buttons())
	{
		case Qt::MidButton:
			ra += (lastX - event->x()) / 3600.0;
			dec += (lastY - event->y()) / 3600.0;
			qDebug() << "new ra " << ra << " " << dec;
			viz.runQuery(ra,dec);

			lastX = event->x();
			lastY = event->y();
			break;

		case Qt::LeftButton:
		{
			uint16_t newMin = imgMin + (imgMax - imgMax) * (float)(width() - event->x()) / width();
			uint16_t newMax = imgMax - (imgMax - imgMin) * (float)(height() - event->y()) / height();
			fi.scaleData(newMin, newMax, LINEAR);
			repaint();
			break;
		}
	}
}

void Rts2QCat::mousePressEvent(QMouseEvent *event)
{
	switch (event->button())
	{
		case Qt::RightButton:
		{
			contextMenu.hide();
			contextMenu.move(event->globalX(), event->globalY());
			contextMenu.show();
			break;
		}

		case Qt::MidButton:
		case Qt::LeftButton:
			lastX = event->x();
			lastY = event->y();
			break;
	}
}

void Rts2QCat::mouseReleaseEvent(QMouseEvent *event)
{
	switch (event->button())
	{
		case Qt::MidButton:
			break;
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

void Rts2QCat::paintStars(QPainter *painter)
{
	painter->setPen(QColor(255,0,0));

	for (auto it = viz.stars.begin(); it != viz.stars.end(); it++)
	{
		double x, y;
		it->azimuthalEqualArea (&conditions, x, y);
		if (!std::isnan(it->mag))
		{
			double m = 22 - it->mag;
			if (selectedStars.contains(it->name))
				painter->setBrush(selected);
			else
				painter->setBrush(Qt::NoBrush);

			painter->drawRect(QRectF(300 + x, 300 + y, m, m));
		}
	}
}

void Rts2QCat::paintAG(QPainter *painter)
{
	int cx = 300;
	int cy = 300;
	painter->setPen(QColor(0,255,0));

	QPainterPath ag;
	ag.moveTo(100,100);
	ag.lineTo(-100,100);
	ag.lineTo(-100,80);
	ag.lineTo(80,80);
	ag.lineTo(80,-80);
	ag.lineTo(-100,-80);
	ag.lineTo(-100,-100);
	ag.lineTo(100,-100);
	ag.lineTo(100,100);

	painter->translate(QPointF(cx,cy));
//	painter->rotate(75);
//	painter->scale(1.2,1.2);

	painter->drawPath(ag);
}
