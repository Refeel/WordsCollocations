#include "mainwindow.h"
#include <QApplication>

#include <libcorpus2/tagsetmanager.h>
#include <libcorpus2/io/xcesreader.h>

#include <algorithm>
#include <boost/bind.hpp>

#include <fstream>
#include <QDebug>

#include <QFileDialog>
#include <QFile>

#include <sstream>

#include <vector>
#include <QString>

#include "wordsstatistics.h"
#include "methods.h"
#include "methodsngrams.h"

#include "wordsstatisticngrams.h"

static char swiatopoglad[] =
"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
"<!DOCTYPE cesAna SYSTEM \"xcesAnaIPI.dtd\">\n"
"<cesAna xmlns:xlink=\"http://www.w3.org/1999/xlink\" version=\"1.0\" type=\"lex disamb\">\n"
"<chunkList>\n"
"<chunk id=\"ch51\" type=\"tok\">\n"
"<chunk type=\"s\">\n"
"<tok>\n"
"<orth>Uważam</orth>\n"
"<lex disamb=\"1\"><base>uważać</base><ctag>fin:sg:pri:imperf</ctag></lex>\n"
"</tok>\n"
"<ns/>\n"
"<tok>\n"
"<orth>,</orth>\n"
"<lex disamb=\"1\"><base>,</base><ctag>interp</ctag></lex>\n"
"</tok>\n"
"<tok>\n"
"<orth>że</orth>\n"
"<lex disamb=\"1\"><base>że</base><ctag>conj</ctag></lex>\n"
"</tok>\n"
"<tok>\n"
"<orth>światopogląd</orth>\n"
"<lex><base>światopogląd</base><ctag>subst:sg:acc:m3</ctag></lex>\n"
"<lex disamb=\"1\"><base>światopogląd</base><ctag>subst:sg:nom:m3</ctag></lex>\n"
"</tok>\n"

        "<tok>\n"
        "<orth>nie</orth>\n"
        "<lex disamb=\"1\"><base>nie</base><ctag>conj</ctag></lex>\n"
        "</tok>\n"

        "<tok>\n"
        "<orth>i</orth>\n"
        "<lex disamb=\"1\"><base>i</base><ctag>interp</ctag></lex>\n"
        "</tok>\n"

        "<tok>\n"
        "<orth>i</orth>\n"
        "<lex disamb=\"1\"><base>i</base><ctag>subst:sg:nom:m3</ctag></lex>\n"
        "</tok>\n"

        "<tok>\n"
        "<orth>i</orth>\n"
        "<lex disamb=\"1\"><base>i</base><ctag>subst:sg:nom:m3</ctag></lex>\n"
        "</tok>\n"

        "<tok>\n"
        "<orth>może</orth>\n"
        "<lex disamb=\"1\"><base>móc</base><ctag>subst:sg:nom:m3</ctag></lex>\n"
        "</tok>\n"

        "<tok>\n"
        "<orth>i</orth>\n"
        "<lex disamb=\"1\"><base>i</base><ctag>subst:sg:nom:m3</ctag></lex>\n"
        "</tok>\n"

        "<tok>\n"
        "<orth>móc</orth>\n"
        "<lex disamb=\"1\"><base>móc</base><ctag>subst:sg:nom:m3</ctag></lex>\n"
        "</tok>\n"

        "<tok>\n"
        "<orth>nie</orth>\n"
        "<lex disamb=\"1\"><base>nie</base><ctag>subst:sg:nom:m3</ctag></lex>\n"
        "</tok>\n"

        "<tok>\n"
        "<orth>mógł</orth>\n"
        "<lex disamb=\"1\"><base>móc</base><ctag>subst:sg:nom:m3</ctag></lex>\n"
        "</tok>\n"

        "<tok>\n"
        "<orth>Uważam</orth>\n"
        "<lex disamb=\"1\"><base>uważać</base><ctag>subst:sg:nom:m3</ctag></lex>\n"
        "</tok>\n"

        "<tok>\n"
        "<orth>,</orth>\n"
        "<lex disamb=\"1\"><base>,</base><ctag>subst:sg:nom:m3</ctag></lex>\n"
        "</tok>\n"

        "<tok>\n"
        "<orth>że</orth>\n"
        "<lex disamb=\"1\"><base>że</base><ctag>subst:sg:nom:m3</ctag></lex>\n"
        "</tok>\n"

        "<tok>\n"
        "<orth>nie</orth>\n"
        "<lex disamb=\"1\"><base>nie</base><ctag>subst:sg:nom:m3</ctag></lex>\n"
        "</tok>\n"

        "<tok>\n"
        "<orth>zrobię</orth>\n"
        "<lex disamb=\"1\"><base>zrobić</base><ctag>subst:sg:nom:m3</ctag></lex>\n"
        "</tok>\n"

        "<tok>\n"
        "<orth>Uważam</orth>\n"
        "<lex disamb=\"1\"><base>uważać</base><ctag>subst:sg:nom:m3</ctag></lex>\n"
        "</tok>\n"

        "<tok>\n"
        "<orth>też</orth>\n"
        "<lex disamb=\"1\"><base>też</base><ctag>subst:sg:nom:m3</ctag></lex>\n"
        "</tok>\n"

        "<tok>\n"
        "<orth>,</orth>\n"
        "<lex disamb=\"1\"><base>,</base><ctag>subst:sg:nom:m3</ctag></lex>\n"
        "</tok>\n"
