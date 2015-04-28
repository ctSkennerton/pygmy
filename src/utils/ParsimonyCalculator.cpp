//=======================================================================
// Copyright 2008, Dalhousie University
// Author: Donovan Parks
//
// Based on code by: Julien Dutheil (Bio++ project)
//
// The contents of this file are licensed under the 
// Attribution-ShareAlike Creative Commons License:
// http://creativecommons.org/licenses/by-sa/3.0/
//=======================================================================


#include "ParsimonyCalculator.hpp"

using namespace utils;
using namespace pygmy;

uint ParsimonyCalculator::Calculate(Tree<pygmy::NodePhylo>::Ptr tree, const QString& field, const std::set< QString >& characters)
{
	// initialize leaf nodes
	std::vector<NodePhylo*> leaves = tree->GetLeaves();
    for(NodePhylo* leaf : leaves)
	{
		ParsimonyData parsimonyData;
		parsimonyData.nodeScore = 0;

        for(const QString& character : characters)
		{
			parsimonyData.characterScores[character] = INT_MAX;
		}

		parsimonyData.characterScores[leaf->GetData(field)] = 0;

		m_parsimonyData[leaf->GetId()] = parsimonyData;
	}

	// Propagate parsimony scores up tree
	CalculateUp(tree, field, characters);

	// Propagate most parsimonious character states down tree.
	CalculateDown(tree, field, characters);

	return m_parsimonyData[tree->GetRootNode()->GetId()].GetNodeScore();
}

void ParsimonyCalculator::CalculateUp(Tree<pygmy::NodePhylo>::Ptr tree, const QString& field, const std::set< QString >& characters)
{
	// set flag indicating if a node has been processed
	std::vector<NodePhylo*> nodes = tree->GetNodes();
    for(NodePhylo* node : nodes)
	{
		node->SetProcessed(node->IsLeaf());
	}

	// calculate values for internal nodes in breadth-first order starting from the leaf nodes
	std::vector<NodePhylo*> leaves = tree->GetLeaves();
	std::set<NodePhylo*> curNodes;
    for(NodePhylo* leaf : leaves)
	{
		curNodes.insert(leaf->GetParent());
	}

	std::set<NodePhylo*> nextNodes;
	while(!curNodes.empty())
	{
		nextNodes.clear();
        for(NodePhylo* curNode : curNodes)
		{
			// check if all children have been processed
			bool bAllProcessed = true;
			std::vector<NodePhylo*> children = curNode->GetChildren();
            for(NodePhylo* child : children)
			{
				if(!child->IsProcessed())
				{
					nextNodes.insert(curNode);
					bAllProcessed = false;
					break;
				}
			}

			if(!bAllProcessed)
				continue;

			// calculate parsimony data for internal node
			ParsimonyData parsimonyData;
			parsimonyData.nodeScore = INT_MAX;
            for(const QString& character : characters)
			{
				parsimonyData.characterScores[character] = 0;
                for(NodePhylo* child : children)
				{
					uint nodeScore = m_parsimonyData[child->GetId()].GetNodeScore() + 1;
					uint characterScore = m_parsimonyData[child->GetId()].GetScore(character);

					if(nodeScore < characterScore)
						parsimonyData.characterScores[character] += nodeScore;
					else
						parsimonyData.characterScores[character] += characterScore;
				}

				if(parsimonyData.characterScores[character] < parsimonyData.nodeScore)
					parsimonyData.nodeScore = parsimonyData.characterScores[character];
			}

			m_parsimonyData[curNode->GetId()] = parsimonyData;

			curNode->SetProcessed(true);

			if(!curNode->IsRoot())
				nextNodes.insert(curNode->GetParent());
		}

		curNodes = nextNodes;
	}
}

void ParsimonyCalculator::CalculateDown(Tree<pygmy::NodePhylo>::Ptr tree, const QString& field, const std::set< QString >& characters)
{
	// assign parsimonious characters to root node
	ParsimonyData& data = m_parsimonyData[tree->GetRootNode()->GetId()];
    for(const QString& character: characters)
	{
		if(data.GetScore(character) == data.GetNodeScore())
			data.parsimoniousCharacters.insert(character);
	}

	// assign parsimonious characters to internal nodes in a breadth-first manner
	std::set<NodePhylo*> curNodes;
	std::vector<NodePhylo*> children = tree->GetRootNode()->GetChildren();
    for(NodePhylo* child : children)
	{
		curNodes.insert(child);
	}

	std::set<NodePhylo*> nextNodes;
	while(!curNodes.empty())
	{
		nextNodes.clear();
        for(NodePhylo* node : curNodes)
		{
			// assign parsimonious characters to node
			ParsimonyData& dataParent = m_parsimonyData[node->GetParent()->GetId()];
			ParsimonyData& dataNode = m_parsimonyData[node->GetId()];
            for(const QString& character : dataParent.parsimoniousCharacters)
			{
				if(dataNode.GetNodeScore() + 1 >= dataNode.GetScore(character))
				{
					dataNode.parsimoniousCharacters.insert(character);
				}
			}

            for(const QString& character : characters)
			{	
				if(dataNode.GetNodeScore() == dataNode.GetScore(character))
				{
					dataNode.parsimoniousCharacters.insert(character);
				}
			}
			
			// process children
			std::vector<NodePhylo*> children = node->GetChildren();
            for(NodePhylo* child : children)
			{
				nextNodes.insert(child);
			}
		}

		curNodes = nextNodes;
	}

}

void ParsimonyCalculator::GetData(pygmy::NodePhylo* node, ParsimonyData& parsimonyData)
{
	parsimonyData = m_parsimonyData[node->GetId()];
}
