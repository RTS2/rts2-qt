#include <QApplication>
#include <QNetworkAccessManager>

#include "qrapp.h"
#include "qrcat.h"
#include "qrdevice.h"
#include "qrlogin.h"
#include "config.h"
 
int
main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QRApp *qapp = new QRApp();

    QRLogin *qlogin = new QRLogin();
    int ret = qlogin->exec();

    if (ret == QDialog::Rejected)
    {
	return 1;
    }

    Config* config = new Config(qlogin->getServer(), qlogin->getLogin(), qlogin->getPassword());

    delete qlogin;

    QRDevice dev("T0");
    dev.show();

    QRCat cat(300,0);
    cat.show();
 
    ret = app.exec();

    delete config;
    delete qapp;

    return ret;
}
