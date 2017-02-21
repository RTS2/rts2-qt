// Vizier client

#ifndef __RTS2Q_VIZIER
#define __RTS2Q_VIZIER

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QHostAddress>
#include <QVector>

#define MAX_LINE  200

class Rts2QStar
{
	public:
		Rts2QStar ();
		double ra;
		double dec;
};

class Rts2QVizier:public QObject
{
	Q_OBJECT
	public:
		Rts2QVizier ();

		void runQuery(double ra, double dec);

	public slots:
		void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
		void readyRead();
		void replyFinished();

	private:
		QVector <Rts2QStar> stars;

		QNetworkAccessManager networkManager;
		QUrl baseurl;
		QNetworkRequest request;
		QNetworkReply *reply;
		QByteArray data;
		char lineData[MAX_LINE];
		char *lineStart;

		void processLine();
};

#endif //!__RTS2Q_VIZIER
