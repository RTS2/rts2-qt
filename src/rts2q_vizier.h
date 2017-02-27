// Vizier client

#ifndef __RTS2Q_VIZIER
#define __RTS2Q_VIZIER

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QHostAddress>
#include <QVector>

#define MAX_LINE  200

class Rts2QConditions
{
	public:
		Rts2QConditions (double _ra0, double _dec0);

		double ra0;
		double dec0;

		double sin_phi1;
		double cos_phi1;

		double scale;

		void changeScale(double change) { scale += change; }

};

class Rts2QStar
{
	public:
		Rts2QStar ();
		Rts2QStar (QString _name, double _ra, double _dec, float _mag);

		void azimuthalEqualArea (Rts2QConditions *conditions, double &x, double &y);

		QString name;
		double ra;
		double dec;
		float mag;
};

class Rts2QVizier:public QObject
{
	Q_OBJECT
	public:
		Rts2QVizier ();

		void runQuery(double ra, double dec);

		QVector <Rts2QStar> stars;

		void inverseAzimuthalEqualArea(Rts2QConditions *conditions, double x, double y, double &ra, double &dec);
		Rts2QStar getClosest(double x, double y);

	signals:
		void starAdded();

	public slots:
		void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
		void readyRead();
		void replyFinished();

	private:
		QNetworkAccessManager networkManager;
		QUrl baseurl;
		QNetworkRequest request;
		QNetworkReply *reply;
		QByteArray data;
		char lineData[MAX_LINE];
		char *lineStart;
		enum {START, COLLUNS, DATA, END} lineSeen;

		QVector <QString> collumns;
		QVector <int> collLengths;

		void processLine();
};

#endif //!__RTS2Q_VIZIER
