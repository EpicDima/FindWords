//
// Created by Dima on 19.01.2020.
//

#ifndef FINDWORDS_MAINWINDOW_H
#define FINDWORDS_MAINWINDOW_H

#include <QtWidgets>

#include "Solver.h"
#include "singleworddialog.h"


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
    void minSpinBoxChange();
    void maxSpinBoxChange();
    void showWord();
    void showCombination();
    void showCombinationWord();

    void showConfigurationScreen();
    void showWordsScreen();
    void showCombinationsScreen();
    void showCombinationWordsScreen();

    
private:
    static const inline std::uint32_t COLORS = 14;
    static const inline std::u16string BACKGROUND[COLORS] = {
        u"white", u"yellow", u"black", u"black", u"white", u"blue", u"yellow",
        u"blue", u"green", u"white", u"red", u"white", u"red", u"yellow"};
    static const inline std::u16string FOREGROUND[COLORS] = {
        u"black", u"black", u"white", u"yellow", u"blue", u"white", u"blue",
        u"yellow", u"white", u"green", u"white", u"red", u"yellow", u"red"};

    Ui::MainWindow *ui;

    QStringList list;
    Solver* solver;
    Solver::Solution solution;

    void outputResult();
    void printWord(BitMask& mask);

    static std::pair<uint64_t, uint64_t> getRowsAndColumns(QString& text);
    static QString getHtmlTd(const char16_t ch, const std::u16string& background = BACKGROUND[0],
                             const std::u16string& foreground = FOREGROUND[0]);
};

#endif //FINDWORDS_MAINWINDOW_H
