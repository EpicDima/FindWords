//
// Created by Dima on 19.01.2020.
//

#ifndef FINDWORDS_MAINWINDOW_H
#define FINDWORDS_MAINWINDOW_H

#include <QtWidgets>

#include "Solver.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

public slots:
    void openDictionary();
    void changeRowAndColumnCount();
    void calculate();
    
private:
    Ui::MainWindow *ui;

    Solver* solver;

    static std::pair<uint64_t, uint64_t> getRowsAndColumns(QString& text);
};

#endif //FINDWORDS_MAINWINDOW_H