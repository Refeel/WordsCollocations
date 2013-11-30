#include "mainwindow.h"
#include <QApplication>

#include <libcorpus2/tagsetmanager.h>
#include <libcorpus2/io/xcesreader.h>

#include <algorithm>

#include <fstream>
#include <QDebug>

#include <QFileDialog>

#include <sstream>

#include <vector>
#include <QString>

#include "wordsstatistics.h"
#include "fscpmethod.h"
#include "zscoremethod.h"


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
        "<orth>,</orth>\n"
        "<lex disamb=\"1\"><base>,</base><ctag>interp</ctag></lex>\n"
        "</tok>\n"

        "<tok>\n"
        "<orth>tak</orth>\n"
        "<lex disamb=\"1\"><base>tak</base><ctag>subst:sg:nom:m3</ctag></lex>\n"
        "</tok>\n"

        "<tok>\n"
        "<orth>nie</orth>\n"
        "<lex disamb=\"1\"><base>nie</base><ctag>subst:sg:nom:m3</ctag></lex>\n"
        "</tok>\n"

        "<tok>\n"
        "<orth>może</orth>\n"
        "<lex disamb=\"1\"><base>móc</base><ctag>subst:sg:nom:m3</ctag></lex>\n"
        "</tok>\n"

        "<tok>\n"
        "<orth>nie</orth>\n"
        "<lex disamb=\"1\"><base>nie</base><ctag>subst:sg:nom:m3</ctag></lex>\n"
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

#ifdef FROM_FILE
    QString fileName = QFileDialog::getOpenFileName(0, "Otworz korpus", "/home/ijn/","*.ccl");
    const Corpus2::Tagset tagset = Corpus2::get_named_tagset("nkjp");
    std::ifstream istr(fileName.toUtf8());
#else
    const Corpus2::Tagset tagset = Corpus2::get_named_tagset("kipi");
    std::stringstream istr;
    istr << swiatopoglad;
#endif

    Corpus2::XcesReader xr(tagset, istr);

    /*
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
    WordsStatistics *wordStats = new WordsStatistics(xr);
    FSCPMethod *fscpm = new FSCPMethod(wordStats);
    for(int i=0; i<fscpm->collocationsRank.size(); i++)
    {
        qDebug() << i;
        QString row = "";
        row.append(fscpm->collocationsRank[i].first.first->get_preferred_lexeme(tagset).lemma_utf8().c_str()).append(" ").append(fscpm->collocationsRank[i].first.second->get_preferred_lexeme(tagset).lemma_utf8().c_str()).append(" ").append(QString::number(fscpm->collocationsRank[i].second));

        qDebug() << row;
    }
    */

    /*
    WordsStatistics *wordStats = new WordsStatistics(xr);
    ZScoreMethod *zsm = new ZScoreMethod(wordStats);
    for(int i=0; i<zsm->collocationsRank.size(); i++)
    {
        qDebug() << i;
        QString row = "";
        row.append(zsm->collocationsRank[i].first.first->orth_utf8().c_str()).append(" ").append(zsm->collocationsRank[i].first.second->orth_utf8().c_str()).append(" ").append(QString::number(zsm->collocationsRank[i].second));

        qDebug() << row;
    }
    */

    //qDebug() << QString::number(wordStats->getWordsCount());


    return a.exec();
}
