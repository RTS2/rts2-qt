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
		void rts2Command(const QString &device, const QString &command);
		virtual void rts2SetValue(const QString &device, const QString &name, const QString &value);
		virtual void rts2SetValue(const QString &device, const QString &name, const double v1, const double v2);

		virtual void rts2IncValue(const QString &device, const QString &name, const QString &value);
		virtual void rts2IncValue(const QString &device, const QString &name, const double v1, const double v2);

	signals:
		void rts2Updated(QJsonDocument &doc);

	private slots:
		void finished();

	private:
		virtual void rts2SetValue(const QString &op, const QString &device, const QString &name, const QString &value);

		static QRApp* singleton;

		QNetworkRequest request;
		QNetworkReply* reply;
};

#endif // !__RTS2Q_APP__
