#include "config.h"

Config* Config::singleton = NULL;

Config::Config(const char *url, const char *login, const char *password): baseurl(url)
{
	Q_ASSERT(!singleton);
	singleton = this;

	baseurl.setUserName(login);
	baseurl.setPassword(password);
}
