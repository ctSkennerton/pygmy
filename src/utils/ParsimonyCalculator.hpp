//=======================================================================
// Copyright 2008, Dalhousie University
// Author: Donovan Parks
//
// The contents of this file are licensed under the 
// Attribution-ShareAlike Creative Commons License:
// http://creativecommons.org/licenses/by-sa/3.0/
//=======================================================================

#ifndef _PARSIMONY_CALCULATOR_
#define _PARSIMONY_CALCULATOR_


#include "../utils/Tree.hpp"
#include "../core/NodePhylo.hpp"

typedef struct sPARSIMONY_DATA
{
	sPARSIMONY_DATA(): nodeScore(0) {}

        uint GetScore(const QString& character) { return characterScores[character]; }
	uint GetNodeScore() { return nodeScore; }

        std::map<QString, uint> characterScores;
	uint nodeScore;
        std::set<QString> parsimoniousCharacters;
} ParsimonyData;

namespace utils
{

/**
 * @brief Calculates parsimony score (i.e., minimum number of changes required to explain the data)
 *				and assignment of internal labels leading to this parsimony score using the Sankoff algorithm.
 */
class ParsimonyCalculator
{
public:
	/** Constructor. */
	ParsimonyCalculator() {}

	/** Destructor. */
	~ParsimonyCalculator() {}

	/** Calculate parismony score and assignment of labels giving this score. */
        uint Calculate(Tree<pygmy::NodePhylo>::Ptr tree, const QString& field, const std::set< QString >& characters);

	/** Get parsimony data for a given node. */
	void GetData(pygmy::NodePhylo* node, ParsimonyData& parsimonyData);

protected:
	/** Propagate parsimony scores up tree.*/
        void CalculateUp(Tree<pygmy::NodePhylo>::Ptr tree, const QString& field, const std::set< QString >& characters);

	/** Propagate most parsimonious character states down tree. */
        void CalculateDown(Tree<pygmy::NodePhylo>::Ptr tree, const QString& field, const std::set< QString >& characters);

protected:
	/** Parsimony data for each node (indexed by node id). */
	std::map<uint, ParsimonyData> m_parsimonyData;
	
};

}

#endif
