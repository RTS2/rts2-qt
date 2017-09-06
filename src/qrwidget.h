#ifndef QRWIDGET_H
#define QRWIDGET_H

#include <QLineEdit>
#include <QXmlStreamReader>
#include <QWidget>

class QRLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    QRLineEdit(const QString &_device, const QString &_var, QWidget *parent = Q_NULLPTR);

private slots:
    void rts2Updated(QJsonDocument &doc);

private:
    QString device;
    QString var;
};

/**
 * Widget configured from XML data.
 */
class QRWidget : public QWidget
{
public:
    QRWidget(QXmlStreamReader *xml);
};

#endif // QRWIDGET_H
