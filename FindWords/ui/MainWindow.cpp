//
// Created by Dima on 19.01.2020.
//

#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->openDictionaryButton, &QPushButton::clicked, this, &MainWindow::openDictionary);
    connect(ui->mainEditText, &QTextEdit::textChanged, this, &MainWindow::changeRowAndColumnCount);
    connect(ui->calculateButton, &QPushButton::clicked, this, &MainWindow::calculate);
    connect(ui->minLengthSpinBox, &QSpinBox::textChanged, this, &MainWindow::minSpinBoxChange);
    connect(ui->maxLengthSpinBox, &QSpinBox::textChanged, this, &MainWindow::maxSpinBoxChange);
    connect(ui->wordsListWidget, &QListWidget::itemSelectionChanged, this, &MainWindow::showWord);
    connect(ui->combinationsListWidget, &QListWidget::itemSelectionChanged, this, &MainWindow::showCombination);
    connect(ui->combinationWordsListWidget, &QListWidget::itemSelectionChanged, this, &MainWindow::showCombinationWord);
    connect(ui->openWordsButton, &QPushButton::clicked, this, &MainWindow::showWordsScreen);
    connect(ui->openCombinationsButton1, &QPushButton::clicked, this, &MainWindow::showCombinationsScreen);
    connect(ui->openCombinationsButton2, &QPushButton::clicked, this, &MainWindow::showCombinationsScreen);
    connect(ui->endWatchResultButton1, &QPushButton::clicked, this, &MainWindow::showConfigurationScreen);
    connect(ui->endWatchResultButton2, &QPushButton::clicked, this, &MainWindow::showConfigurationScreen);
    connect(ui->endWatchResultButton3, &QPushButton::clicked, this, &MainWindow::showConfigurationScreen);

    showConfigurationScreen();

    solver = new Solver();
}


MainWindow::~MainWindow()
{
    delete solver;
    delete ui;
}


void MainWindow::openDictionary()
{
    QString str = QFileDialog::getOpenFileName(nullptr, "Открыть словарь");
    if (str.size() != 0) {
        uint64_t size = solver->openDictionary(str.toStdString());
        ui->vocabularySizeLabel->setText(QString::number(size));
    }
}


void MainWindow::changeRowAndColumnCount()
{
    QString text = ui->mainEditText->toPlainText();
    std::pair<uint64_t, uint64_t> p = getRowsAndColumns(text);
    ui->rowCountLabel->setText(QString::number(p.first));
    ui->columnCountLabel->setText(QString::number(p.second));
}


void MainWindow::calculate()
{
    ui->centralwidget->setEnabled(false);
    ui->mainEditText->setReadOnly(true);

    QString text = ui->mainEditText->toPlainText();
    std::pair<uint64_t, uint64_t> p = getRowsAndColumns(text);

    char16_t** matrix = new char16_t*[p.first];
    for (uint64_t i = 0; i < p.first; i++) {
        matrix[i] = new char16_t[p.second];
        for (uint64_t j = 0; j < p.second; j++) {
            matrix[i][j] = u' ';
        }
    }

    list = text.split(u'\n');
    for (int i = 0; i < list.size(); i++) {
        std::u16string str = list[i].toStdU16String();
        for (size_t j = 0; j < str.size(); j++) {
            matrix[i][j] = str[j];
        }
    }

    solver->setMinAndMaxWordLength(ui->minLengthSpinBox->value(), ui->maxLengthSpinBox->value());

    bool needFindCombinations = ui->combinationCheckBox->isChecked();
    solution = solver->solve(matrix, p.first, p.second, needFindCombinations);

    outputResult();
}

void MainWindow::minSpinBoxChange()
{
    int min = ui->minLengthSpinBox->value();
    int max = ui->maxLengthSpinBox->value();
    if (min > max) {
        ui->maxLengthSpinBox->setValue(min);
    }
}

