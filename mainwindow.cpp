#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <fstream>
#include <QDebug>

#include <QFileDialog>
#include <QFile>

#include <sstream>

#include <vector>
#include <QString>
#include <QHeaderView>

#include "methods.h"
#include "methodsngrams.h"

#include <libcorpus2/tagsetmanager.h>
#include <libcorpus2/io/xcesreader.h>
#include <libcorpus2/ann/annotatedsentence.h>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    wordStats = NULL;
    methodsBi = NULL;
    methodsN  = NULL;
}

MainWindow::~MainWindow()
{
    delete wordStats;
    delete methodsBi;
    delete methodsN;

    delete ui;
}

void MainWindow::onBigramsChecked()
{
    ui->ngramsGroupBox->setEnabled(false);
    ui->bigramsGroupBox->setEnabled(true);
}

void MainWindow::onNgramsChecked()
{
    ui->bigramsGroupBox->setEnabled(false);
    ui->ngramsGroupBox->setEnabled(true);
}

void MainWindow::onSectionClicked(int logicalIndex)
{
    ui->collTableWidget->sortByColumn(logicalIndex);
}

void MainWindow::onUseSignsOfSegmChecked()
{
    ui->signsOfSegmLineEdit->setEnabled(ui->signsOfSegmCheckBox->isChecked());
}

void MainWindow::onUseIgnoredWordsChecked()
{
    ui->ignoredWordsLineEdit->setEnabled(ui->ignoredWordsCheckBox->isChecked());
}

