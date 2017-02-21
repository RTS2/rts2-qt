#include "rts2q_vizier.h"

#include <QUrlQuery>
#include <math.h>

Rts2QStar::Rts2QStar()
{
	ra = NAN;
	dec = NAN;
}

Rts2QVizier::Rts2QVizier():QObject(), baseurl("http://vizier.u-strasbg.fr/viz-bin/asu-txt")
{
}

void Rts2QVizier::runQuery(double ra, double dec)
{
	QUrl rurl(baseurl);

	QUrlQuery query;
	query.addQueryItem("-source", "I/305"); // catalogue name
	query.addQueryItem("-c", QString("%1 %2").arg(ra).arg(dec));
	query.addQueryItem("-c.rs", QString("%1").arg(300));
	rurl.setQuery(query);

	request.setUrl(rurl);

	reply = networkManager.get(request);
	connect(reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(downloadProgress(qint64,qint64)));
	connect(reply, SIGNAL(finished()), this, SLOT(replyFinished()));
}

void Rts2QVizier::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
	qDebug() << bytesReceived << "/" << bytesTotal;
	data.append(reply->read(bytesReceived - data.size()));
}

void Rts2QVizier::replyFinished()
{
	qDebug() << "Vizier reply errors" << reply->error();

	data.append(reply->readAll());
	qDebug() << data.data();

	reply->deleteLater();
}
