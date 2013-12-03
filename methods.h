#ifndef METHODS_H
#define METHODS_H

#include "wordsstatistics.h"

#include <math.h>

#include <boost/bind.hpp>

class Methods
{
public:
    Methods(WordsStatistics *wordsStats);


    std::vector<std::pair<std::pair<Corpus2::Token*, Corpus2::Token*>, double> > collocationsRankFSCP;
    std::vector<std::pair<std::pair<Corpus2::Token*, Corpus2::Token*>, double> > collocationsRankZScore;
    std::vector<std::pair<std::pair<Corpus2::Token*, Corpus2::Token*>, double> > collocationsRankPMI;


    void ranking(WordsStatistics *ws);

    //wartosc oczekiwania liczby wystapien pary (w1, w2) w korpusie
    double getE(double numberOfInstancesOfWord1, double numberOfInstancesOfWord2, double numberOfAllInstances);
    //pierwiastek z wariancji
    double getD(double numberOfInstancesOfWord1, double numberOfInstancesOfWord2, double numberOfAllInstances);

    //prawdopodobienstwo obserwowane wystąpienia pary slow p(w1, w2) w korpusie
    double getPropabilityOfWordPair(double numberOfInstancesOfWordPair, double numberOfAllInstances);
    //prawdopodobieństwo wystąpienia pojedyńczego słowa p(w) w korpusie
    double getPropabilityOfWord(double numberOfInstancesOfWord, double numberOfAllInstances);

    double getFSCPRank(double numberOfInstancesOfWordPair, double numberOfInstancesOfWord1, double numberOfInstancesOfWord2);
    double getZScoreRank(double numberOfInstancesOfWordPair, double numberOfInstancesOfWord1, double numberOfInstancesOfWord2, double numberOfAllInstances);
    double getPMIRank(double numberOfInstancesOfWordPair, double numberOfInstancesOfWord1, double numberOfInstancesOfWord2, double numberOfAllInstances);

    void sortingRanking();
};

#endif // METHODS_H
