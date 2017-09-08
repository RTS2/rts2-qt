#ifndef QRTELESCOPE_H
#define QRTELESCOPE_H

#include <QToolButton>
#include <QWidget>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QLCDNumber>

/**
 * Display telescope widget
 */
class QRTelescope : public QWidget
{
    Q_OBJECT

public:
    QRTelescope(const QString &_telescope, QWidget *parent = 0);
    ~QRTelescope();

private:
    QVBoxLayout *mainLayout;
    QGridLayout *gridLayout;

    QLCDNumber *raDec;

    QString telescope;

private slots:
    void buttonUpClicked();
    void buttonDownClicked();
    void buttonLeftClicked();
    void buttonRightClicked();
    void buttonStopClicked();

    void rts2Updated(QJsonDocument &doc);
};

#endif
