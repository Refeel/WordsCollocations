#ifndef WORDSSTATISTICNGRAMS_H
#define WORDSSTATISTICNGRAMS_H

#include <libcorpus2/io/xcesreader.h>

#include <QDebug>
#include <QString>
#include <QHash>

class WordsStatisticNGrams
{
public:
    WordsStatisticNGrams(boost::shared_ptr<Corpus2::TokenReader> &reader, unsigned n);
    ~WordsStatisticNGrams();

    void setN(unsigned n);
    int getN();

    int getWordsCount();

    void makeStatistics();

    void addSegmentationSign(QString sign);
    void addIgnoredWord(std::string word);
    void setFilterNumbersEnabled(bool filter);
    void setFilterProperNamesEnabled(bool filter);

    std::vector<QHash<QString, int> > wordsStatistic;

private:
    Corpus2::TokenReader *reader;
    Corpus2::Tagset tagset;

    unsigned int n;

    int wordsCount;
    bool filterNumbers;
    bool filterProperNames;

    std::vector<QString> segmentedSigns;
    std::vector<std::string> ignoredWords;

    void makeStatistics(Corpus2::TokenReader &reader);
    void makeStatisticsForSentence(Corpus2::Sentence::Ptr sentence);

    bool isIgnored(Corpus2::Token *token);
    std::vector<int> isSegmentedSign(std::vector<QString> tokens);

    bool numberFilter(std::vector<QString> tokens);
    bool properNameFilter(std::vector<QString> tokens);


    void countLastWords(std::vector<QString> tokens);

    class TokenIterator
    {
        public:
            TokenIterator(std::vector<Corpus2::Token*> &sentence)
            {
                m_cont_iter = sentence.begin();
                curr = 0;
                size = sentence.size();
            }

            Corpus2::Token* get_next_token()
            {
                if(curr < size)
                    return *(m_cont_iter + curr++);
                else
                    return NULL;
            }

        private:
            size_t curr;
            size_t size;
            std::vector<Corpus2::Token *>::const_iterator m_cont_iter;
    };
};

#endif // WORDSSTATISTICNGRAMS_H
