#include "wordsstatisticngrams.h"

WordsStatisticNGrams::WordsStatisticNGrams(Corpus2::XcesReader &xr)
{
    xreader = &xr;
    tagset = xreader->tagset();

    setN(3);
    setSpan(2);

    makeStatistics( *xreader );
}

void WordsStatisticNGrams::setN(unsigned n)
{
    this->n = n;
}

void WordsStatisticNGrams::setSpan(unsigned span)
{
    this->span = span;
}

void WordsStatisticNGrams::makeStatistics(Corpus2::XcesReader &xr)
{
    Corpus2::Token *pivot;
    std::vector<std::pair<Corpus2::Token *, int> > wordsInSpans;

    int positionInContext = 0 - span;
    std::pair<Corpus2::Token *, int> wordWithPosition;

    for(int i=0; i<span; i++)
    {
        Corpus2::Token *token = xr.get_next_token();
        wordWithPosition = std::make_pair(token, positionInContext);
        wordsInSpans.push_back(wordWithPosition);
        positionInContext += 1;
    }

    pivot = xr.get_next_token();
    positionInContext += 1;

    for(int i=0; i<span-1; i++)
    {
        Corpus2::Token *token = xr.get_next_token();
        wordWithPosition = std::make_pair(token, positionInContext);
        wordsInSpans.push_back(wordWithPosition);
        positionInContext += 1;
    }

    Corpus2::Token *lastWordInContext;;
    while(lastWordInContext = xr.get_next_token())
    {
        wordsCount++;

        wordWithPosition = std::make_pair(lastWordInContext, positionInContext);
        wordsInSpans.push_back(wordWithPosition);

        if(isNewToken(pivot) == true)
        {
            std::pair<Corpus2::Token *, int> newWordWithCount;
            newWordWithCount = std::make_pair(pivot, 1);
            allWordsWithCount.push_back(newWordWithCount);

            std::vector<std::vector<int> > allSequences = generateSequencesWithoutRepetition(span, n-1);

            std::pair<std::vector<std::pair<Corpus2::Token*, int> >, int> nGramWithCount;
            std::vector<std::pair<std::vector<std::pair<Corpus2::Token*, int> >, int> > nGramsForOnePivot;
            for(size_t i=0; i<allSequences.size(); i++)
            {
                std::vector<std::pair<Corpus2::Token *, int> > vectorWordsWithPositions;
                for(size_t j=0; j<allSequences[i].size(); j++)
                {
                    for(size_t k=0; k<wordsInSpans.size(); k++)
                    {
                        if(allSequences[i][j] == wordsInSpans[k].second)
                        {
                            vectorWordsWithPositions.push_back(wordsInSpans[k]);
                            break;
                        }
                    }
                }
                nGramWithCount = std::make_pair(vectorWordsWithPositions, 1);
                nGramsForOnePivot.push_back(nGramWithCount);
            }
            nGramsWithCount.push_back(nGramsForOnePivot);
        }
        else
        {
            allWordsWithCount[getPositionOfWord(pivot)].second += 1;

            std::vector<std::vector<int> > allSequences = generateSequencesWithoutRepetition(span, n-1);

            std::pair<std::vector<std::pair<Corpus2::Token*, int> >, int> nGramWithCount;
            for(size_t i=0; i<allSequences.size(); i++)
            {
                std::vector<std::pair<Corpus2::Token *, int> > vectorWordsWithPositions;
                for(size_t j=0; j<allSequences[i].size(); j++)
                {
                    for(size_t k=0; k<wordsInSpans.size(); k++)
                    {
                        if(allSequences[i][j] == wordsInSpans[k].second)
                        {
                            vectorWordsWithPositions.push_back(wordsInSpans[k]);
                            break;
                        }
                    }
                }

                bool isNewNGram = true;
                int positionOfRepetitionNGram = -1;
                for(size_t j=0; j<nGramsWithCount[getPositionOfWord(pivot)].size(); j++)
                {
                    int isNewNGramCount = 0;
                    for(size_t k=0; k<nGramsWithCount[getPositionOfWord(pivot)][j].first.size(); k++)
                    {
                        if(nGramsWithCount[getPositionOfWord(pivot)][j].first[k].first->get_preferred_lexeme(tagset).lemma_utf8().compare(vectorWordsWithPositions[k].first->get_preferred_lexeme(tagset).lemma_utf8()) == 0 && nGramsWithCount[getPositionOfWord(pivot)][j].first[k].second == vectorWordsWithPositions[k].second)
                        {
                            isNewNGramCount++;
                        }
                    }

                    if(isNewNGramCount == nGramsWithCount[getPositionOfWord(pivot)][j].first.size())
                    {
                        isNewNGram = false;
                        positionOfRepetitionNGram = j;
                        break;
                    }
                }

                if(isNewNGram == true)
                {
                    nGramWithCount = std::make_pair(vectorWordsWithPositions, 1);
                    nGramsWithCount[getPositionOfWord(pivot)].push_back(nGramWithCount);
                }
                else
                {
                    nGramsWithCount[getPositionOfWord(pivot)][positionOfRepetitionNGram].second++;
                }
            }
        }

        std::vector<Corpus2::Token *> wordsInSpansCopyWithPivot;
        for(int i=1; i<span*2; i++)
        {
            if(i == span)
            {
                wordsInSpansCopyWithPivot.push_back(pivot);
                wordsInSpansCopyWithPivot.push_back(wordsInSpans[i].first);
            }
            else
                wordsInSpansCopyWithPivot.push_back(wordsInSpans[i].first);
        }

        wordsInSpans.clear();
        positionInContext = 0 - span;
        for(int i=0; i<span; i++)
        {
            wordWithPosition = std::make_pair(wordsInSpansCopyWithPivot[i], positionInContext);
            wordsInSpans.push_back(wordWithPosition);
            positionInContext++;
        }

        pivot = wordsInSpansCopyWithPivot[span];
        positionInContext++;

        for(int i=span+1; i<span*2; i++)
        {
            wordWithPosition = std::make_pair(wordsInSpansCopyWithPivot[i], positionInContext);
            wordsInSpans.push_back(wordWithPosition);
            positionInContext++;
        }
    }
}

