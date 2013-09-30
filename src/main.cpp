#include "vagrantup.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    VagrantUp w;
    w.show();
    
    return a.exec();
}
