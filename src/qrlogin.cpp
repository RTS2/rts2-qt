#include "qrlogin.h"

#include <QDebug>
#include <QPushButton>
#include <QDialogButtonBox>

QRLogin::QRLogin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QRLogin),
    settings("rts2", "logins")
{
    ui->setupUi(this);

    load();

    connect(ui->name, SIGNAL(currentIndexChanged(int)), this, SLOT(currentIndexChanged(int)));
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(accepted()));

    QPushButton *db = new QPushButton(tr("&Delete"));
    ui->buttonBox->addButton(db, QDialogButtonBox::ActionRole);
    connect(db, SIGNAL(clicked()), this, SLOT(deleteClicked()));
}

QRLogin::~QRLogin()
{
    delete ui;
}

void
QRLogin::load()
{
    QStringListIterator groups(settings.childGroups());
    while (groups.hasNext())
        ui->name->addItem(groups.next());
    if (!ui->name->currentText().isEmpty())
	currentIndexChanged(0);
}

void
QRLogin::save()
{
    settings.beginGroup(ui->name->currentText());
    settings.setValue("server", getServer());
    settings.setValue("login", getLogin());
    settings.setValue("password", getPassword());
    settings.endGroup();
}

void
QRLogin::remove()
{
    settings.beginGroup(ui->name->currentText());
    settings.remove("");
    settings.endGroup();
    ui->name->clear();
    load();
}

void
QRLogin::currentIndexChanged(int index)
{
    settings.beginGroup(ui->name->currentText());
    ui->server->setText(settings.value("server").toString());
    ui->login->setText(settings.value("login").toString());
    ui->password->setText(settings.value("password").toString());
    settings.endGroup();
}

void
QRLogin::accepted()
{
    qDebug() << "accepted";
    if (!ui->name->currentText().isEmpty())
        save();
}

void
QRLogin::deleteClicked()
{
    remove();
}
