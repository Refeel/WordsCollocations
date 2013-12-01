#include "fscpmethod.h"

#include <math.h>

FSCPMethod::FSCPMethod(WordsStatistics *wordsStats)
{
    fscpRanking(wordsStats);
}

void FSCPMethod::fscpRanking(WordsStatistics *ws)
{
    std::vector<std::vector<Corpus2::Token*> > & allWordsWithPairs = ws->getAllWordsWithPairs();

    for(size_t i=0; i<allWordsWithPairs.size(); i++)
    {
        if(allWordsWithPairs[i].size() > 1)
        {
            int word1Count = ws->getNumberOfInstancesOfWord(allWordsWithPairs[i][0]);
            for(size_t j=1; j<allWordsWithPairs[i].size(); j++)
            {
                std::pair<Corpus2::Token*, Corpus2::Token*> collocation;
                collocation = std::make_pair(allWordsWithPairs[i][0], allWordsWithPairs[i][j]);

                std::pair<std::pair<Corpus2::Token*, Corpus2::Token*>, double> collocationWithRank;
                double rFSCP = 0;
                int pairCount = ws->getNumberOfInstancesOfPair(allWordsWithPairs[i][0], allWordsWithPairs[i][j]);
                int word2Count = ws->getNumberOfInstancesOfWord(allWordsWithPairs[i][j]);
                rFSCP = (pow(pairCount, 3)) / (word1Count * word2Count);
                collocationWithRank = std::make_pair(collocation, rFSCP);

                collocationsRank.push_back(collocationWithRank);
            }
        }
    }
}
