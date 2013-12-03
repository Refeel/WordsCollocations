#ifndef WORDSSTATISTICS_H
#define WORDSSTATISTICS_H

#include <vector>

#include <libcorpus2/tagsetmanager.h>
#include <libcorpus2/io/xcesreader.h>
#include <boost/assign/list_of.hpp>

#define SIZE_OF_PUNCTUATION 13
#define SIZE_OF_IGNORED_WORDS 7

class WordsStatistics
{
public:
    WordsStatistics(Corpus2::XcesReader &xr);

    //metody
    int getNumberOfInstancesOfWord(Corpus2::Token *token);
    int getNumberOfInstancesOfPair(Corpus2::Token *token1, Corpus2::Token *token2);
    std::vector<std::vector<Corpus2::Token *> > &getAllWordsWithPairs();
    int getWordsCount();

private:
    Corpus2::XcesReader *xreader;
    Corpus2::Tagset tagset;
    std::vector<std::vector<int> > numberAllWords;
    std::vector<std::vector<Corpus2::Token*> > allWords;
    int wordsCount;
    std::vector <std::string> signsOfPunctuation;
    std::vector <std::string> ignoredWords;

    //metody
    bool isSignOfPunctuation(Corpus2::Token *token, std::vector <std::string> signOfPunctuationTab);
    bool isIgnoredWords(Corpus2::Token *token, std::vector <std::string> ignoredWordsTab);
    bool isNewToken(Corpus2::Token *token);
    bool isNewPairTokens(Corpus2::Token *token, int positionOfWord);
    int getPositionOfWord(Corpus2::Token *token);
    int getPositionOfPair(Corpus2::Token *token, int positionOfWord);

    void makeStatistics(Corpus2::XcesReader &xr);
};

#endif // WORDSSTATISTICS_H
