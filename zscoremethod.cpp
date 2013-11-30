#include "zscoremethod.h"

#include <math.h>

#include <QDebug>

ZScoreMethod::ZScoreMethod(WordsStatistics *wordsStats)
{
    zScoreRanking(wordsStats);
}

void ZScoreMethod::zScoreRanking(WordsStatistics *ws)
{
    std::vector<std::vector<Corpus2::Token*> > allWordsWithPairs = ws->getAllWordsWithPairs();

    //liczba słów w korpusie
    int n = ws->getWordsCount();

    for(size_t i=0; i<allWordsWithPairs.size(); i++)
    {
        if(allWordsWithPairs[i].size() > 1)
        {
            //liczba wystapien slowa w1 w korpusie
            int word1Count = ws->getNumberOfInstancesOfWord(allWordsWithPairs[i][0]);
            for(size_t j=1; j<allWordsWithPairs[i].size(); j++)
            {
                std::pair<Corpus2::Token*, Corpus2::Token*> collocation;
                collocation = std::make_pair(allWordsWithPairs[i][0], allWordsWithPairs[i][j]);

                std::pair<std::pair<Corpus2::Token*, Corpus2::Token*>, double> collocationWithRank;
                //ranking z-score
                double rZScore = 0;
                //liczba wystapien pary slow (w1, w2) w korpusie
                int pairCount = ws->getNumberOfInstancesOfPair(allWordsWithPairs[i][0], allWordsWithPairs[i][j]);
                //liczba wystapien slowa w2 w korpusie
                int word2Count = ws->getNumberOfInstancesOfWord(allWordsWithPairs[i][j]);
                //wartosc oczekiwania liczby wystapien pary (w1, w2) w korpusie
                double E = (word1Count * word2Count) / (double)n;
                //pierwiastek z wariancji
                double D = sqrt((word1Count * word2Count) / (double)n);
                rZScore = (pairCount - E) / D;
                collocationWithRank = std::make_pair(collocation, rZScore);

                collocationsRank.push_back(collocationWithRank);
            }
        }
    }
}
