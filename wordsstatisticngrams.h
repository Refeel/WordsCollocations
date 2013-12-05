#ifndef WORDSSTATISTICNGRAMS_H
#define WORDSSTATISTICNGRAMS_H

#include <libcorpus2/io/xcesreader.h>

#include "stdlib.h"
#include "time.h"

#include <QDebug>
#include <QString>

class WordsStatisticNGrams
{
public:
    WordsStatisticNGrams(Corpus2::XcesReader &xr);

    void setN(unsigned n);
    void setSpan(unsigned span);

    int getNumberOfInstancesOfWord(Corpus2::Token *token);
    int getNumberOfInstancesOfNGram(Corpus2::Token *pivot, std::vector<std::pair<Corpus2::Token*, int> > nGram);
    std::vector<std::pair<Corpus2::Token*, std::vector<std::vector<std::pair<Corpus2::Token *, int> > > > > *getAllNGramsForAllPivots();
    int getWordsCount();

private:
    Corpus2::XcesReader *xreader;
    Corpus2::Tagset tagset;

    unsigned int n;
    unsigned int span;

    int wordsCount;

    std::vector<std::pair<Corpus2::Token *, int> > allWordsWithCount;
    std::vector<std::vector<std::pair<std::vector<std::pair<Corpus2::Token*, int> >, int> > > nGramsWithCount;

    bool isNewToken(Corpus2::Token *token);
    int getPositionOfWord(Corpus2::Token *token);

    int factorial(int n);
    int countingSequencesWithoutRepetition(int n, int k);
    std::vector<std::vector<int> > generateSequencesWithoutRepetition(int span, int n);

    void makeStatistics(Corpus2::XcesReader &xr);
};

#endif // WORDSSTATISTICNGRAMS_H
