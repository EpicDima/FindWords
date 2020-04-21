#include <QApplication>
#include <QDebug>
#include <MainWindow.h>

int main(int argv, char **args) {
    QApplication app(argv, args);
    MainWindow w;
    w.show();
    return QApplication::exec();
}