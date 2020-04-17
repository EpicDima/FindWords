//
// Created by Dima on 19.01.2020.
//

#include <QApplication>
#include <QDebug>
#include <ui/MainWindow.h>

int main(int argv, char **args) {
    QApplication app(argv, args);
    MainWindow w;
    w.show();
    return QApplication::exec();
}
