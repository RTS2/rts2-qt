#include <QApplication>
#include <QNetworkAccessManager>

#include "qrcat.h"
#include "qrdevice.h"
#include "config.h"
 
int main(int argc, char **argv)
{
	QApplication app (argc, argv);

	Config* config = new Config("http://localhost:8889", "rts2test", "test");

	QRDevice dev("T0");
	dev.show();

	QRCat cat(300,0);
	cat.show();
 
	int ret = app.exec();

	delete config;

	return ret;
}
