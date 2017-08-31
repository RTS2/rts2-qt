#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>
#include <QDialog>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QVector>
#include <QGridLayout>
#include <QDialogButtonBox>
#include <QPushButton>

#include <libnova/libnova.h>

#include "qrcat.h"

QRCat::QRCat(double _ra, double _dec, QWidget *parent): QWidget(parent), conditions(_ra, _dec), contextMenu(this)
{
    ra = _ra;
    dec = _dec;

    background = QBrush(QColor(0,0,255));
    origin = QBrush(QColor(255,0,0));
    selected = QBrush(QColor(0,255,0));

    connect(&viz, &QRVizier::starAdded, this, &QRCat::starAdded);

    setMouseTracking(true);

    //viz.runQuery(ra,dec);

    imgMin = 400;
    imgMax = 2000;

    fi.exposeImage("C0");
//    fi.loadFITS("test.fits");
//    fi.scaleData(imgMin, imgMax, LINEAR);

    QAction *ma = new QAction(tr("&Histogram"), this);

    contextMenu.addAction(ma);
    connect(ma, &QAction::triggered, this, &QRCat::showHistogram);
}

void
QRCat::starAdded()
{
    repaint();
}

void
QRCat::showHistogram()
{
    QDialog *wind = new QDialog(this);
    wind->setWindowTitle(tr("Histogram"));

    histogramDialog.setupUi(wind);

    histogramDialog.minSpin->setValue(imgMin);
    histogramDialog.maxSpin->setValue(imgMax);

    // setup histogram
    QVector<double> x(101), y(101); // initialize with entries 0..100
    for (int i=0; i<101; ++i)
    {
        x[i] = i/50.0 - 1; // x goes from -1 to 1
        y[i] = x[i]*x[i]; // let's plot a quadratic function
    }
    // create graph and assign data to it:
    QCPBars *histogram = new QCPBars(histogramDialog.customPlot->xAxis, histogramDialog.customPlot->yAxis);

    histogram->setData(x, y);

    histogramDialog.customPlot->replot();

    QPushButton* applyButton = histogramDialog.buttonBox->button(QDialogButtonBox::Apply);

    connect(applyButton, SIGNAL(clicked()), this, SLOT(histogramApply()));

    wind->show();
}

void
QRCat::histogramApply()
{
    imgMin = histogramDialog.minSpin->value();
    imgMax = histogramDialog.maxSpin->value();

    fi.scaleData(imgMin, imgMax, LINEAR);

    repaint();
}

void
QRCat::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    fi.drawImage(&painter, -200, -200);

    paintStars(&painter);
    paintAG(&painter);

    painter.end();

    QWidget::paintEvent(event);
}

void
QRCat::mouseMoveEvent(QMouseEvent *event)
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

void
QRCat::mousePressEvent(QMouseEvent *event)
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

void
QRCat::mouseReleaseEvent(QMouseEvent *event)
{
    switch (event->button())
    {
        case Qt::MidButton:
            break;
    }
}

void
QRCat::wheelEvent(QWheelEvent *event)
{
    conditions.changeScale(event->delta() * 100);
    repaint();
}

void
QRCat::keyPressEvent(QKeyEvent *event)
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
	case 'H':
	    if (event->modifiers() & Qt::AltModifier)
	        showHistogram();
	    break;
    }
}

void
QRCat::paintStars(QPainter *painter)
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

void
QRCat::paintAG(QPainter *painter)
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
//    painter->rotate(75);
//    painter->scale(1.2,1.2);

    painter->drawPath(ag);
}
