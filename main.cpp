#include "mainwindow.h"
#include <QApplication>


#include <libcorpus2/tagsetmanager.h>
#include <libcorpus2/io/xcesreader.h>

#include <algorithm>

#include <fstream>
#include <QDebug>

#include <QFileDialog>

#include <sstream>

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
    QString fileName = QFileDialog::getOpenFileName(0, "Otworz korpus", "/home/ijn/","*");
    const Corpus2::Tagset tagset = Corpus2::get_named_tagset("nkjp");
    std::ifstream istr(fileName.toUtf8());
#else
    const Corpus2::Tagset tagset = Corpus2::get_named_tagset("kipi");
    std::stringstream istr;
    istr << swiatopoglad;
#endif

    Corpus2::XcesReader xr(tagset, istr);
    Corpus2::Token *token;


    while(token = xr.get_next_token())
        qDebug() << token->orth_utf8().c_str();

    return a.exec();
}
