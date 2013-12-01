#include "pmimethod.h"

#include <math.h>

PMIMethod::PMIMethod(WordsStatistics *wordsStats)
{
    pmiRanking(wordsStats);
}

void PMIMethod::pmiRanking(WordsStatistics *ws)
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
            //prawdopodobieństwo wystąpienia pojedyńczego słowa p(w1) w korpusie
            double p_w1 = word1Count / (double)n;

            for(size_t j=1; j<allWordsWithPairs[i].size(); j++)
            {
                std::pair<Corpus2::Token*, Corpus2::Token*> collocation;
                collocation = std::make_pair(allWordsWithPairs[i][0], allWordsWithPairs[i][j]);

                std::pair<std::pair<Corpus2::Token*, Corpus2::Token*>, double> collocationWithRank;
                //ranking Pointwise Mutual Information
                double rPMI = 0;

                //liczba wystapien pary slow (w1, w2) w korpusie
                int pairCount = ws->getNumberOfInstancesOfPair(allWordsWithPairs[i][0], allWordsWithPairs[i][j]);
                //prawdopodobienstwo obserwowane wystąpienia pary slow p(w1, w2) w korpusie
                double p_w1w2 = pairCount / (double)n;

                //liczba wystapien slowa w2 w korpusie
                int word2Count = ws->getNumberOfInstancesOfWord(allWordsWithPairs[i][j]);
                //prawdopodobieństwo wystąpienia pojedyńczego słowa p(w2) w korpusie
                double p_w2 = word2Count / (double)n;

                rPMI = log2((p_w1w2) / (p_w1 * p_w2));
                collocationWithRank = std::make_pair(collocation, rPMI);

                collocationsRank.push_back(collocationWithRank);
            }
        }
    }
}
