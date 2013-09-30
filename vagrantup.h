#ifndef VAGRANTUP_H
#define VAGRANTUP_H

#include <QMainWindow>
#include <QString>
#include <QListWidgetItem>
#include <QSettings>

namespace Ui {
class VagrantUp;
}

class VagrantUp : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit VagrantUp(QWidget *parent = 0);
    ~VagrantUp();

private slots:
    void on_add_pressed();

    void on_remove_pressed();

    void on_listWidget_itemSelectionChanged();

    void on_refresh_pressed();

    void on_listWidget_itemActivated(QListWidgetItem *item);

    void on_up_clicked();

private:
    void checkVagrant();
    void loadSettings();
    void processFilename(QString filename);
    void updateSettings();
    void bring_up_vagrantfiles(QList<QListWidgetItem*>* vagrantFiles);
    QSettings *settings;
    QStringList *vagrantFilenames;
    Ui::VagrantUp *ui;
};

#endif // VAGRANTUP_H