"</chunk>\n"
"</chunk>\n"
"</chunkList>\n"
"</cesAna>\n"
;

#define FROM_FILE

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

//#ifdef FROM_FILE
//    QString fileName = QFileDialog::getOpenFileName(0, "Otworz korpus", "/home/ijn/","*.ccl");
//    const Corpus2::Tagset tagset = Corpus2::get_named_tagset("nkjp");
//    std::ifstream istr(fileName.toUtf8());
//#else
//    const Corpus2::Tagset tagset = Corpus2::get_named_tagset("kipi");
//    std::stringstream istr;
//    istr << swiatopoglad;
//#endif

//    Corpus2::XcesReader xr(tagset, istr);

    /*
    //wyswietlanie statystyki slow
    WordsStatistics *wordStats = new WordsStatistics(xr);

    for(int i=0; i<wordStats->allWords.size(); i++)
    {
        qDebug() << i;
        QString row = "";
        for(int j=0; j<wordStats->allWords[i].size(); j++)
            row.append(wordStats->allWords[i][j]->get_preferred_lexeme(tagset).lemma_utf8().c_str()).append(" ").append(QString::number(wordStats->numberAllWords[i][j])).append(" ");

        qDebug() << row;
    }
    */

    /*
    //wyswietlanie wszyskich rankingow obok kolokacji (uwaga: rankingi nie moga byc posortowane)
    WordsStatistics *wordStats = new WordsStatistics(xr);
    Methods *m = new Methods(wordStats);
    for(int i=0; i<m->collocationsRankFSCP.size(); i++)
    {
        qDebug() << i;
        QString row = "";
        row.append(m->collocationsRankFSCP[i].first.first->get_preferred_lexeme(tagset).lemma_utf8().c_str()).append(" ").append(m->collocationsRankFSCP[i].first.second->get_preferred_lexeme(tagset).lemma_utf8().c_str()).append(" FSCP: ").append(QString::number(m->collocationsRankFSCP[i].second)).append(" Z-Score: ").append(QString::number(m->collocationsRankZScore[i].second)).append(" PMI: ").append(QString::number(m->collocationsRankPMI[i].second));
        qDebug() << row;
    }
    */

    /*
    //wysietlanie posortowanych rankingow
    WordsStatistics *wordStats = new WordsStatistics(xr);
    Methods *m = new Methods(wordStats);

    QString fileNameOut = "kolokacje.out";
    QFile mFile(fileNameOut);

    if(!mFile.open(QFile::WriteOnly | QFile::Text)){
        qDebug() << "Could not open file for writing";
    }
    QTextStream out(&mFile);

    for(int i=0; i<m->collocationsRankFSCP.size(); i++)
    {
        qDebug() << i;
        QString row = "";
        row.append(m->collocationsRankFSCP[i].first.first->get_preferred_lexeme(tagset).lemma_utf8().c_str()).append(" ").append(m->collocationsRankFSCP[i].first.second->get_preferred_lexeme(tagset).lemma_utf8().c_str()).append(" FSCP: ").append(QString::number(m->collocationsRankFSCP[i].second));
        row.append(" | ").append(m->collocationsRankZScore[i].first.first->get_preferred_lexeme(tagset).lemma_utf8().c_str()).append(" ").append(m->collocationsRankZScore[i].first.second->get_preferred_lexeme(tagset).lemma_utf8().c_str()).append(" Z-Score: ").append(QString::number(m->collocationsRankZScore[i].second));
        row.append(" | ").append(m->collocationsRankPMI[i].first.first->get_preferred_lexeme(tagset).lemma_utf8().c_str()).append(" ").append(m->collocationsRankPMI[i].first.second->get_preferred_lexeme(tagset).lemma_utf8().c_str()).append(" PMI: ").append(QString::number(m->collocationsRankPMI[i].second));
        qDebug() << row;
        out << row;
        out << "\n";
    }
    */

//    WordsStatisticNGrams *wordStats = new WordsStatisticNGrams(xr);
//    MethodsNGrams *methods = new MethodsNGrams(wordStats);

//    for(int i=0;i<methods->collocationsRankSCP.size(); i++)
//    {
//        qDebug() << methods->collocationsRankSCP[i].first << " " << methods->collocationsRankSCP[i].second;
//    }

//    for(int i=0; i< wordStats->wordsStatistic.size(); i++)
//    {
//        QHash<QString, int>::const_iterator iter = wordStats->wordsStatistic[i].constBegin();
//        while( iter != wordStats->wordsStatistic[i].constEnd())
//        {
//            qDebug() << iter.key() << " " << iter.value();
//            iter++;
//        }
//    }


    return a.exec();
}
