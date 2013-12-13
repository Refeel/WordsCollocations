#ifndef METHODS_H
#define METHODS_H

#include "wordsstatisticngrams.h"
#include "math.h"

class Methods
{
public:
    Methods(WordsStatisticNGrams *wordsStats);
    ~Methods();

    std::vector<std::pair<QString, double> > collocationsRankFSCP;
    std::vector<std::pair<QString, double> > collocationsRankZScore;
    std::vector<std::pair<QString, double> > collocationsRankPMI;

private:
    void ranking(WordsStatisticNGrams *ws);

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
};

#endif // METHODS_H
