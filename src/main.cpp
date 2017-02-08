#include <QApplication>

#include "rts2q_device.h"
 
int main(int argc, char **argv)
{
 QApplication app (argc, argv);

 Rts2QDevice dev ("T0");
 dev.show();
 
 return app.exec();
}
