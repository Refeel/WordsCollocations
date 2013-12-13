#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class WordsStatisticNGrams;
class Methods;
class MethodsNGrams;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void findCollocations();
    void onBigramsChecked();
    void onNgramsChecked();
    void onSectionClicked(int logicalIndex);
    void onUseSignsOfSegmChecked();
    void onUseIgnoredWordsChecked();

private:
    Ui::MainWindow *ui;

    WordsStatisticNGrams *wordStats;
    Methods *methodsBi;
    MethodsNGrams *methodsN;
};

#endif // MAINWINDOW_H
