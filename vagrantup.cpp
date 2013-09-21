#include "vagrantup.h"
#include "ui_vagrantup.h"

#include <QFileDialog>
#include <QSettings>
#include <QInputDialog>
#include <QSettings>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDebug>

VagrantUp::VagrantUp(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::VagrantUp)
{
    ui->setupUi(this);

    this->statusBar()->setSizeGripEnabled(false);

    settings = new QSettings("VagrantUp");
    vagrantFilenames = new QStringList();

    loadSettings();

    checkVagrant();

}

void VagrantUp::loadSettings()
{
    QString vagrantfiles = settings->value("vagrantfiles").toString();

    QString debug_message = "Loading previous Vagrantfiles: "
            + settings->value("vagrantfiles").toString();

    qDebug(debug_message.toStdString().c_str());

    QString current_file;

    for( int i = 0; i < vagrantfiles.length(); i++ )
    {
        if(vagrantfiles.at(i) == ',' ) {
            processFilename(current_file);
            current_file.clear();
        } else {
            current_file.append(vagrantfiles.at(i));
        }
    }

    processFilename(current_file);

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

void VagrantUp::bring_up_vagrantfiles(QList<QListWidgetItem*>* vagrantFiles)
{

}

void VagrantUp::processFilename(QString filename)
{
    QString debugmsg = "Processing filename '" + filename +"'";

    qDebug(debugmsg.toStdString().c_str());

    QString enviroName = "";

    QFile file(filename);

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
        enviroName = filename;

    ui->listWidget->addItem(enviroName);
    vagrantFilenames->append(filename);

    // create of string of comma seperated values to serialize the list of vagrantfiles
    // it is hacky, but it works just fine

    QString serializedFilenames;

    for (int i = 0; i < vagrantFilenames->length(); i++)
    {
        serializedFilenames.append(vagrantFilenames->at(i));

        if (i < vagrantFilenames->length() - 1 ) {
            serializedFilenames.append(QString(","));
        }
    }

    settings->setValue("vagrantfiles",settings->value("vagrantfiles",serializedFilenames));

}

void VagrantUp::on_add_pressed()
{

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Vagrantfile"),"~/",tr("Vagrantfile (Vagrantfile)"));

    processFilename(fileName);

}

void VagrantUp::on_remove_pressed()
{

    qDeleteAll(ui->listWidget->selectedItems());

    if(ui->listWidget->count() == 0) {
        ui->remove->setEnabled(false);
        ui->up->setEnabled(false);
    }
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

void VagrantUp::on_listWidget_itemActivated(QListWidgetItem *item)
{
   ui->up->setEnabled(true);
}

void VagrantUp::on_up_clicked()
{
    QString command('vagrant up');
    QList<QListWidgetItem*> vagrantFiles = ui->listWidget->selectedItems();

    if (vagrantFiles.length() > 1)
    {
        QMessageBox::StandardButton reply;
         reply = QMessageBox::question(this, "Multiple Vagrantfiles Selected",
            "Are you sure you want to bring up mutliple Vagrant environments?",

            QMessageBox::Yes|QMessageBox::No);

         if (reply == QMessageBox::Yes) {
             bring_up_vagrantfiles(&vagrantFiles);
         } else {
         // Do nothing, fall back to Vagrantfile list
         }

    }


}
