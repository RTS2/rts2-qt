#ifndef QRLOGIN_H
#define QRLOGIN_H

#include "ui_loginDialog.h"

#include <QDialog>
#include <QSettings>

namespace Ui {
class QRLogin;
}

class QRLogin : public QDialog
{
    Q_OBJECT

public:
    explicit QRLogin(QWidget *parent = 0);
    ~QRLogin();

    QString getServer() { return ui->server->text(); }
    QString getLogin() { return ui->login->text(); }
    QString getPassword() { return ui->password->text(); }

private:
    Ui::QRLogin *ui;
    QSettings settings;

    void load();
    void save();

private slots:
    void currentIndexChanged(int index);
    void accepted();
};

#endif // QRLOGIN_H
