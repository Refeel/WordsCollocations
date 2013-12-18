#include "wordsstatisticngrams.h"

#include <libcorpus2/ann/annotatedsentence.h>
#include <libcorpus2/io/reader.h>

WordsStatisticNGrams::WordsStatisticNGrams(boost::shared_ptr<Corpus2::TokenReader> &reader, unsigned n)
{
    this->reader = reader.get();
    tagset = reader->tagset();

    setN(n);
    wordsCount = 0;

    segmentedSigns.clear();
    ignoredWords.clear();

    setFilterNumbersEnabled(false);
    setFilterProperNamesEnabled(false);
}

WordsStatisticNGrams::~WordsStatisticNGrams()
{
}



void WordsStatisticNGrams::makeStatistics()
{
    makeStatistics(*reader);
}



void WordsStatisticNGrams::setN(unsigned n)
{
    this->n = n;
}

int WordsStatisticNGrams::getN()
{
    return this->n;
}

int WordsStatisticNGrams::getWordsCount()
{
    return wordsCount;
}

void WordsStatisticNGrams::addSegmentationSign(QString sign)
{
    segmentedSigns.push_back(sign);
}

void WordsStatisticNGrams::addIgnoredWord(std::string word)
{
    ignoredWords.push_back(word);
}

void WordsStatisticNGrams::setFilterNumbersEnabled(bool filter)
{
    filterNumbers = filter;
}

void WordsStatisticNGrams::setFilterProperNamesEnabled(bool filter)
{
    filterProperNames = filter;
}



void WordsStatisticNGrams::makeStatistics(Corpus2::TokenReader &reader)
{
    wordsStatistic.resize(n);

    Corpus2::Sentence::Ptr sentence;

    while(sentence = reader.get_next_sentence())
    {
        makeStatisticsForSentence(sentence);
    }

    return;
}

