#include "qrapp.h"

QRApp* QRApp::singleton = NULL;

QRApp::QRApp()
{
	Q_ASSERT(!singleton);
	singleton = this;
}
