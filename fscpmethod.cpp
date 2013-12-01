/*************************************************************************************
//      FSCP (Frequency Biased Symmetric Conditional Propability):                  //
//                                                                                  //
//      Przyjmuje wartości od 0 (słowa w ogóle nie występują obok siebie)           //
//      do 1 (słowa występują tylko i wyłącznie obok siebie).                       //
//                                                                                  //
//      miary częstościowe - miary preferujące kolokacjie występujące często        //
//      miary zależnościowe - miary preferujące kolokacje występujące rzedko        //
//                                                                                  //
//      FSCP jest skrzyżowaniem miar z obu powyższych grup.                         //
//                                                                                  //
*************************************************************************************/

#include "fscpmethod.h"

#include <math.h>

FSCPMethod::FSCPMethod(WordsStatistics *wordsStats)
{
    fscpRanking(wordsStats);
}

void FSCPMethod::fscpRanking(WordsStatistics *ws)
{
    std::vector<std::vector<Corpus2::Token*> > allWordsWithPairs = ws->getAllWordsWithPairs();

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

                //ranking Frequency Biased Symmetric Conditional Propability
                double rFSCP = 0;

                //liczba wystapien pary slow (w1, w2) w korpusie
                int pairCount = ws->getNumberOfInstancesOfPair(allWordsWithPairs[i][0], allWordsWithPairs[i][j]);

                //liczba wystapien slowa w2 w korpusie
                int word2Count = ws->getNumberOfInstancesOfWord(allWordsWithPairs[i][j]);

                rFSCP = (pow(pairCount, 3)) / (word1Count * word2Count);
                collocationWithRank = std::make_pair(collocation, rFSCP);

                collocationsRank.push_back(collocationWithRank);
            }
        }
    }
}
