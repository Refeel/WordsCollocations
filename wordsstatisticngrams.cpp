#include "wordsstatisticngrams.h"

WordsStatisticNGrams::WordsStatisticNGrams(Corpus2::XcesReader &xr, unsigned n)
{
    xreader = &xr;
    tagset = xreader->tagset();

    setN(n);
    wordsCount = 0;

    makeStatistics( *xreader );
}

void WordsStatisticNGrams::setN(unsigned n)
{
    this->n = n;
}

int WordsStatisticNGrams::getN()
{
    return this->n;
}

void WordsStatisticNGrams::makeStatistics(Corpus2::XcesReader &xr)
{
    wordsStatistic.resize(n);

    std::vector<QString> tokens(n);

    for(int i=0; i<n-1; i++)
    {
        Corpus2::Token* token = xr.get_next_token();
        tokens[i] = token->get_preferred_lexeme(tagset).lemma_utf8().c_str();
    }

    Corpus2::Token* token;
    while(token = xr.get_next_token())
    {
        wordsCount++;

        tokens[n-1] = token->get_preferred_lexeme(tagset).lemma_utf8().c_str();

        for(int pivot=1; pivot<=n; pivot++)
        {
            QString left="";

            for(int p1=0; p1 < pivot; p1++)
            {
                left.append(tokens[p1]).append(" ");
            }
            left.chop(1);

            bool exists = wordsStatistic[pivot-1].contains(left);

            if(!exists)
            {
                wordsStatistic[pivot-1].insert(left, 1);
            }
            else
            {
                wordsStatistic[pivot-1][left]++;
            }
        }

        for(int i=0; i<tokens.size()-1; i++)
            tokens[i] = tokens[i+1];
    }
}


int WordsStatisticNGrams::getWordsCount()
{
    return wordsCount;
}
