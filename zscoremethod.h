#ifndef ZSCOREMETHOD_H
#define ZSCOREMETHOD_H

#include "wordsstatistics.h"

class ZScoreMethod
{
public:
    ZScoreMethod(WordsStatistics *wordsStats);

private:
    std::vector<std::pair<std::pair<Corpus2::Token*, Corpus2::Token*>, double> > collocationsRank;

    //metody
    void zScoreRanking(WordsStatistics *ws);
};

#endif // ZSCOREMETHOD_H
