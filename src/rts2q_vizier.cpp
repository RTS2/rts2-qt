#include "rts2q_vizier.h"

#include <QUrlQuery>
#include <QLocale>
#include <math.h>

Rts2QStar::Rts2QStar()
{
	ra = NAN;
	dec = NAN;
	mag = NAN;
}

Rts2QStar::Rts2QStar(double _ra, double _dec, float _mag)
{
	ra = _ra;
	dec = _dec;
	mag = _mag;
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
	query.addQueryItem("-c", QString("%1 %2").arg(ra/15.0).arg(dec));
	query.addQueryItem("-c.rs", QString("%1").arg(30));
	rurl.setQuery(query);

	request.setUrl(rurl);

	qDebug() << rurl.toString();

	lineStart = lineData;
	lineSeen = START;

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

	if (lineData[0] == '-')
	{
		// find spaces
		if (lineSeen == START)
		{
			int lastC = 0;
			int i = 0;
			for (i = 0; i < strlen(lineData); i++)
			{
				if (lineData[i] == ' ')
				{
					collLengths.append(i - lastC);
					lastC = i + 1;
					collumns.append(QString());
				}
			}
			collLengths.append(i - lastC);
			collumns.append(QString());
			lineSeen = COLLUNS;
		}
		else if (lineSeen == COLLUNS)
		{
			// dump collumns
			auto il = collLengths.begin();
			auto in = collumns.begin();
			for (; il != collLengths.end() && in != collumns.end(); il++, in++)
			{
				qDebug() << "collumn length " << *il << " " << *in;
			}
			lineSeen = DATA;
		}
	}
	else if (lineSeen == COLLUNS)
	{
		int i = 0;
		int lastL = 0;
		qDebug() << lineData;
		for (auto iter = collLengths.begin(); iter != collLengths.end(); iter++)
		{
			int cl = collLengths[i];
			while (cl > 0 && lineData[lastL + cl] == ' ')
			{
				lineData[lastL + cl] = '\0';
				cl--;
			}
			if (cl >= 0 && lineData[lastL] != ' ')
			{
				collumns[i].append(lineData + lastL);
			}
			lastL += collLengths[i] + 1;
			i++;
		}
	}
	else if (lineSeen == DATA)
	{
		if (lineData[0] == '\0')
		{
			lineSeen = END;
			return;
		}
		qDebug() << lineData;
		int tl = 0;
		for (auto il = collLengths.begin(); il != collLengths.end(); il++)
		{
			lineData[tl + *il] = '\0';
			tl += *il + 1;
		}
		QLocale loc("C");
		bool ok;
		Rts2QStar star(loc.toDouble(lineData + collLengths[0] + 1, &ok), loc.toDouble(lineData + collLengths[0] + collLengths[1] + 2), 1);
		qDebug() << "star ra " << star.ra << " " << star.dec << lineData + collLengths[0] + 1;

		stars.append(star);

		emit starAdded();
	}
}
