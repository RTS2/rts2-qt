#include <QWidget>

#ifndef __RTS2Q_APP__
#define __RTS2Q_APP__

#include <QNetworkRequest>
#include <QNetworkReply>

/**
 * Singleton class for all values sourced from RTS2. Rts2Widgets can connect
 * to rts2Update signal
register
 * to app class to have rts2Updated called when updated value is fetched
 * from RTS2. 
 *
 * @author Petr Kubanek <kubanek@fzu.cz>
 */

class QRApp:public QObject
{
	Q_OBJECT
	public:
		QRApp();

		static QRApp& getInstance() {Q_ASSERT(singleton); return *singleton;}
		void rts2Update();

	signals:
		void rts2Updated(QJsonDocument &doc);

	private slots:
		void finished();

	private:

		static QRApp* singleton;

		QNetworkRequest request;
		QNetworkReply* reply;
};

#endif // !__RTS2Q_APP__
