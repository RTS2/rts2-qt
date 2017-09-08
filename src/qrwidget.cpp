#include <QGridLayout>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>

#include "qrwidget.h"
#include "qrapp.h"
#include "qrtelescope.h"

QRLineEdit::QRLineEdit(const QString &_device, const QString &_var, QWidget *parent) :
    QLineEdit(parent),
    device(_device),
    var(_var)
{
    connect(&QRApp::getInstance(), &QRApp::rts2Updated, this, &QRLineEdit::rts2Updated);
}

void
QRLineEdit::rts2Updated(QJsonDocument &doc)
{
    QJsonObject docObject = doc.object();
    QJsonObject devArray = docObject[device].toObject();
    QJsonObject dArray = devArray["d"].toObject();

    setText(dArray[var].toArray()[1].toVariant().toString());
}

QRWidget::QRWidget(QXmlStreamReader *xml)
{
    QGridLayout *layout = new QGridLayout;

    int i = 0;

    while (xml->readNextStartElement()) {
        if (xml->name() == "edit")
	   layout->addWidget(new QRLineEdit(
	       xml->attributes().value("device").toString(),
	       xml->attributes().value("var").toString(),
	       this
	   ), i, 0);
	
	else if (xml->name() == "telescope")
	   layout->addWidget(new QRTelescope(
	       xml->attributes().value("device").toString()
	   ), i, 0);

	xml->skipCurrentElement();
	i++;
    }

    setLayout(layout);
}
