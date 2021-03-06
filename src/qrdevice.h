/*
 * Class representing RTS2 device.
 *
 * @author Petr Kubanek <kubanek@fzu.cz>
 */

#ifndef __RTS2Q_DEVICE__
#define __RTS2Q_DEVICE__

#include <QWidget>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QPushButton>

#include <QNetworkRequest>
#include <QNetworkReply>

class QRDevice:public QWidget
{
Q_OBJECT
public:
    QRDevice(QString devname, QWidget *parent=0);

private slots:
    void slotRefresh();
    void rts2Updated(QJsonDocument &doc);

private:
    QString thisdevice;

    QVBoxLayout *m_layout;

    QTableWidget *m_table;

    QPushButton *m_button;
};

#endif //!__RTS2Q_DEVICE__
