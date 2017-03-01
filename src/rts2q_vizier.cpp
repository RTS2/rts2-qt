#include "rts2q_vizier.h"

#include <QUrlQuery>
#include <QLocale>

#include <libnova/libnova.h>
#include <math.h>

Rts2QConditions::Rts2QConditions (double _ra0, double _dec0)
{
	ra0 = _ra0;
	dec0 = _dec0;

	cos_phi1 = cos (ln_deg_to_rad (dec0));
	sin_phi1 = sin (ln_deg_to_rad (dec0));

	scale = 20 * 3600;
}

Rts2QStar::Rts2QStar()
{
	name = QString();
	ra = NAN;
	dec = NAN;
	mag = NAN;
}

Rts2QStar::Rts2QStar(QString _name, double _ra, double _dec, float _mag) : name(_name)
{
	ra = _ra;
	dec = _dec;
	mag = _mag;
}

void Rts2QStar::azimuthalEqualArea(Rts2QConditions *conditions, double &x, double &y)
{
	double kp, phi, lam, rho;
  
	phi = ln_deg_to_rad (dec);
	lam = ln_deg_to_rad (ra - conditions->ra0);

	if (conditions->dec0 == 90)
	{
		rho = 2 * conditions->scale * sin (M_PI/4 - phi/2);
		x =  rho * sin (lam);
		y = -rho * cos (lam);
	}
	else if (conditions->dec0 == -90)
	{
		rho = 2 * conditions->scale * cos (M_PI/4 - phi/2);
		x =  rho * sin (lam);
		y =  rho * cos (lam);
	}
	else
	{
		double sin_phi = sin (phi);
		double cos_phi = cos (phi);
		double cos_lam = cos (lam);

		kp = sqrt (2. / (1 + conditions->sin_phi1 * sin_phi + conditions->cos_phi1 * cos_phi * cos_lam));
		x = conditions->scale * kp * cos (phi) * sin (lam);
		y = conditions->scale * kp * (conditions->cos_phi1 * sin_phi - conditions->sin_phi1 * cos_phi * cos_lam);
	}
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
	query.addQueryItem("-c", QString("%1 %2").arg(ra,0,'f',10).arg(dec,0,'f',10));
	query.addQueryItem("-c.rs", QString("%1").arg(500));
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

void Rts2QVizier::inverseAzimuthalEqualArea(Rts2QConditions *conditions, double x, double y, double &ra, double &dec)
{
	double phi, lam, rho, c;

	rho = sqrt(x * x + y * y);

	if (rho != 0.0)
	{
		c = 2 * asin (rho / (2 * conditions->scale));
    
		phi = asin (cos(c) * conditions->sin_phi1 + y * sin(c) * conditions->cos_phi1 / rho);
    
		if (conditions->dec0 == 90)
			lam = atan2 (x, -y);
		else if (conditions->dec0 == -90)
			lam = atan2 (x, y);
		else
			lam = atan2 (x * sin(c), rho * conditions->cos_phi1 * cos(c) - y * conditions->sin_phi1 * sin(c));
	}
	else
	{
		phi = ln_rad_to_deg (conditions->dec0);
		lam = 0.0;
	}
  
	dec = ln_rad_to_deg (phi);
	ra = ln_rad_to_deg (lam) + conditions->ra0;
	ra = ln_range_degrees (ra);
}

Rts2QStar Rts2QVizier::getClosest(double ra, double dec)
{
	double dist = NAN;
	auto found = stars.begin();
	struct ln_equ_posn orig;
	orig.ra = ra;
	orig.dec = dec;
	for (auto si = stars.begin(); si != stars.end(); si++)
	{
		struct ln_equ_posn spos;
		spos.ra = si->ra;
		spos.dec = si->dec;
		double sd = ln_get_angular_separation(&orig, &spos);
		if (sd < dist || std::isnan(dist))
		{
			found = si;
			dist = sd;
		}
	}
	return *(found);
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
			int i = 0;
			for (i = 0; i < strlen(lineData); i++)
			{
				if (lineData[i] == ' ')
				{
					collIndex.append(i);
					collumns.append(QString());
				}
			}
			collIndex.append(i);
			collumns.append(QString());
			lineSeen = COLLUMNS;
		}
		else if (lineSeen == COLLUMNS)
		{
			// dump collumns
			auto il = collIndex.begin();
			auto in = collumns.begin();
			for (; il != collIndex.end() && in != collumns.end(); il++, in++)
			{
				qDebug() << "collumn index " << *il << " " << *in;
			}
			lineSeen = DATA;
		}
	}
	else if (lineSeen == COLLUMNS)
	{
		qDebug() << lineData;
		int lastI = 0;
		int i = 0;
		for (auto iter = collIndex.begin(); iter != collIndex.end(); iter++)
		{
			int cl = *iter;
			while (cl > lastI && lineData[cl] == ' ')
			{
				lineData[cl] = '\0';
				cl--;
			}
			if (cl >= lastI && lineData[lastI] != ' ')
			{
				collumns[i].append(lineData + lastI);
			}
			lastI = *iter + 1;
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
		for (auto il = collIndex.begin(); il != collIndex.end(); il++)
		{
			lineData[*il] = '\0';
		}
		QLocale loc("C");
		bool ok;
		float m;
		if (lineData[collIndex[4] + 1] != ' ')
			m = loc.toDouble(lineData + collIndex[4] + 1, &ok);
		else
			m = NAN;

		Rts2QStar star(lineData, loc.toDouble(lineData + collIndex[0] + 1, &ok), loc.toDouble(lineData + collIndex[1] + 1), m);
		qDebug() << "star ra " << star.ra << " dec " << star.dec << " mag " << m;

		stars.append(star);

		emit starAdded();
	}
}
