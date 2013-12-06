#ifndef METHODSNGRAMS_H
#define METHODSNGRAMS_H

#include "wordsstatisticngrams.h"

class MethodsNGrams
{
public:
    MethodsNGrams(WordsStatisticNGrams *wordsStats);

    std::vector<std::pair<QString, double> > collocationsRankSI;
    std::vector<std::pair<QString, double> > collocationsRankSCP;

private:
    void ranking(WordsStatisticNGrams *ws);

    double Avp(WordsStatisticNGrams *ws, QString collocation);

    double getSIRank(WordsStatisticNGrams *ws, double avp, QString collocation);
    double getSCPRank(WordsStatisticNGrams *ws, double avp, QString collocation);
};

#endif // METHODSNGRAMS_H
