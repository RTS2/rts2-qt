#ifndef QRTELESCOPE_H
#define QRTELESCOPE_H

#include <QToolButton>
#include <QWidget>
#include <QGridLayout>

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
    QGridLayout *gridLayout;
    QString telescope;

private slots:
    void buttonUpClicked();
    void buttonDownClicked();
    void buttonLeftClicked();
    void buttonRightClicked();
};

#endif
