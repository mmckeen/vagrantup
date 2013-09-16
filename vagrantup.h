#ifndef VAGRANTUP_H
#define VAGRANTUP_H

#include <QMainWindow>

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

private:
    void checkVagrant();
    Ui::VagrantUp *ui;
};

#endif // VAGRANTUP_H
