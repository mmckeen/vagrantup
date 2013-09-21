#include "vagrantup.h"
#include "ui_vagrantup.h"

#include <QFileDialog>
#include <QSettings>
#include <QFile>
#include <QTextStream>

VagrantUp::VagrantUp(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::VagrantUp)
{
    ui->setupUi(this);

    this->statusBar()->setSizeGripEnabled(false);

    checkVagrant();

}

void VagrantUp::checkVagrant()
{

 QString command = "vagrant -h";

 int status = system(command.toLocal8Bit().data());

 if (status) {
    ui->statusBar->showMessage("Vagrant is not installed.");
 } else {
    ui->statusBar->showMessage("Vagrant is installed and ready.");
 }

}

VagrantUp::~VagrantUp()
{
    delete ui;
}

void VagrantUp::on_add_pressed()
{

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Vagrantfile"),"~/",tr("Vagrantfile (Vagrantfile)"));

    QString enviroName = "";

    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);

    QString line = in.readLine();

    while ( !in.atEnd() )
    {
        if(line.contains("config.vm.hostname"))
        {
            enviroName = line.mid(line.indexOf("\"")+1);
            enviroName = enviroName.mid(0,enviroName.indexOf("\""));
        }

        line = in.readLine();
    }

    if (enviroName == "")
        enviroName = fileName;

    ui->listWidget->addItem(enviroName);

}

void VagrantUp::on_remove_pressed()
{
    qDeleteAll(ui->listWidget->selectedItems());
}

void VagrantUp::on_listWidget_itemSelectionChanged()
{
    if(ui->listWidget->selectedItems().length() > 0)
        ui->remove->setEnabled(true);
    else
        ui->remove->setEnabled(false);
}

void VagrantUp::on_refresh_pressed()
{
    checkVagrant();
}
