#include "methods.h"

Methods::Methods(WordsStatistics *wordsStats)
{
    ranking(wordsStats);
}

void Methods::ranking(WordsStatistics *ws)
{
    std::vector<std::vector<Corpus2::Token*> > & allWordsWithPairs = ws->getAllWordsWithPairs();

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

                //liczba wystapien pary slow (w1, w2) w korpusie
                int pairCount = ws->getNumberOfInstancesOfPair(allWordsWithPairs[i][0], allWordsWithPairs[i][j]);

                //liczba wystapien slowa w2 w korpusie
                int word2Count = ws->getNumberOfInstancesOfWord(allWordsWithPairs[i][j]);

                //rankingi
                double rFSCP = getFSCPRank((double)pairCount, (double)word1Count, (double)word2Count);
                double rZScore = getZScoreRank((double)pairCount, (double)word1Count, (double)word2Count, (double)n);
                double rPMI = getPMIRank((double)pairCount, (double)word1Count, (double)word2Count, (double)n);

                collocationWithRank = std::make_pair(collocation, rFSCP);
                collocationsRankFSCP.push_back(collocationWithRank);

                collocationWithRank = std::make_pair(collocation, rZScore);
                collocationsRankZScore.push_back(collocationWithRank);

                collocationWithRank = std::make_pair(collocation, rPMI);
                collocationsRankPMI.push_back(collocationWithRank);
            }
        }
    }

    sortingRanking();
}




double Methods::getE(double numberOfInstancesOfWord1, double numberOfInstancesOfWord2, double numberOfAllInstances)
{
    return (numberOfInstancesOfWord1 * numberOfInstancesOfWord2) / numberOfAllInstances;
}

double Methods::getD(double numberOfInstancesOfWord1, double numberOfInstancesOfWord2, double numberOfAllInstances)
{
    return sqrt((numberOfInstancesOfWord1 * numberOfInstancesOfWord2) / numberOfAllInstances);
}

double Methods::getPropabilityOfWordPair(double numberOfInstancesOfWordPair, double numberOfAllInstances)
{
    return numberOfInstancesOfWordPair / numberOfAllInstances;
}

double Methods::getPropabilityOfWord(double numberOfInstancesOfWord, double numberOfAllInstances)
{
    return numberOfInstancesOfWord / numberOfAllInstances;
}




double Methods::getFSCPRank(double numberOfInstancesOfWordPair, double numberOfInstancesOfWord1, double numberOfInstancesOfWord2)
{
    return (pow(numberOfInstancesOfWordPair, 3)) / (numberOfInstancesOfWord1 * numberOfInstancesOfWord2);
}

double Methods::getZScoreRank(double numberOfInstancesOfWordPair, double numberOfInstancesOfWord1, double numberOfInstancesOfWord2, double numberOfAllInstances)
{
    return (numberOfInstancesOfWordPair - getE(numberOfInstancesOfWord1, numberOfInstancesOfWord2, numberOfAllInstances)) / getD(numberOfInstancesOfWord1, numberOfInstancesOfWord2, numberOfAllInstances);
}

double Methods::getPMIRank(double numberOfInstancesOfWordPair, double numberOfInstancesOfWord1, double numberOfInstancesOfWord2, double numberOfAllInstances)
{
    return log2((getPropabilityOfWordPair(numberOfInstancesOfWordPair, numberOfAllInstances)) / (getPropabilityOfWord(numberOfInstancesOfWord1, numberOfAllInstances) * getPropabilityOfWord(numberOfInstancesOfWord2, numberOfAllInstances)));
}




void Methods::sortingRanking()
{
    std::sort(collocationsRankFSCP.begin(), collocationsRankFSCP.end(),
            boost::bind(&std::pair<std::pair<Corpus2::Token*, Corpus2::Token*>, double>::second, _1) >
            boost::bind(&std::pair<std::pair<Corpus2::Token*, Corpus2::Token*>, double>::second, _2));

    std::sort(collocationsRankZScore.begin(), collocationsRankZScore.end(),
            boost::bind(&std::pair<std::pair<Corpus2::Token*, Corpus2::Token*>, double>::second, _1) >
            boost::bind(&std::pair<std::pair<Corpus2::Token*, Corpus2::Token*>, double>::second, _2));

    std::sort(collocationsRankPMI.begin(), collocationsRankPMI.end(),
            boost::bind(&std::pair<std::pair<Corpus2::Token*, Corpus2::Token*>, double>::second, _1) >
            boost::bind(&std::pair<std::pair<Corpus2::Token*, Corpus2::Token*>, double>::second, _2));
}
