#include <QUrlQuery>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>

#include "rts2q_device.h"
#include "config.h"

Rts2QDevice::Rts2QDevice (QString devname, QWidget *parent):QWidget (parent)
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

	QUrl rurl(Config::getInstance().baseurl);

	rurl.setPath("/api/get");

	QUrlQuery query;
	query.addQueryItem("d", devname);
	rurl.setQuery(query);

	request.setUrl(rurl);
}

void Rts2QDevice::slotRefresh()
{
	reply = Config::getInstance().networkManager.get(request);
	connect(reply, SIGNAL(finished()), this, SLOT(finished()));

	m_button->setText(tr("Pushed"));
}

void Rts2QDevice::finished()
{
	qDebug() << "reply errors" << reply->error();
	if (reply->error() != QNetworkReply::NoError)
		return;

	m_button->setText("--");

	QByteArray data = reply->readAll();
	qDebug() << "RTS2 device " << thisdevice << " data:" << (QString) data;

	QJsonDocument doc;
	QJsonParseError jsonError;
	doc = QJsonDocument::fromJson(data, &jsonError);

	QJsonObject docObject = doc.object();
	QJsonObject dArray = docObject["d"].toObject();
	int r = 0;
	foreach (const QString & val, dArray.keys())
	{
		m_table->insertRow(m_table->rowCount());
		m_table->setItem(r, 0, new QTableWidgetItem(val));
		m_table->setItem(r, 1, new QTableWidgetItem(dArray[val].toString()));
		r++;
	};
}
