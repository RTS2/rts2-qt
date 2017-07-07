#include <QUrlQuery>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>

#include "qrapp.h"
#include "qrdevice.h"
#include "config.h"

QRDevice::QRDevice (QString devname, QWidget *parent):QWidget (parent)
{
	thisdevice = devname;

	m_layout = new QVBoxLayout(this);

	m_table = new QTableWidget(0,2);
	m_table->setItem(0,0,new QTableWidgetItem("test"));

	m_button = new QPushButton(tr("&Retrieve"));
	m_button->setGeometry(QRect(QPoint(100,100), QSize(200,50)));

	m_layout->addWidget(m_table);
	m_layout->addWidget(m_button);

	connect(m_button, SIGNAL(clicked()), this, SLOT(slotRefresh()));
	connect(&QRApp::getInstance(), &QRApp::rts2Updated, this, &QRDevice::rts2Updated);
}

void QRDevice::slotRefresh()
{
	QRApp::getInstance().rts2Update();
	m_button->setText(tr("Pushed"));
}

void QRDevice::rts2Updated(QJsonDocument &doc)
{
	QJsonParseError jsonError;

	QJsonObject docObject = doc.object();
	QJsonObject devArray = docObject["centrald"].toObject();
	QJsonObject dArray = devArray["d"].toObject();
	int r = 0;
	foreach (const QString & val, dArray.keys())
	{
		m_table->insertRow(m_table->rowCount());
		m_table->setItem(r, 0, new QTableWidgetItem(val));
		m_table->setItem(r, 1, new QTableWidgetItem(dArray[val].toString()));
		r++;
	};
}
