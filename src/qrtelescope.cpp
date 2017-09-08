#include "qrtelescope.h"
#include "qrapp.h"

#include <QApplication>
#include <QStyle>

QRTelescope::QRTelescope(const QString &_telescope, QWidget *parent) :
    QWidget(parent),
    telescope(_telescope)
{
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

    setLayout(gridLayout);
}

QRTelescope::~QRTelescope()
{
    delete gridLayout;
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
