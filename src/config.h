#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <QNetworkAccessManager>
#include <QUrl>

/**
 * Singleton class for connection configuration.
 *
 * @author Petr Kubanek <kubanek@fzu.cz>
 */
class Config
{

public:
    Config(const QString &url, const QString &login, const QString &password);

    QNetworkAccessManager networkManager;
    QUrl baseurl;

    static Config& getInstance() {Q_ASSERT(singleton); return *singleton;}

private:
    static Config* singleton;
};

#endif // !__CONFIG_H__
