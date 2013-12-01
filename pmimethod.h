#ifndef PMIMETHOD_H
#define PMIMETHOD_H

#include "wordsstatistics.h"

class PMIMethod
{
public:
    PMIMethod(WordsStatistics *wordsStats);

//private:
    std::vector<std::pair<std::pair<Corpus2::Token*, Corpus2::Token*>, double> > collocationsRank;

    //metody
    void pmiRanking(WordsStatistics *ws);
};

#endif // PMIMETHOD_H
