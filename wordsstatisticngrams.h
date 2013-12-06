#ifndef WORDSSTATISTICNGRAMS_H
#define WORDSSTATISTICNGRAMS_H

#include <libcorpus2/io/xcesreader.h>
#include <tr1/unordered_set>

#include <QDebug>
#include <QString>
#include <QHash>

class WordsStatisticNGrams
{
public:
    WordsStatisticNGrams(Corpus2::XcesReader &xr, unsigned n);

    void setN(unsigned n);
    int getN();

    int getWordsCount();

    std::vector<QHash<QString, int> > wordsStatistic;

private:
    Corpus2::XcesReader *xreader;
    Corpus2::Tagset tagset;

    unsigned int n;

    int wordsCount;

    void makeStatistics(Corpus2::XcesReader &xr);

};

#endif // WORDSSTATISTICNGRAMS_H
