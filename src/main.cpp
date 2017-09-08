#include <QApplication>
#include <QHBoxLayout>
#include <QNetworkAccessManager>
#include <QMessageBox>

#include "qrapp.h"
#include "qrcat.h"
#include "qrdevice.h"
#include "qrlogin.h"
#include "qrwidget.h"
#include "config.h"
 
int
main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QRApp *qapp = new QRApp();

    QFile file("rts.form");

    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(NULL, qapp->tr("QRTS2"),
            qapp->tr("Cannot read file %1:\n%2.").arg("rts.form").arg(file.errorString()));
        return -1;
    }

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
   
    QDialog testDialog;

    QXmlStreamReader xml;
    xml.setDevice(&file);

    Q_ASSERT(xml.readNextStartElement());

    Q_ASSERT(xml.isStartElement() && xml.name() == "rtsform");

    QHBoxLayout l;
    l.addWidget(new QRWidget(&xml));

    testDialog.setLayout(&l);
    testDialog.show();

    ret = app.exec();

    delete config;
    delete qapp;

    return ret;
}