void WordsStatisticNGrams::makeStatisticsForSentence(Corpus2::Sentence::Ptr sentence)
{
    std::vector<Corpus2::Token*> tokensInSentence = sentence->tokens();

    //wskazowka dla pawla:
    Corpus2::AnnotatedSentence::Ptr annSentence = Corpus2::AnnotatedSentence::wrap_sentence(sentence);

    TokenIterator xr(tokensInSentence);

    std::vector<QString> tokens;

    for(size_t i=0; i<n-1; i++)
    {
        Corpus2::Token* token = xr.get_next_token();
        wordsCount++;
        while(isIgnored(token) == true)
        {
            token = xr.get_next_token();

            if(token == NULL)
            {
                countLastWords(tokens);
                return;
            }
            else
            {
                wordsCount++;
            }
        }

        tokens.push_back(token->get_preferred_lexeme(tagset).lemma_utf8().c_str());
    }

    Corpus2::Token* token;
    while(token = xr.get_next_token())
    {
        wordsCount++;

        //usuniecie ostatniego tokenu z wektora, gdyz zostanie on zastapiony nowym
        if(tokens.size() == n)
            tokens.pop_back();

        while(isIgnored(token) == true)
        {
            token = xr.get_next_token();

            if(token == NULL)
            {
                countLastWords(tokens);
                return;
            }
            else
            {
                wordsCount++;
            }
        }

        //dodanie nowego tokenu do wektora
        tokens.push_back(token->get_preferred_lexeme(tagset).lemma_utf8().c_str());

        std::vector<int> positionsOfSegmentedSigns = isSegmentedSign(tokens);
        if(positionsOfSegmentedSigns.size() != 0)
        {
            for(int position=0; position<positionsOfSegmentedSigns.size(); position++)
            {
                int numbersOfWords = 0;
                int from = 0;
                if(position == 0)
                {
                    numbersOfWords = positionsOfSegmentedSigns[position];
                    from = 0;
                }
                else
                {
                    numbersOfWords = positionsOfSegmentedSigns[position] - positionsOfSegmentedSigns[position-1] - 1;
                    from = positionsOfSegmentedSigns[position-1] + 1;
                }

                for(int i=1; i<=numbersOfWords; i++)
                {
                    for(int j=0; j<numbersOfWords-i+1; j++)
                    {
                        QString elements = "";
                        for(int k=0; k<i; k++)
                        {
                            elements.append(tokens[from+j+k]).append(" ");
                        }
                        elements.chop(1);

                        bool exist = wordsStatistic[i-1].contains(elements);

                        if(!exist)
                        {
                            wordsStatistic[i-1].insert(elements, 1);
                        }
                        else
                        {
                            wordsStatistic[i-1][elements]++;
                        }
                    }
                }
            }

            int wordsInTokens = 0;
            for(int i=positionsOfSegmentedSigns[positionsOfSegmentedSigns.size()-1]+1, j=0; i<n; i++, j++)
            {
                tokens[j] = tokens[i];
                wordsInTokens++;
            }

            for(int i=wordsInTokens; i<n-1; i++)
            {
                Corpus2::Token* newToken = xr.get_next_token();
                if(newToken == NULL)
                {
                    countLastWords(tokens);
                    return;
                }

                wordsCount++;

                while(isIgnored(newToken) == true)
                {
                    newToken = xr.get_next_token();

                    if(newToken == NULL)
                    {
                        countLastWords(tokens);
                        return;
                    }
                    else
                    {
                        wordsCount++;
                    }
                }

                tokens[i] = newToken->get_preferred_lexeme(tagset).lemma_utf8().c_str();
            }
        }
        else
        {
            for(unsigned pivot=1; pivot<=n; pivot++)
            {
                if(filterNumbers && pivot == n)
                {
                    if(numberFilter(tokens) == true)
                        break;
                }

                QString left = "";

                for(unsigned p1=0; p1<pivot; p1++)
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

            for(size_t i=0; i<tokens.size()-1; i++)
                tokens[i] = tokens[i+1];
        }
    }

    //dodanie ostatnich slow do statystyki
    if(tokens.size() == n)
        tokens.pop_back();

    countLastWords(tokens);

/*
    //wypisywanie statystyki
    for(int i=0; i<wordsStatistic.size(); i++)
    {
        QHashIterator<QString, int> c(wordsStatistic[i]);
        while(c.hasNext())
        {
            c.next();
            qDebug() << c.key() << c.value();
        }
    }

    qDebug() << "liczba slow: " << wordsCount;
*/

}




bool WordsStatisticNGrams::isIgnored(Corpus2::Token *token)
{
    for(size_t i=0; i<ignoredWords.size(); i++)
    {
        if(ignoredWords[i].compare(token->orth_utf8()) == 0)
        {
            return true;
        }
    }

    return false;
}

std::vector<int> WordsStatisticNGrams::isSegmentedSign(std::vector<QString> tokens)
{
    std::vector<int> positionOfSigns;
    for(size_t i=0; i<segmentedSigns.size(); i++)
    {
        for(size_t j=0; j<tokens.size(); j++)
        {
            if(segmentedSigns[i].compare(tokens[j]) == 0)
            {
                positionOfSigns.push_back(j);
            }
        }
    }

    std::sort(positionOfSigns.begin(), positionOfSigns.end());

    return positionOfSigns;
}




bool WordsStatisticNGrams::numberFilter(std::vector<QString> tokens)
{
    for(unsigned i=0; i<tokens.size(); i++)
    {
        for(int j=0; j<tokens[i].size(); j++)
        {
            if(tokens[i][j].isDigit() == true)
            {
                return true;
            }
        }
    }

    return false;
}




void WordsStatisticNGrams::countLastWords(std::vector<QString> tokens)
{
    for(int i=1; i<=tokens.size(); i++)
    {
        for(int j=0; j<tokens.size()-i+1; j++)
        {
            QString elements = "";
            for(int k=0; k<i; k++)
            {
                elements.append(tokens[j+k]).append(" ");
            }
            elements.chop(1);

            bool exist = wordsStatistic[i-1].contains(elements);

            if(!exist)
            {
                wordsStatistic[i-1].insert(elements, 1);
            }
            else
            {
                wordsStatistic[i-1][elements]++;
            }
        }
    }
}
