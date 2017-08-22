#include "qrapp.h"
#include "config.h"

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

    qDebug() << "data " << data;

    doc = QJsonDocument::fromJson(data, &jsonError);

    emit rts2Updated(doc);
}
