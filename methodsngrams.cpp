#include "methodsngrams.h"
#include "QStringList"

#include <boost/bind.hpp>

MethodsNGrams::MethodsNGrams(WordsStatisticNGrams *wordsStats)
{
    ranking(wordsStats);
}

void MethodsNGrams::ranking(WordsStatisticNGrams *ws)
{
    QString collocation = "";

    QHash<QString, int>::const_iterator iter = ws->wordsStatistic[ws->getN()-1].constBegin();
    while(iter != ws->wordsStatistic[ws->getN()-1].constEnd())
    {
        collocation = iter.key();

        double avp = Avp(ws, collocation);
        double rankSI = getSIRank(ws, avp, collocation);
        double rankSCP = getSCPRank(ws, avp, collocation);

        collocationsRankSI.push_back(std::make_pair<QString, double>(collocation, rankSI));
        collocationsRankSCP.push_back(std::make_pair<QString, double>(collocation, rankSCP));

        iter++;
    }


//    std::sort(collocationsRankSI.begin(), collocationsRankSI.end(),
//            boost::bind(&std::pair<QString, double>::second, _1) >
//            boost::bind(&std::pair<QString, double>::second, _2));

//    std::sort(collocationsRankSCP.begin(), collocationsRankSCP.end(),
//            boost::bind(&std::pair<QString, double>::second, _1) >
//            boost::bind(&std::pair<QString, double>::second, _2));

}

double MethodsNGrams::Avp(WordsStatisticNGrams *ws, QString collocation)
{
    QStringList wordsInColl = collocation.split(" ");
    double avpSum = 0;

    for(int pivot=1; pivot<ws->getN(); pivot++)
    {
        QString left = "";  double leftCnt = 0;
        QString right = ""; double rightCnt = 0;

        for(int p1=0; p1<pivot; p1++)
        {
            left.append(wordsInColl[p1]).append(" ");
        }
        left.chop(1);

        for(int p2=pivot; p2<ws->getN(); p2++)
        {
            right.append(wordsInColl[p2]).append(" ");
        }
        right.chop(1);

        leftCnt = ws->wordsStatistic[pivot-1][left];
        rightCnt = ws->wordsStatistic[ws->getN()-pivot-1][right];

        avpSum += (leftCnt / (double)ws->getWordsCount()) * (rightCnt / (double)ws->getWordsCount());
    }

    return (avpSum / (double)(ws->getN()-1));
}

double MethodsNGrams::getSIRank(WordsStatisticNGrams *ws, double avp, QString collocation)
{
    double collCnt = ws->wordsStatistic[ws->getN()-1][collocation];

    double collPropability = collCnt / ws->getWordsCount();

    return log2( collPropability / avp );
}

double MethodsNGrams::getSCPRank(WordsStatisticNGrams *ws, double avp, QString collocation)
{
    double collCnt = ws->wordsStatistic[ws->getN()-1][collocation];

    double collPropability = collCnt / ws->getWordsCount();

    return ((collPropability * collPropability) / avp);
}

