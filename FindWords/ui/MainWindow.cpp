//
// Created by Dima on 19.01.2020.
//

#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent),
                                         ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->openDictionaryButton, &QPushButton::clicked, this, &MainWindow::openDictionary);
    connect(ui->inputPlainTextEdit, &QPlainTextEdit::textChanged, this, &MainWindow::changeRowAndColumnCount);
    connect(ui->calculateButton, &QPushButton::clicked, this, &MainWindow::calculate);

    solver = new Solver();
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::openDictionary()
{
    QString str = QFileDialog::getOpenFileName(nullptr, "Открыть словарь");
    uint64_t size = solver->openDictionary(str.toStdString());
    ui->vocabularySizeLabel->setText(QString::number(size));
}


void MainWindow::changeRowAndColumnCount()
{
    QString text = ui->inputPlainTextEdit->toPlainText();
    std::pair<uint64_t, uint64_t> p = getRowsAndColumns(text);
    ui->rowCountLabel->setText(QString::number(p.first));
    ui->columnCountLabel->setText(QString::number(p.second));
}


void MainWindow::calculate()
{
    QString text = ui->inputPlainTextEdit->toPlainText();
    std::pair<uint64_t, uint64_t> p = getRowsAndColumns(text);

    char16_t** matrix = new char16_t*[p.first];
    for (uint64_t i = 0; i < p.first; i++) {
        matrix[i] = new char16_t[p.second];
        for (uint64_t j = 0; j < p.second; j++) {
            matrix[i][j] = u' ';
        }
    }

    auto list = text.split(u'\n');
    for (int i = 0; i < list.size(); i++) {
        std::u16string str = list[i].toStdU16String();
        for (size_t j = 0; j < str.size(); j++) {
            matrix[i][j] = str[j];
        }
    }

    Solver::Solution solution = solver->solve(matrix, p.first, p.second);

    std::u16string result = u"";
    for (auto & temp : solution.matchedWordsWithMasks) {
        result += temp.str + u'\n';
    }
    ui->outputPlainTextEdit->setPlainText(QString::fromStdU16String(result));
}


std::pair<uint64_t, uint64_t> MainWindow::getRowsAndColumns(QString& text)
{
    auto list = text.split(u'\n');
    int64_t rows = list.size();
    int64_t columns = 0;
    for (auto & line : list) {
        if (columns < line.size()) {
            columns = line.size();
        }
    }
    return std::pair<uint64_t, uint64_t>(rows, columns);
}