bool WordsStatisticNGrams::isNewToken(Corpus2::Token *token)
{
    for(size_t i=0; i<allWordsWithCount.size(); i++)
    {
        if(token->get_preferred_lexeme(tagset).lemma_utf8().compare(allWordsWithCount[i].first->get_preferred_lexeme(tagset).lemma_utf8()) == 0)
            return false;
    }

    return true;
}

int WordsStatisticNGrams::getPositionOfWord(Corpus2::Token *token)
{
    for(size_t i=0; i<allWordsWithCount.size(); i++)
    {
        if(token->get_preferred_lexeme(tagset).lemma_utf8().compare(allWordsWithCount[i].first->get_preferred_lexeme(tagset).lemma_utf8()) == 0)
            return i;
    }

    return -1;
}

int WordsStatisticNGrams::factorial(int n)
{
    if(n == 1 || n == 0)
    {
        return 1;
    }
    else
    {
        return n * factorial(n - 1);
    }
}

int WordsStatisticNGrams::countingSequencesWithoutRepetition(int n, int k)
{
    return (factorial(n)) / (factorial(n - k));
}

std::vector<std::vector<int> > WordsStatisticNGrams::generateSequencesWithoutRepetition(int span, int n)
{
    srand(time(0));
    std::vector<std::vector<int> > allSequences;

    std::vector<int> positions;
    int position = 0 - span;
    for(int i=0; i<span*2+1; i++)
    {
        if(position != 0)
            positions.push_back(position);

        position++;
    }

    //wektor zawierajacy jeden z podzbiorow wszystkich wariacji
    int *subSet = new int[n];
    for(int i=0; i<n; i++)
        subSet[i] = 0;

    int numbersOfSequences = countingSequencesWithoutRepetition(span * 2, n);

    for(int i=0; i<numbersOfSequences; i++)
    {
        int count = 0;
        int repetition2 = 0;

        while(repetition2 == 0)
        {
            while(count < n)
            {
                int repetition = 0;
                int randPosition = rand() % (span * 2);
                int positionInRandPosition = positions[randPosition];
                for(int j=0; j<n; j++)
                {
                    if(subSet[j] == positionInRandPosition)
                    {
                        repetition = 1;
                        break;
                    }
                }

                if(repetition == 0)
                {
                    subSet[count] = positionInRandPosition;
                    count += 1;
                }
            }

            repetition2 = 1;
            for(size_t j=0; j<allSequences.size(); j++)
            {
                for(int k=0; k<n; k++)
                {
                    std::vector<int> asd = allSequences[j];
                    if(allSequences[j][k] == subSet[k])
                    {
                        repetition2 = 0;
                    }
                    else
                    {
                        repetition2 = 1;
                        break;
                    }
                }

                if(repetition2 == 0)
                {
                    for(int k=0; k<n; k++)
                    {
                        subSet[k] = 0;
                    }

                    count = 0;
                    break;
                }
            }
        }

        std::vector<int> sequence;
        for(int j=0; j<n; j++)
        {
            sequence.push_back(subSet[j]);
            subSet[j] = 0;
        }

        allSequences.push_back(sequence);
    }

    delete [] subSet;

    return allSequences;
}

