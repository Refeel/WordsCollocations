#ifndef WORDSSTATISTICNGRAMS_H
#define WORDSSTATISTICNGRAMS_H

#include <libcorpus2/io/xcesreader.h>

#include <QDebug>
#include <QString>
#include <QHash>

class WordsStatisticNGrams
{
public:
    WordsStatisticNGrams(Corpus2::XcesReader &xr, unsigned n);
    ~WordsStatisticNGrams();

    void setN(unsigned n);
    int getN();

    int getWordsCount();

    void makeStatistics();

    void addSegmentationSign(QString sign);
    void addIgnoredWord(std::string word);
    void setFilterNumbersEnabled(bool filter);
    void setFilterProperNamesEnabled(bool filter);

    std::vector<QHash<QString, int> > wordsStatistic;

private:
    Corpus2::XcesReader *xreader;
    Corpus2::Tagset tagset;

    unsigned int n;

    int wordsCount;
    bool filterNumbers;
    bool filterProperNames;

    std::vector<QString> segmentedSigns;
    std::vector<std::string> ignoredWords;

    void makeStatistics(Corpus2::XcesReader &xr);

    bool isIgnored(Corpus2::Token *token);
    std::vector<int> isSegmentedSign(std::vector<QString> tokens);

    bool numberFilter(std::vector<QString> tokens);
    bool properNameFilter(std::vector<QString> tokens);
};

#endif // WORDSSTATISTICNGRAMS_H
