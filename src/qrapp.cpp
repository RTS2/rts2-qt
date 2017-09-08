#include "qrapp.h"
#include "config.h"

#include <QUrlQuery>
#include <QJsonDocument>

QRApp* QRApp::singleton = NULL;

QRApp::QRApp()
{
    Q_ASSERT(!singleton);
    singleton = this;
}

void
QRApp::rts2Update()
{
    QUrl rurl(Config::getInstance().baseurl);

    rurl.setPath("/api/getall");
    rurl.setQuery("e=1");

    request.setUrl(rurl);
    reply = Config::getInstance().networkManager.get(request);
    connect(reply, SIGNAL(finished()), this, SLOT(finished()));
}

void
QRApp::rts2Command(const QString &device, const QString &command)
{
    QUrl rurl(Config::getInstance().baseurl);

    rurl.setPath("/api/cmd");

    QUrlQuery query;
    query.addQueryItem("d", device);
    query.addQueryItem("c", command);

    rurl.setQuery(query);

    request.setUrl(rurl);
    reply = Config::getInstance().networkManager.get(request);
    //connect(reply, SIGNAL(finished()), this, SLOT(finished()));
}

void
QRApp::rts2SetValue(const QString &device, const QString &name, const QString &value)
{
    rts2SetValue("set", device, name, value);
}

void
QRApp::rts2SetValue(const QString &device, const QString &name, const double v1, const double v2)
{
    rts2SetValue(device, name, QString("%1 %2").arg(v1).arg(v2));
}

void
QRApp::rts2IncValue(const QString &device, const QString &name, const QString &value)
{
    rts2SetValue("inc", device, name, value);
}

void
QRApp::rts2IncValue(const QString &device, const QString &name, const double v1, const double v2)
{
    rts2IncValue(device, name, QString("%1 %2").arg(v1).arg(v2));
}

void
QRApp::rts2SetValue(const QString &op, const QString &device, const QString &name, const QString &value)
{
    QUrl rurl(Config::getInstance().baseurl);

    rurl.setPath(QString("/api/%1").arg(op));

    QUrlQuery query;
    query.addQueryItem("d", device);
    query.addQueryItem("n", name);
    query.addQueryItem("v", value);

    rurl.setQuery(query);

    request.setUrl(rurl);
    reply = Config::getInstance().networkManager.get(request);
}

void
QRApp::finished()
{
    if (reply->error() != QNetworkReply::NoError)
    {
        qCritical() << "cannot recieve JSON reply: " << reply->errorString();
        return;
    }
    QByteArray data = reply->readAll();

    QJsonDocument doc;
    QJsonParseError jsonError;

    //qDebug() << "data " << data;

    doc = QJsonDocument::fromJson(data, &jsonError);

    emit rts2Updated(doc);
}