int WordsStatisticNGrams::getWordsCount()
{
    return wordsCount;
}

int WordsStatisticNGrams::getNumberOfInstancesOfWord(Corpus2::Token *token)
{
    for(size_t i=0; i<allWordsWithCount.size(); i++)
    {
        if(token->get_preferred_lexeme(tagset).lemma_utf8().compare(allWordsWithCount[i].first->get_preferred_lexeme(tagset).lemma_utf8()) == 0)
        {
            return allWordsWithCount[i].second;
        }
    }

    return 0;
}

int WordsStatisticNGrams::getNumberOfInstancesOfNGram(Corpus2::Token *pivot, std::vector<std::pair<Corpus2::Token *, int> > nGram)
{
    for(size_t i=0; i<nGramsWithCount[getPositionOfWord(pivot)].size(); i++)
    {
        int count = 0;
        for(size_t j=0; j<nGramsWithCount[getPositionOfWord(pivot)][i].first.size(); j++)
        {
            if(nGramsWithCount[getPositionOfWord(pivot)][i].first[j].first->get_preferred_lexeme(tagset).lemma_utf8().compare(nGram[j].first->get_preferred_lexeme(tagset).lemma_utf8()) == 0 && nGramsWithCount[getPositionOfWord(pivot)][i].first[j].second == nGram[j].second)
            {
                count++;
            }
        }

        if(count == nGramsWithCount[getPositionOfWord(pivot)][i].first.size())
        {
            return nGramsWithCount[getPositionOfWord(pivot)][i].second;
        }
    }

    return 0;
}

std::vector<std::pair<Corpus2::Token*, std::vector<std::vector<std::pair<Corpus2::Token *, int> > > > > *WordsStatisticNGrams::getAllNGramsForAllPivots()
{
    std::vector<std::pair<Corpus2::Token*, std::vector<std::vector<std::pair<Corpus2::Token *, int> > > > > *vectorAllNGramsForAllPivots = new std::vector<std::pair<Corpus2::Token*, std::vector<std::vector<std::pair<Corpus2::Token *, int> > > > >();

    for(size_t i=0; i<nGramsWithCount.size(); i++)
    {
        std::vector<std::vector<std::pair<Corpus2::Token *, int> > > vectorNGramsForOnePivot;
        for(size_t j=0; j<nGramsWithCount[i].size(); j++)
        {
            vectorNGramsForOnePivot.push_back(nGramsWithCount[i][j].first);
        }
        std::pair<Corpus2::Token*, std::vector<std::vector<std::pair<Corpus2::Token *, int> > > > pair;
        pair = std::make_pair(allWordsWithCount[i].first, vectorNGramsForOnePivot);
        vectorAllNGramsForAllPivots->push_back(pair);
    }

    return vectorAllNGramsForAllPivots;
}
