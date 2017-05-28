// Vizier client

#ifndef __RTS2Q_VIZIER
#define __RTS2Q_VIZIER

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QHostAddress>
#include <QVector>

#define MAX_LINE  200

class QRConditions
{
	public:
		QRConditions (double _ra0, double _dec0);

		double ra0;
		double dec0;

		double sin_phi1;
		double cos_phi1;

		double scale;

		void changeScale(double change) { scale += change; }

};

class QRStar
{
	public:
		QRStar ();
		QRStar (QString _name, double _ra, double _dec, float _mag);

		void azimuthalEqualArea (QRConditions *conditions, double &x, double &y);

		QString name;
		double ra;
		double dec;
		float mag;
};

class QRVizier:public QObject
{
	Q_OBJECT
	public:
		QRVizier ();

		void runQuery(double ra, double dec);

		QVector <QRStar> stars;

		void inverseAzimuthalEqualArea(QRConditions *conditions, double x, double y, double &ra, double &dec);
		QRStar* getClosest(double x, double y);

		bool findStarName(QString name);

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
		enum {START, COLLUMNS, DATA, END} lineSeen;

		QVector <QString> collumns;
		QVector <int> collIndex;

		void processLine();
};

#endif //!__RTS2Q_VIZIER
