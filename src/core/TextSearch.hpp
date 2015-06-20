//=======================================================================
// Copyright 2008, Dalhousie University
// Author: Donovan Parks
//
// The contents of this file are licensed under the 
// Attribution-ShareAlike Creative Commons License:
// http://creativecommons.org/licenses/by-sa/3.0/
//=======================================================================

#ifndef _TEXTSEARCH_H_
#define _TEXTSEARCH_H_


#include "NodePhylo.hpp"
#include "Filter.hpp"
#include <QRegularExpression>

namespace pygmy
{

/**
 * @brief Search a list of words. Functionality is provided to
 *				filter the list of words based on prefix. The node assoicated
 *				with a word is stored for later retrieval.
 */
class TextSearch
{
public:
	/** Constructor. */
	TextSearch(): m_dataFilter(new Filter()) {}

	/** Clear the list of words. */
	void Clear() { m_words.clear(); }

	/**
	 * @brief Add a word to the list.
	 * @param word Word to add to the list.
	 * @param id Node id to associate with the given word.
	 */
    void Add(const QString& word, uint id) { m_words.insert(std::pair<QString, uint>(word, id)); }

	/**
	 * @brief Get a vector of all words which match the specified search.
	 *				A filter of "" will return the entire list of words.
	 * @param searchStr String to search for.
	 * @param searchType Type of search to perform (i.e., starts with, contains, ends with, matches)
	 */
    std::vector<QString>& FilterData(const QString& searchStr, const bool regularExpression, const bool caseInsensitive)
	{
		m_wordFilter.clear();
		m_dataFilter->Clear();
        QRegularExpression re;
        if(regularExpression)
        {
            re.setPattern(searchStr);
            if(caseInsensitive)
            {
                re.setPatternOptions(QRegularExpression::CaseInsensitiveOption);
            }
        }
		// Add in all text that match the specified search.
        std::map<QString, uint>::iterator it;
		for (it = m_words.begin() ; it != m_words.end(); ++it)
		{
            if(regularExpression)
            {
                if(it->first.contains(re))
                {
                    m_wordFilter.push_back(it->first);
                }
            }
            else
            {
                if(it->first.contains(searchStr, caseInsensitive ? Qt::CaseInsensitive : Qt::CaseSensitive))
                {
                    m_wordFilter.push_back(it->first);
                }
            }
		}

		return m_wordFilter;		
	}

	/**
	 * @brief Get data associated with a given word.
	 * @param word Word to retrieve data for.
	 * @param id Id associated with the given word.
	 * @return True if the word was found, else false.
	 */
    bool Data(const QString& word, uint& id)
	{ 
        std::map<QString, uint>::iterator it;
		
		it = m_words.find(word);
		if(it != m_words.end())
		{
			id = it->second;
			return true;
		}

		return false; 
	}

	/**
	 * @brief Get data for filtered items.
	 * @return Filter containing data only for filtered items.
	 */
	FilterPtr DataFilter() { return m_dataFilter; }

private:
	/** Map words to associated data. */
    std::map<QString, uint> m_words;

	/** List of filtered words. */
    std::vector<QString> m_wordFilter;

	/** Filtered data items. */
	FilterPtr m_dataFilter;
};

}


#endif

