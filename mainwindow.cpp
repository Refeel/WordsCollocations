#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <fstream>
#include <QDebug>

#include <QFileDialog>
#include <QFile>

#include <QtSql/QSqlTableModel>
#include <QtSql/QSqlDatabase>
#include <sstream>

#include <vector>
#include <QString>
#include <QHeaderView>

#include "wordsstatistics.h"
#include "methods.h"
#include "methodsngrams.h"

#include <libcorpus2/tagsetmanager.h>
#include <libcorpus2/io/xcesreader.h>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
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

void MainWindow::findCollocations()
{
    QString fileName = QFileDialog::getOpenFileName(0, "Otworz korpus", "/home/ijn/","*.ccl");
    const Corpus2::Tagset tagset = Corpus2::get_named_tagset("nkjp");
    std::ifstream istr(fileName.toUtf8());

    Corpus2::XcesReader xr(tagset, istr);


    if(ui->bigramsRadioButton->isChecked())
    {
        bool fscpStatus = ui->FSCPCheckBox->isChecked();
        bool zscoreStatus = ui->zscoreCheckBox->isChecked();
        bool pmiStatus = ui->PMICheckBox->isChecked();
        bool numbersFilterStatus = ui->numFilterCheckBox->isChecked();
        bool properNamesFilterStatus = ui->propNamesCheckBox->isChecked();

        WordsStatistics *wordStats = new WordsStatistics(xr);
        Methods *m = new Methods(wordStats);



        ui->collTableWidget->setRowCount(m->collocationsRankFSCP.size());
        ui->collTableWidget->setColumnCount(1 + (int)fscpStatus + (int)zscoreStatus + (int)pmiStatus);

        ui->collTableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("Collocations"));

        int colsNum = 1;
        int fscpColId, zscoreColId, pmiColId;

        if(fscpStatus)
            ui->collTableWidget->setHorizontalHeaderItem(fscpColId = colsNum++, new QTableWidgetItem("FSCP"));

        if(zscoreStatus)
            ui->collTableWidget->setHorizontalHeaderItem(zscoreColId = colsNum++, new QTableWidgetItem("z-score"));

        if(pmiStatus)
            ui->collTableWidget->setHorizontalHeaderItem(pmiColId = colsNum++, new QTableWidgetItem("PMI"));




        for(int i=0; i<m->collocationsRankFSCP.size(); i++)
        {
            qDebug() << i;
            QString row = "";
            row.append(m->collocationsRankFSCP[i].first.first->get_preferred_lexeme(tagset).lemma_utf8().c_str()).append(" ").append(m->collocationsRankFSCP[i].first.second->get_preferred_lexeme(tagset).lemma_utf8().c_str()).append(" FSCP: ").append(QString::number(m->collocationsRankFSCP[i].second)).append(" Z-Score: ").append(QString::number(m->collocationsRankZScore[i].second)).append(" PMI: ").append(QString::number(m->collocationsRankPMI[i].second));
            qDebug() << row;

            ui->collTableWidget->setItem(i,0, new QTableWidgetItem(QString(m->collocationsRankFSCP[i].first.first->get_preferred_lexeme(tagset).lemma_utf8().c_str()).append(" ").append(m->collocationsRankFSCP[i].first.second->get_preferred_lexeme(tagset).lemma_utf8().c_str())));
            if(fscpStatus)   ui->collTableWidget->setItem(i,fscpColId, new QTableWidgetItem(QString::number(m->collocationsRankFSCP[i].second)));
            if(zscoreStatus) ui->collTableWidget->setItem(i,zscoreColId, new QTableWidgetItem(QString::number(m->collocationsRankZScore[i].second)));
            if(pmiStatus)    ui->collTableWidget->setItem(i,pmiColId, new QTableWidgetItem(QString::number(m->collocationsRankPMI[i].second)));
        }


    }
    else if(ui->ngramsRadioButton->isChecked())
    {
        unsigned n = ui->nSpinBox->value();
        bool miStatus = ui->MICheckBox->isChecked();
        bool scpStatus = ui->SCPCheckBox->isChecked();


        WordsStatisticNGrams *wordStats = new WordsStatisticNGrams(xr, ui->nSpinBox->value());

        MethodsNGrams *m = new MethodsNGrams(wordStats);


        ui->collTableWidget->setRowCount(m->collocationsRankSCP.size());
        ui->collTableWidget->setColumnCount(1 + (int)miStatus + (int)scpStatus);

        ui->collTableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("Collocations"));

        int colsNum = 1;

        int miColId, scpColId;

        if(miStatus)
            ui->collTableWidget->setHorizontalHeaderItem(miColId = colsNum++, new QTableWidgetItem("MI"));

        if(scpStatus)
            ui->collTableWidget->setHorizontalHeaderItem(scpColId = colsNum++, new QTableWidgetItem("SCP"));


        for(int i=0;i<m->collocationsRankSCP.size(); i++)
        {
            qDebug() << m->collocationsRankSCP[i].first << " " << m->collocationsRankSCP[i].second;

            ui->collTableWidget->setItem(i, 0, new QTableWidgetItem(m->collocationsRankSCP[i].first));

            if(miStatus)   ui->collTableWidget->setItem(i,miColId, new QTableWidgetItem(QString::number(m->collocationsRankSI[i].second)));
            if(scpStatus)  ui->collTableWidget->setItem(i,scpColId, new QTableWidgetItem(QString::number(m->collocationsRankSCP[i].second)));
        }
    }

    QHeaderView *header = ui->collTableWidget->horizontalHeader();
    connect(header, SIGNAL(sectionClicked(int) ), this, SLOT(onSectionClicked(int)) );
}
