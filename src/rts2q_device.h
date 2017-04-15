/*
 * Class representing RTS2 device.
 *
 * @author Petr Kubanek <kubanek@fzu.cz>
 */

#ifndef __RTS2Q_DEVICE__
#define __RTS2Q_DEVICE__

#include <QWidget>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QPushButton>

#include <QNetworkRequest>
#include <QNetworkReply>

class Rts2QDevice:public QWidget
{
	Q_OBJECT
	public:
		Rts2QDevice (QString devname, QWidget *parent = 0);
	private slots:
		void slotRefresh ();
		void finished();
	private:
		QString thisdevice;

		QVBoxLayout *m_layout;

		QTableWidget *m_table;

		QPushButton *m_button;

		QNetworkRequest request;
		QNetworkReply* reply;
};

#endif //!__RTS2Q_DEVICE__
