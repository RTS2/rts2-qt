#include "qrtelescope.h"
#include "qrapp.h"

QRTelescope::QRTelescope(const QString &_telescope, QWidget *parent) :
    QWidget(parent),
    telescope(_telescope)
{
    gridLayout = new QGridLayout(this);

    QToolButton *b = new QToolButton();
    b->setArrowType(Qt::UpArrow);
    gridLayout->addWidget(b, 0, 1);
    connect(b, SIGNAL(clicked()), this, SLOT(buttonUpClicked()));

    b = new QToolButton();
    b->setArrowType(Qt::LeftArrow);
    gridLayout->addWidget(b, 1, 0);
    connect(b, SIGNAL(clicked()), this, SLOT(buttonLeftClicked()));

    b = new QToolButton();
    b->setArrowType(Qt::RightArrow);
    gridLayout->addWidget(b, 1, 2);
    connect(b, SIGNAL(clicked()), this, SLOT(buttonRightClicked()));

    b = new QToolButton();
    b->setArrowType(Qt::DownArrow);
    gridLayout->addWidget(b, 2, 1);
    connect(b, SIGNAL(clicked()), this, SLOT(buttonDownClicked()));

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
QRTelescope::buttonRightClicked()
{
    QRApp::getInstance().rts2IncValue(telescope, "OFFS", 0, +1/60.0);
}
