#include <QApplication>

#include "MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("xalanq");
    QCoreApplication::setApplicationName("Week1 Assignment");

    MainWindow w;
    w.show();

    return a.exec();
}
