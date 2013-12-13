#include "methods.h"
#include "QStringList"

Methods::Methods(WordsStatisticNGrams *wordsStats)
{
    ranking(wordsStats);
}

Methods::~Methods()
{

}

void Methods::ranking(WordsStatisticNGrams *ws)
{
    QString collocation = "";
    int biGram = 2;

    QHash<QString, int>::const_iterator iter = ws->wordsStatistic[biGram-1].constBegin();
    while(iter != ws->wordsStatistic[biGram-1].constEnd())
    {
        collocation = iter.key();

        QStringList wordsInColl = collocation.split(" ");
        int numberOfInstancesOfWord1 = ws->wordsStatistic[0][wordsInColl[0]];
        int numberOfInstancesOfWord2 = ws->wordsStatistic[0][wordsInColl[1]];
        int numberOfInstancesOfWordPair = ws->wordsStatistic[biGram-1][collocation];
        int allWordsInCorpusCount = ws->getWordsCount();

        double rankFSCP = getFSCPRank((double)numberOfInstancesOfWordPair, (double)numberOfInstancesOfWord1,(double) numberOfInstancesOfWord2);
        double rankZScore = getZScoreRank((double)numberOfInstancesOfWordPair, (double)numberOfInstancesOfWord1, (double)numberOfInstancesOfWord2, (double)allWordsInCorpusCount);
        double rankPMI = getPMIRank((double)numberOfInstancesOfWordPair, (double)numberOfInstancesOfWord1, (double)numberOfInstancesOfWord2, (double)allWordsInCorpusCount);

        collocationsRankFSCP.push_back(std::make_pair(collocation, rankFSCP));
        collocationsRankZScore.push_back(std::make_pair(collocation, rankZScore));
        collocationsRankPMI.push_back(std::make_pair(collocation, rankPMI));

        iter++;
    }
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
