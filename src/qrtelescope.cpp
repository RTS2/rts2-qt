#include "qrtelescope.h"
#include "qrapp.h"

#include <libnova/libnova.h>

#include <QApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QStyle>

QRTelescope::QRTelescope(const QString &_telescope, QWidget *parent) :
    QWidget(parent),
    telescope(_telescope)
{
    mainLayout = new QVBoxLayout(this);

    raDec = new QLCDNumber();
    mainLayout->addWidget(raDec);

    raDec->setDigitCount(24);
    raDec->display("00:00:00.000 +00'00:00.00");

    gridLayout = new QGridLayout(this);

    QToolButton *b = new QToolButton();
    b->setArrowType(Qt::UpArrow);
    b->setFixedSize(50, 50);
    b->setIconSize(QSize(50, 50));
    gridLayout->addWidget(b, 0, 1);
    connect(b, SIGNAL(clicked()), this, SLOT(buttonUpClicked()));

    b = new QToolButton();
    b->setArrowType(Qt::LeftArrow);
    b->setFixedSize(50, 50);
    b->setIconSize(QSize(50, 50));
    gridLayout->addWidget(b, 1, 0);
    connect(b, SIGNAL(clicked()), this, SLOT(buttonLeftClicked()));

    b = new QToolButton();
    b->setArrowType(Qt::RightArrow);
    b->setFixedSize(50, 50);
    b->setIconSize(QSize(50, 50));
    gridLayout->addWidget(b, 1, 2);
    connect(b, SIGNAL(clicked()), this, SLOT(buttonRightClicked()));

    b = new QToolButton();
    b->setArrowType(Qt::DownArrow);
    b->setFixedSize(50, 50);
    b->setIconSize(QSize(50, 50));
    gridLayout->addWidget(b, 2, 1);
    connect(b, SIGNAL(clicked()), this, SLOT(buttonDownClicked()));

    b = new QToolButton();
    b->setIcon(QApplication::style()->standardIcon(QStyle::SP_BrowserStop));
    b->setFixedSize(50, 50);
    b->setIconSize(QSize(50, 50));
    gridLayout->addWidget(b, 1, 1);
    connect(b, SIGNAL(clicked()), this, SLOT(buttonStopClicked()));

    mainLayout->addLayout(gridLayout);

    setLayout(mainLayout);

    connect(&QRApp::getInstance(), &QRApp::rts2Updated, this, &QRTelescope::rts2Updated);
}

QRTelescope::~QRTelescope()
{
    delete gridLayout;
    delete mainLayout;
}

void
QRTelescope::buttonUpClicked()
{
    QRApp::getInstance().rts2IncValue(telescope, "OFFS", +1/60.0, 0);
}

void
QRTelescope::buttonDownClicked()
{
    QRApp::getInstance().rts2IncValue(telescope, "OFFS", -1/60.0, 0);
}

void
QRTelescope::buttonLeftClicked()
{
    QRApp::getInstance().rts2IncValue(telescope, "OFFS", 0, -1/60.0);
}

void
QRTelescope::buttonStopClicked()
{
    QRApp::getInstance().rts2Command(telescope, "stop");
}

void
QRTelescope::buttonRightClicked()
{
    QRApp::getInstance().rts2IncValue(telescope, "OFFS", 0, +1/60.0);
}

void
QRTelescope::rts2Updated(QJsonDocument &doc)
{
    QJsonObject docObject = doc.object();
    QJsonObject devArray = docObject[telescope].toObject();
    qDebug() << "tel " << devArray.toVariantMap();
    QJsonObject dArray = devArray["d"].toObject();

    struct ln_equ_posn telPos;
    telPos.ra = dArray["TEL"].toArray()[1].toObject()["ra"].toVariant().toDouble();
    telPos.dec = dArray["TEL"].toArray()[1].toObject()["dec"].toVariant().toDouble();

    struct lnh_equ_posn rdv;
    ln_equ_to_hequ(&telPos, &rdv);

    raDec->display(QString("%1:%2:%3 %4'%5:%6")
       .arg((short int) rdv.ra.hours, 2, 10, QLatin1Char('0'))
       .arg((short int) rdv.ra.minutes, 2, 10, QLatin1Char('0'))
       .arg(rdv.ra.seconds, 2, 'g', 3, QLatin1Char('0'))
       .arg((short int) rdv.dec.degrees, 2, 10, QLatin1Char('0'))
       .arg((short int) rdv.dec.minutes, 2, 10, QLatin1Char('0'))
       .arg(rdv.dec.seconds, 2, 'g', 2, QLatin1Char('0'))
    );
}
