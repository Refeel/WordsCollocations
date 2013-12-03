#include "wordsstatistics.h"

WordsStatistics::WordsStatistics(Corpus2::XcesReader &xr)
{
    xreader = &xr;
    tagset = xreader->tagset();
    wordsCount = 0;
    signsOfPunctuation = boost::assign::list_of(",")(".")("·")("(")(")")("!")("?")(":")(";")("-")("\\")("/")("\“");
    ignoredWords = boost::assign::list_of("i")("o")("się")("w")("we")("z")("ze");

    makeStatistics( *xreader );
}

bool WordsStatistics::isSignOfPunctuation(Corpus2::Token *token, std::vector <std::string> signOfPunctuationTab)
{
    for(int i=0; i<SIZE_OF_PUNCTUATION; i++)
    {
        if(token->orth_utf8().c_str() == signOfPunctuationTab[i])
            return true;
    }

    return false;
}

bool WordsStatistics::isIgnoredWords(Corpus2::Token *token, std::vector <std::string> ignoredWordsTab)
{
    for(int i=0; i<SIZE_OF_IGNORED_WORDS; i++)
    {
        if(token->orth_utf8().c_str() == ignoredWordsTab[i])
            return true;
    }

    return false;
}

bool WordsStatistics::isNewToken(Corpus2::Token *token)
{
    for(size_t i=0; i<allWords.size(); i++)
    {
        if(token->get_preferred_lexeme(tagset).lemma_utf8().compare(allWords[i][0]->get_preferred_lexeme(tagset).lemma_utf8()) == 0)
            return false;
    }

    return true;
}

bool WordsStatistics::isNewPairTokens(Corpus2::Token *token, int positionOfWord)
{
    for(size_t i=1; i<allWords[positionOfWord].size(); i++)
    {
        if(token->get_preferred_lexeme(tagset).lemma_utf8().compare(allWords[positionOfWord][i]->get_preferred_lexeme(tagset).lemma_utf8()) == 0)
            return false;
    }

    return true;
}

int WordsStatistics::getPositionOfWord(Corpus2::Token *token)
{
    for(size_t i=0; i<allWords.size(); i++)
    {
        if(token->get_preferred_lexeme(tagset).lemma_utf8().compare(allWords[i][0]->get_preferred_lexeme(tagset).lemma_utf8()) == 0)
        {
            return i;
        }
    }

    return -1;
}

int WordsStatistics::getPositionOfPair(Corpus2::Token *token, int positionOfWord)
{
    for(size_t i=1; i<allWords[positionOfWord].size(); i++)
    {
        if(token->get_preferred_lexeme(tagset).lemma_utf8().compare(allWords[positionOfWord][i]->get_preferred_lexeme(tagset).lemma_utf8()) == 0)
        {
            return i;
        }
    }

    return -1;
}

void WordsStatistics::makeStatistics(Corpus2::XcesReader &xr)
{
    Corpus2::Token *token;
    Corpus2::Token *nextToken;

    token = xr.get_next_token();
    while(nextToken = xr.get_next_token())
    {
        wordsCount++;

        if(isSignOfPunctuation(nextToken, signsOfPunctuation) == false)
        {
            if(isNewToken(token) == true)
            {
                std::vector <Corpus2::Token*> tknVec(1);
                tknVec[0] = token;
                allWords.push_back(tknVec);

                int one = 1;
                //zliczanie wystapien danego slowa
                std::vector <int> numVec(1);
                numVec[0] = one;
                numberAllWords.push_back(numVec);

                while(isIgnoredWords(nextToken, ignoredWords) == true)
                {
                    nextToken = xr.get_next_token();
                }

                if(nextToken != NULL)
                {
                    allWords.back().push_back(nextToken);

                    //zliczanie wystapien danego slowa ze slowem nastepnym
                    numberAllWords.back().push_back(one);
                }
            }
            else
            {
                while(isIgnoredWords(nextToken, ignoredWords) == true)
                {
                    nextToken = xr.get_next_token();
                }

                if(nextToken != NULL)
                {
                    numberAllWords[getPositionOfWord(token)][0] += 1;
                    if(isNewPairTokens(nextToken, getPositionOfWord(token)) == true)
                    {
                        allWords[getPositionOfWord(token)].push_back(nextToken);
                        int one = 1;
                        numberAllWords[getPositionOfWord(token)].push_back(one);
                    }
                    else
                    {
                        numberAllWords[getPositionOfWord(token)][getPositionOfPair(nextToken, getPositionOfWord(token))] += 1;
                    }
                }
            }

            token = nextToken;
        }
        else
        {
            if(isSignOfPunctuation(token, signsOfPunctuation) == false)
            {
                if(isNewToken(token) == true)
                {
                    std::vector <Corpus2::Token*> tknVec(1);
                    tknVec[0] = token;
                    allWords.push_back(tknVec);

                    int one = 1;
                    //zliczanie wystapien danego slowa
                    std::vector <int> numVec(1);
                    numVec[0] = one;
                    numberAllWords.push_back(numVec);
                }
                else
                {
                    numberAllWords[getPositionOfWord(token)][0] += 1;
                }
            }

            nextToken = xr.get_next_token();
            token = nextToken;
        }
    }
}




int WordsStatistics::getNumberOfInstancesOfWord(Corpus2::Token *token)
{
    for(size_t i=0; i<allWords.size(); i++)
    {
        if(token->get_preferred_lexeme(tagset).lemma_utf8().compare(allWords[i][0]->get_preferred_lexeme(tagset).lemma_utf8()) == 0)
        {
            return numberAllWords[i][0];
        }
    }

    return 0;
}

int WordsStatistics::getNumberOfInstancesOfPair(Corpus2::Token *token1, Corpus2::Token *token2)
{
    for(size_t i=0; i<allWords.size(); i++)
    {
        if(token1->get_preferred_lexeme(tagset).lemma_utf8().compare(allWords[i][0]->get_preferred_lexeme(tagset).lemma_utf8()) == 0)
        {
            for(size_t j=1; j<allWords[i].size(); j++)
            {
                if(token2->get_preferred_lexeme(tagset).lemma_utf8().compare(allWords[i][j]->get_preferred_lexeme(tagset).lemma_utf8()) == 0)
                {
                    return numberAllWords[i][j];
                }
            }
        }
    }

    return 0;
}

std::vector<std::vector<Corpus2::Token*> > & WordsStatistics::getAllWordsWithPairs()
{
    return allWords;
}

int WordsStatistics::getWordsCount()
{
    return wordsCount;
}
