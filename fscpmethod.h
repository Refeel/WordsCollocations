#ifndef FSCPMETHOD_H
#define FSCPMETHOD_H

#include "wordsstatistics.h"

class FSCPMethod
{
public:
    FSCPMethod(WordsStatistics *wordsStats);

//private:
    std::vector<std::pair<std::pair<Corpus2::Token*, Corpus2::Token*>, double> > collocationsRank;

    //metody
    void fscpRanking(WordsStatistics *ws);
};

#endif // FSCPMETHOD_H