void MainWindow::maxSpinBoxChange()
{
    int min = ui->minLengthSpinBox->value();
    int max = ui->maxLengthSpinBox->value();
    if (max < min) {
        ui->minLengthSpinBox->setValue(max);
    }
}

void MainWindow::showWord()
{
    int index = ui->wordsListWidget->currentRow();
    printWord(solution.matchedWordsWithMasks[index].mask);
}

void MainWindow::showCombination()
{
    int index = ui->combinationsListWidget->currentRow();

    ui->combinationWordsListWidget->clear();
    for (size_t i = 0; i < solution.combinedWordsIndexes[index].size(); i++) {
        std::u16string str = solution.matchedWordsWithMasks[solution.combinedWordsIndexes[index][i]].str;
        ui->combinationWordsListWidget->addItem(QString::fromStdU16String(str));
    }

    QString html = "<table cellspacing=\"0\" cellpadding=\"2\">";
    for (int i = 0; i < list.size(); i++) {
        html.append("<tr>");
        std::u16string str = list[i].toStdU16String();
        for (size_t j = 0; j < str.size(); j++) {
            if (str[j] != u' ') {
                for (size_t k = 0; k < solution.combinedWordsIndexes[index].size(); k++) {
                    if (solution.matchedWordsWithMasks[solution.combinedWordsIndexes[index][k]].mask[i][j]) {
                        html.append(getHtmlTd(str[j], BACKGROUND[k % COLORS], FOREGROUND[k % COLORS]));
                        break;
                    }
                }
            } else {
                html.append(getHtmlTd(str[j]));
            }
        }
        html.append("</tr>");
    }
    html.append("</table>");
    ui->mainEditText->setHtml(html);

    showCombinationWordsScreen();
}

void MainWindow::showCombinationWord()
{
    int index1 = ui->combinationsListWidget->currentRow();
    int index2 = ui->combinationWordsListWidget->currentRow();
    printWord(solution.matchedWordsWithMasks[solution.combinedWordsIndexes[index1][index2]].mask);
}

void MainWindow::showConfigurationScreen()
{
    ui->mainEditText->setText(list.join('\n'));
    ui->mainEditText->setReadOnly(false);
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::showWordsScreen()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::showCombinationsScreen()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::showCombinationWordsScreen()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::outputResult()
{
    QString time("Время нахождения слов заняло (секунд): ");
    time.append(QString::number(static_cast<double>(solution.calculationTimeInNanoseconds) / 1e9, 'g', 3));
    ui->statusBar->showMessage(time);

    ui->wordsListWidget->clear();
    ui->combinationsListWidget->clear();
    for (auto & temp : solution.matchedWordsWithMasks) {
        ui->wordsListWidget->addItem(QString::fromStdU16String(temp.str));
    }
    for (uint64_t i = 0; i < solution.combinedWordsIndexes.size(); i++) {
        ui->combinationsListWidget->addItem("Комбинация #" + QString::number(i + 1));
    }

    showWordsScreen();
    ui->centralwidget->setEnabled(true);
}

void MainWindow::printWord(BitMask& mask)
{
    QString html = "<table cellspacing=\"0\" cellpadding=\"2\">";

    for (int i = 0; i < list.size(); i++) {
        html.append("<tr>");
        std::u16string str = list[i].toStdU16String();
        for (size_t j = 0; j < str.size(); j++) {
            if (mask[i][j]) {
                html.append(getHtmlTd(str[j], BACKGROUND[1]));
            } else {
                html.append(getHtmlTd(str[j]));
            }
        }
        html.append("</tr>");
    }
    html.append("</table>");
    ui->mainEditText->setHtml(html);
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

QString MainWindow::getHtmlTd(const char16_t ch, const std::u16string& background, const std::u16string& foreground)
{
    return QString::fromStdU16String(u"<td bgcolor=\"" + background + u"\" style=\"color: \'"
                                     + foreground + u"\'\">" + ch + u"</td>");
}