void MainWindow::findCollocations()
{
    QString fileName = QFileDialog::getOpenFileName(0, "Otworz korpus", "/home/ijn/","*.ccl");
    if(fileName.size() == 0)
        return;

    const Corpus2::Tagset tagset = Corpus2::get_named_tagset("nkjp");
    std::ifstream istr(fileName.toUtf8());

    Corpus2::XcesReader *xr = new Corpus2::XcesReader(tagset, istr);

    delete wordStats; wordStats = NULL;
    delete methodsBi; methodsBi = NULL;
    delete methodsN;  methodsN  = NULL;

    ui->collTableWidget->clear();

    bool numbersFilterStatus = ui->numFilterCheckBox->isChecked();
    bool properNamesFilterStatus = ui->propNamesCheckBox->isChecked();
    bool useSignsOfSegm = ui->signsOfSegmCheckBox->isChecked();
    bool useIgnoredWords = ui->ignoredWordsCheckBox->isChecked();

    QStringList signsOfSegm; signsOfSegm.clear();
    QStringList ignoredWords; ignoredWords.clear();

    if(useSignsOfSegm)
    {
        QString str = ui->signsOfSegmLineEdit->text();
        signsOfSegm = str.split(" ");
    }

    if(useIgnoredWords)
    {
        QString str = ui->ignoredWordsLineEdit->text();
        ignoredWords = str.split(" ");
    }

    if(ui->bigramsRadioButton->isChecked())
    {
        bool fscpStatus = ui->FSCPCheckBox->isChecked();
        bool zscoreStatus = ui->zscoreCheckBox->isChecked();
        bool pmiStatus = ui->PMICheckBox->isChecked();

        wordStats = new WordsStatisticNGrams(*xr, 2);

        for(int i=0; i<signsOfSegm.size(); i++)
            wordStats->addSegmentationSign(signsOfSegm[i]);

        for(int i=0; i<ignoredWords.size(); i++)
            wordStats->addIgnoredWord(ignoredWords[i].toStdString());

        wordStats->setFilterProperNamesEnabled(properNamesFilterStatus);
        wordStats->setFilterNumbersEnabled(numbersFilterStatus);

        wordStats->makeStatistics();

        methodsBi = new Methods(wordStats);


        ui->collTableWidget->setRowCount(methodsBi->collocationsRankFSCP.size());
        ui->collTableWidget->setColumnCount(1 + (int)fscpStatus + (int)zscoreStatus + (int)pmiStatus);

        ui->collTableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("Collocations"));

        int colsNum = 1;
        int fscpColId=0, zscoreColId=0, pmiColId=0;

        if(fscpStatus)
            ui->collTableWidget->setHorizontalHeaderItem(fscpColId = colsNum++, new QTableWidgetItem("FSCP"));

        if(zscoreStatus)
            ui->collTableWidget->setHorizontalHeaderItem(zscoreColId = colsNum++, new QTableWidgetItem("z-score"));

        if(pmiStatus)
            ui->collTableWidget->setHorizontalHeaderItem(pmiColId = colsNum++, new QTableWidgetItem("PMI"));



        QTableWidgetItem *item;

        for(unsigned i=0; i<methodsBi->collocationsRankFSCP.size(); i++)
        {
            ui->collTableWidget->setItem(i,0, new QTableWidgetItem(QString(methodsBi->collocationsRankFSCP[i].first)));

            if(fscpStatus)
            {
                item = new QTableWidgetItem();
                item->setData(Qt::DisplayRole, methodsBi->collocationsRankFSCP[i].second);
                ui->collTableWidget->setItem(i,fscpColId, item);
            }

            if(zscoreStatus)
            {
                item = new QTableWidgetItem();
                item->setData(Qt::DisplayRole, methodsBi->collocationsRankZScore[i].second);
                ui->collTableWidget->setItem(i,zscoreColId, item);
            }

            if(pmiStatus)
            {
                item = new QTableWidgetItem();
                item->setData(Qt::DisplayRole, methodsBi->collocationsRankPMI[i].second);
                ui->collTableWidget->setItem(i,pmiColId, item);
            }
        }
    }
    else if(ui->ngramsRadioButton->isChecked())
    {
        unsigned n = ui->nSpinBox->value();
        bool miStatus = ui->MICheckBox->isChecked();
        bool scpStatus = ui->SCPCheckBox->isChecked();

        wordStats = new WordsStatisticNGrams(*xr, n);

        for(int i=0; i<signsOfSegm.size(); i++)
            wordStats->addSegmentationSign(signsOfSegm[i]);

        for(int i=0; i<ignoredWords.size(); i++)
            wordStats->addIgnoredWord(ignoredWords[i].toStdString());

        wordStats->setFilterProperNamesEnabled(properNamesFilterStatus);
        wordStats->setFilterNumbersEnabled(numbersFilterStatus);

        wordStats->makeStatistics();

        methodsN = new MethodsNGrams(wordStats);

        ui->collTableWidget->setRowCount(methodsN->collocationsRankSCP.size());
        ui->collTableWidget->setColumnCount(1 + (int)miStatus + (int)scpStatus);

        ui->collTableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("Collocations"));

        int colsNum = 1;

        int miColId=0, scpColId=0;

        if(miStatus)
            ui->collTableWidget->setHorizontalHeaderItem(miColId = colsNum++, new QTableWidgetItem("MI"));

        if(scpStatus)
            ui->collTableWidget->setHorizontalHeaderItem(scpColId = colsNum++, new QTableWidgetItem("SCP"));


        QTableWidgetItem *item;

        for(size_t i=0;i<methodsN->collocationsRankSCP.size(); i++)
        {
            ui->collTableWidget->setItem(i, 0, new QTableWidgetItem(methodsN->collocationsRankSCP[i].first));

            if(miStatus)
            {
                item = new QTableWidgetItem();
                item->setData(Qt::DisplayRole, methodsN->collocationsRankSI[i].second);
                ui->collTableWidget->setItem(i,miColId, item);
            }

            if(scpStatus)
            {
                item = new QTableWidgetItem();
                item->setData(Qt::DisplayRole, methodsN->collocationsRankSCP[i].second);
                ui->collTableWidget->setItem(i,scpColId, item);
            }
        }
    }

    QHeaderView *header = ui->collTableWidget->horizontalHeader();
    connect(header, SIGNAL(sectionClicked(int) ), this, SLOT(onSectionClicked(int)) );

    delete xr;
}
