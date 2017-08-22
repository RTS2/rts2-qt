#include "config.h"

Config* Config::singleton = NULL;

Config::Config(const QString &url, const QString &login, const QString &password): baseurl(url)
{
	Q_ASSERT(!singleton);
	singleton = this;

	baseurl.setUserName(login);
	baseurl.setPassword(password);
}
