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
	reply = NULL;
	lineStart = lineData;
}

void Rts2QVizier::runQuery(double ra, double dec)
{
	if (reply)
		delete reply;

	QUrl rurl(baseurl);

	QUrlQuery query;
	query.addQueryItem("-source", "I/305"); // catalogue name
	query.addQueryItem("-c", QString("%1 %2").arg(ra).arg(dec));
	query.addQueryItem("-c.rs", QString("%1").arg(300));
	rurl.setQuery(query);

	request.setUrl(rurl);

	lineStart = lineData;

	reply = networkManager.get(request);
	connect(reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(downloadProgress(qint64,qint64)));
	connect(reply, SIGNAL(readyRead()), this, SLOT(readyRead()));
	connect(reply, SIGNAL(finished()), this, SLOT(replyFinished()));
}

void Rts2QVizier::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
	qDebug() << bytesReceived << "/" << bytesTotal;
}

void Rts2QVizier::readyRead()
{
	while(true)
	{
		if ((lineStart - lineData) >= sizeof(lineData))
		{
			qWarning() << "too long line" << lineData;
			return;
		}
		qint64 lineLength = reply->readLine(lineStart, sizeof(lineData) - (lineStart - lineData));
		if (lineLength <= 0)
			return;

		lineStart += lineLength;
		if (lineStart[-1] == '\n')
		{
			processLine();
			lineStart = lineData;
		}
	}
}

void Rts2QVizier::replyFinished()
{
	qDebug() << "Vizier reply errors" << reply->error();

	readyRead();

	reply->deleteLater();
}

void Rts2QVizier::processLine()
{
	if (lineStart[-1] == '\n')
		lineStart[-1] = '\0';
	qDebug() << lineData;
}
