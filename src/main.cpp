#include <QApplication>

#include "rts2q_cat.h"
#include "rts2q_device.h"
 
int main(int argc, char **argv)
{
 QApplication app (argc, argv);

 Rts2QDevice dev("T0");
 dev.show();

 Rts2QCat cat(300,90);
 cat.show();
 
 return app.exec();
}
