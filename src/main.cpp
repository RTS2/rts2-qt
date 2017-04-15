#include <QApplication>
#include <QNetworkAccessManager>

#include "rts2q_cat.h"
#include "rts2q_device.h"
#include "config.h"
 
int main(int argc, char **argv)
{
	QApplication app (argc, argv);

	Config* config = new Config("http://localhost:8889", "rts2test", "test");

	Rts2QDevice dev("T0");
	dev.show();

	Rts2QCat cat(300,0);
	cat.show();
 
	int ret = app.exec();

	delete config;

	return ret;
}
