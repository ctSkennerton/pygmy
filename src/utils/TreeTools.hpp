//=======================================================================
// Copyright 2008, Dalhousie University
// Author: Donovan Parks
//
// Based on code by: Julien Dutheil and Celine Scornavacca (Bio++ project)
//
// The contents of this file are licensed under the 
// Attribution-ShareAlike Creative Commons License:
// http://creativecommons.org/licenses/by-sa/3.0/
//=======================================================================

#ifndef _TREE_TOOLS_
#define _TREE_TOOLS_
#include "Error.hpp"
#include "Node.hpp"
#include <vector>
#include <QString>
namespace utils
{

/**
 * @brief Methods for obtaining information from a tree. 
 *
 * @see TreeTools for more generic methods.
 */
template<class N> class TreeTools
{
public:
  TreeTools() {}
  virtual ~TreeTools() {}

public:

	/**
	 * @brief Clone subtree starting at the specified node.
	 * @param node Root node of subtree to clone.
	 * @return Cloned tree.
	 */
	static N* CloneSubtree(const N* node); 

	/**
	 * @brief Retrieve all leaves from a subtree.
	 * @param node Node that defines the subtree.
	 * @return vector of leaves in the subtree.
	 */
	static std::vector<N*> GetLeaves(N* node);

	/**
	 * @brief Retrieve all leaves from a subtree.
	 * @param node Node that defines the subtree.
	 * @param leaves A vector of leaves in the subtree.
	 */
	static void GetLeaves(N* node, std::vector<N*>& leaves);

	/**
	 * @brief Retrieve all leaf ids from a subtree.
	 * @param node Node that defines the subtree.
	 * @return A vector of ids.
	 */
	static std::vector<unsigned int> GetLeafIds(const N* node);

	/**
	 * @brief Get the id of a leaf given its name in a subtree.
	 *
	 * @param node The node defining the subtree to search.
	 * @param name The name of the node.
	 * @param id The id of the node.
	 */
    static void SearchLeaf(const N* node,  const QString & name, unsigned int * & id);

	/**
	 * @brief Retrieve all son nodes from a subtree.
	 * @param node Node that defines the subtree.
	 * @return Vector of nodes in the subtree.
	 */
	static std::vector<N*> GetNodes(N* node);

	/**
	 * @brief Retrieve all nodes ids from a subtree.
	 * @param node Bode that defines the subtree.
	 * @return Vector of ids.
	 */
	static std::vector<int> GetNodesId(const N* node);

	/**
	 * @brief Determine if subtree contains a node with a given id.
	 * @param node Node defining the subtree to be searched.
	 * @param name Name to search for.
	 * @return True if the subtree contains a node with the specified id.
	 */
	static bool HasNodeWithId(const N* node, unsigned int id);

	/**
	 * @brief Find all nodes in a subtree with a given id.
	 * @param node Node defining the subtree to be searched.
	 * @param id  Id to search for.
	 * @return Vector of nodes with the specified id.
	 */
	static std::vector<N*> SearchNodeWithId(const N* node, unsigned int id);

	/**
	 * @brief Find all nodes in a subtree with a given name.
	 * @param node  Node defining the subtree to be searched.
	 * @param name  The name to search for.
	 * @param nodes Vector of nodes with the specified name.
	 */
    static std::vector<N*> SearchNodeWithName(N* node, const QString & name);

	/**
	 * @brief Determine if subtree contains a node with a given name.
	 * @param node Node defining the subtree to be searched.
	 * @param name Name to search for.
	 * @return True if the subtree contains a node with the specified name.
	 */
    static bool HasNodeWithName(const N* node, const QString & name);

	/**
	 * @brief Tell if a particular node is the root of a tree
	 * @param node Node to check.
	 * @return True if node is the root.
	 */
	static bool IsRoot(const N* node) { return node->IsRoot(); }

	/**
	 * @brief Get the number of leaves of a subtree defined by a particular node.
	 * @param node Node defining the subtree to check.
	 * @return Number of leaves.
	 */
	static unsigned int GetNumberOfLeaves(const N* node);

	/**
	 * @brief Get the number of nodes of a subtree defined by a particular node.
	 * @param node Node defining the subtree to check.
	 * @return Number of leaves.
	 */
	static unsigned int GetNumberOfNodes(const N* node);

	/**
	 * @brief Get the leaf names of a subtree defined by a particular node.
	 * @param node Node defining the subtree to check.
	 * @return Vector of all leaves names.
	 */
    static std::vector<QString> GetLeafNames(const N* node);

	/**
	 * @brief Get the depth of the subtree (i.e. the maximum number of children 'generations').
	 *
	 * ex:
	 * <code>
	 *    +----------A
	 *    |
	 * ---+ N1     +-------B
	 *    |        |
	 *    +--------+ N2
	 *             |
	 *             +------C
	 * </code>
	 * Depth of node 'N1' is 2, depth of node 'N2' is 1, depth of leaves is 0.
	 *
	 * @param node Node defining the subtree to check.
	 * @return Depth of the subtree.
	 */
	static unsigned int GetDepth(const N* node);

	/** Get distance to furthest leaf node. */
	static float GetDistToFurthestLeafNode(const N* node);

	/** Get distance to root node. */
	static float GetDistToRoot(const N* node);

	/**
	 * @brief Tell is a subtree is multifurcating.
	 * @param node Root node of the subtree.
	 * @return True is the subtree contains at least one multifurcating node.
	 */
	static bool IsMultifurcating(const N* node);

	/**
	 * @brief Get the path between two nodes.
	 * @param node1 First node.
	 * @param node2 Second node.
	 * @param includeAncestor Flag indicating if common ancestor should be included.
	 * @return A vector with all branch lengths.
	 */
	static std::vector<N*> GetPathBetweenAnyTwoNodes(const N* node1, const N* node2, bool includeAncestor = true);

	/**
	 * @brief Get all the branch lengths of a subtree.
	 * @param node Root node of the subtree.
	 * @return A vector with all branch lengths.
	 */
	static std::vector<float> GetBranchLengths(const N* node);

	/**
	 * @brief Get the total length (sum of all branch lengths) of a subtree.
	 * @param node Root node of the subtree.
	 * @return Total length of the subtree.
	 */
	static float GetTotalLength(const N* node);

	/**
	 * @brief Set all the branch lengths of a subtree.
	 * @param node Root node of the subtree.
	 * @param brLen Branch length to apply.
	 */
	static void SetBranchLengths(const N* node, float brLen);
	        
	/**
	 * @brief Multiply all branch lengths by a given factor.
	 * @param node Root node of the subtree to scale.
	 * @param factor Factor to multiply all branch lengths with.
	 */
	static void ScaleTree(const N* node, float factor);

	/**
	 * @brief Get the total distance between two nodes.
	 * @param node1 First node.
	 * @param node2 Second node.
	 * @return Sum of all branch distances between the two nodes.
	 */
	static float GetDistanceBetweenAnyTwoNodes(const N* node1, const N* node2);

private:
	/**
	 * @brief Retrieve all leaf ids from a subtree.
	 * @param node Node that defines the subtree.
	 * @param ids Vector of ids.
	 */
	static void GetLeafIds(const N* node, std::vector<unsigned int> & ids);

	/**
	 * @brief Retrieve all son nodes from a subtree.
	 * @param node Node that defines the subtree.
	 * @param nodes Vector of pointers toward each son node in the subtree.
	 */
	static void GetNodes(N* node, std::vector<N*>& nodes);

	/**
	 * @brief Retrieve all node ids from a subtree.
	 * @param node The node that defines the subtree.
	 * @param ids Vector of ids.
	 */
	static void GetNodesId(const N* node, std::vector<unsigned int> & ids);

	/**
	 * @brief Find all nodes in a subtree with a given id.
	 * @param node Node defining the subtree to be searched.
	 * @param id  Id to search for.
	 * @param nodes Vector of nodes with the specified id.
	 */
	static void SearchNodeWithId(const N* node, unsigned int id, std::vector<N*> & nodes);

	/**
	 * @brief Find all nodes in a subtree with a given name.
	 * @param node  Node defining the subtree to be searched.
	 * @param name  The name to search for.
	 * @param nodes Vector of nodes with the specified name.
	 */
    static void SearchNodeWithName(N* node, const QString& name, std::vector<N*>& nodes);
};


// --- Function implementations -----------------------------------------------

template<class N>
N* TreeTools<N>::CloneSubtree(const N* node) 
{
  N* clone = new N(*node);

	clone->RemoveChildren();
  for(unsigned int i = 0; i < node->GetNumberOfChildren(); i++)
  {
		clone->AddChild(TreeTools<N>::CloneSubtree(node->GetChild(i)));
  }

  return clone;
}

template <class N>
bool TreeTools<N>::IsMultifurcating(const N* node)
{
  if(node->GetNumberOfChildren() > 2) 
	{
		return true;
	}
	else 
	{
    bool b = false;
    for(unsigned int i = 0; i < node->GetNumberOfChildren(); i++)
    {
      b = b || IsMultifurcating(node->GetChild(i));
    }
    return b;
  }    
}

template <class N>
unsigned int TreeTools<N>::GetNumberOfLeaves(const N* node)
{
  unsigned int nbLeaves = 0;
  if(node->IsLeaf())
  {
    nbLeaves++;
  } 
  for(unsigned int i = 0; i < node->GetNumberOfChildren(); i++)
  {
    nbLeaves += GetNumberOfLeaves(node->GetChild(i));
  }
  return nbLeaves;
}

template <class N>
unsigned int TreeTools<N>::GetNumberOfNodes(const N* node)
{
  unsigned int nbNodes = 1;
  for(unsigned int i = 0; i < node->GetNumberOfChildren(); i++)
  {
    nbNodes += GetNumberOfNodes(node->GetChild(i));
  }
  return nbNodes;
}

template <class N>
std::vector<QString> TreeTools<N>::GetLeafNames(const N* node)
{
    std::vector<QString> names;

  if(node->IsLeaf())
  {
    names.push_back(node->GetName());
  }
  for(unsigned int i = 0; i < node->GetNumberOfChildren(); i++)
  {
        std::vector<QString> subNames = GetLeafNames(node->GetChild(i));
        for(QString& subName : subNames)
		{
			names.push_back(subName);
		}
  }

  return names;   
}

template <class N>
unsigned int TreeTools<N>::GetDepth(const N* node)
{
  unsigned int d = 0;
  for(unsigned int i = 0; i < node->GetNumberOfChildren(); i++)
  {
    unsigned int c = GetDepth(node->GetChild(i)) + 1;
    if( c > d) d = c;
  }
  return d;
}

template <class N>
float TreeTools<N>::GetDistToFurthestLeafNode(const N* node)
{
  float d = 0;
  for(unsigned int i = 0; i < node->GetNumberOfChildren(); i++)
  {
    N* child = node->GetChild(i);
    float dist = 0;
		if(child->GetDistanceToParent() != Node::NO_DISTANCE) 
			dist = child->GetDistanceToParent();
    else 
		{
			// node without branch length
			assert(false);
		}

    float c = GetDistToFurthestLeafNode(child) + dist;
    if(c > d) 
			d = c;
  }

  return d;	
}

template <class N>
float TreeTools<N>::GetDistToRoot(const N* node)
{
	float dist = 0.0f;
	N* parent = node;
	while(!parent->IsRoot())
	{
		dist += parent->GetDistanceToParent();
		parent = parent->GetParent();
	}

  return dist;	
}

template <class N>
std::vector<float> TreeTools<N>::GetBranchLengths(const N* node)
{
  std::vector<float> brLen(1);

  if(node->GetDistanceToParent() != Node::NO_DISTANCE) 
	{
		brLen[0] = node->GetDistanceToParent();
	}
  else 
	{
		// no branch length
		assert(false);
	}

  for(unsigned int i = 0; i < node->GetNumberOfChildren(); i++)
  {
    std::vector<float> sonBrLen = GetBranchLengths(node->GetChild(i));
    for(unsigned int j = 0; j < sonBrLen.size(); j++) 
			brLen.push_back(sonBrLen[j]);
  }

  return brLen;
}

template <class N>
float TreeTools<N>::GetTotalLength(const N* node)
{
	if(node->GetDistanceToParent() != Node::NO_DISTANCE)
	{
		float length = node->GetDistanceToParent();
		for(unsigned int i = 0; i < node->GetNumberOfChildren(); i++)
		{
			length += GetTotalLength(node->GetChild(i));
		}
		return length;
	}

	return Node::NO_DISTANCE;
}

template <class N>
void TreeTools<N>::SetBranchLengths(const N* node, float brLen)
{
	node->SetDistanceToParent(brLen);
  for(unsigned int i = 0; i < node->GetNumberOfChildren(); i++)
  {
    SetBranchLengths(node->GetChild(i), brLen);
  }
}

template <class N>
void TreeTools<N>::ScaleTree(const N* node, float factor)
{
  if(!node->IsRoot())
  {
		if(node->GetDistanceToParent() != Node::NO_DISTANCE)
			node->SetDistanceToParent(node->GetDistanceToParent() * factor);
  }

  for(unsigned int i = 0; i < node->GetNumberOfChildren(); i++)
  {
    ScaleTree(node->GetChild(i), factor);
  }
}

template <class N>
std::vector<N*> TreeTools<N>::GetPathBetweenAnyTwoNodes(const N* node1, const N* node2, bool includeAncestor)
{
  std::vector<N*> path;
  std::vector<N*> pathMatrix1;
  std::vector<N*> pathMatrix2;

  N* nodeUp = node1;
  while(!nodeUp->IsRoot())  
	{ 
    pathMatrix1.push_back(nodeUp);
		nodeUp = nodeUp->GetParent();
  }
  pathMatrix1.push_back(nodeUp); // The root.

  nodeUp = node2;
  while(!nodeUp->IsRoot())  
	{
    pathMatrix2.push_back(nodeUp);
    nodeUp = nodeUp->GetParent();
  }
  pathMatrix2.push_back(nodeUp); // The root.
  // Must check that the two nodes have the same root!!!

  int tmp1 = pathMatrix1.size() - 1;
  int tmp2 = pathMatrix2.size() - 1;

  while((tmp1 >= 0) && (tmp2 >= 0)) 
	{
    if (pathMatrix1[tmp1] != pathMatrix2[tmp2]) break;
    tmp1--; tmp2--;
  }

  for (int y = 0; y <= tmp1; ++y) 
		path.push_back(pathMatrix1[y]);

  if(includeAncestor) 
		path.push_back(pathMatrix1[tmp1 + 1]); // pushing once, the Node that was common to both.

  for (int j = tmp2; j >= 0; --j) 
	{
    path.push_back(pathMatrix2[j]);
  }
  return path;
}

template <class N>
float TreeTools<N>::GetDistanceBetweenAnyTwoNodes(const N* node1, const N* node2)
{
  std::vector<N*> path = GetPathBetweenAnyTwoNodes(node1, node2, false);
  float d = 0;
    for(N* n: path)
  {
		d += n->GetDistanceToParent();
  }
  return d;
}

template <class N>
std::vector<N*> TreeTools<N>::GetLeaves(N* node)
{
	std::vector<N*> leaves;
	GetLeaves(node, leaves);
	return leaves;
}

template <class N>
void TreeTools<N>::GetLeaves(N* node, std::vector<N*>& leaves)
{
	if(node->IsLeaf())
	{
		node->SetLeafOrderIndex(leaves.size());
		leaves.push_back(node);
	}
	for(unsigned int i = 0; i < node->GetNumberOfChildren(); i++)
	{
		GetLeaves(node->GetChild(i), leaves);
	}
}

template <class N>
std::vector<unsigned int> TreeTools<N>::GetLeafIds(const N* node)
{
	std::vector<unsigned int> ids;
	GetLeafIds(node, ids);
	return ids;
}

template <class N>
void TreeTools<N>::GetLeafIds(const N* node, std::vector<unsigned int> & ids)
{
	if(node->IsLeaf()) {
		ids.push_back(node->GetId());
	}
	for(unsigned int i = 0; i < node->GetNumberOfChildren(); i++) {
		GetLeafIds(node->GetChild(i), ids);
	}
}

template <class N>
void TreeTools<N>::SearchLeaf(const N* node,  const QString & name, unsigned int * & id)
{
	if(node->IsLeaf())
	{
		if(node->GetName() == name)
		{
			id = new unsigned int(node->GetId());
			return;
		}
	}

	for(unsigned int i = 0; i < node->GetNumberOfChildren(); i++)
	{
		SearchLeaf(node->GetChild(i), name, id);
	}
}

template <class N>
std::vector<N*> TreeTools<N>::GetNodes(N* node)
{
	std::vector<N*> nodes;
	GetNodes(node, nodes);
	return nodes;
}

template <class N>
void TreeTools<N>::GetNodes(N* node, std::vector<N*>& nodes)
{
	for(unsigned int i = 0; i < node->GetNumberOfChildren(); i++)
	{
		GetNodes(node->GetChild(i), nodes);
	}

	nodes.push_back(node);
}

template <class N>
std::vector<int> TreeTools<N>::GetNodesId(const N* node)
{
	std::vector<int> ids;
	GetNodesId(node, ids);
	return ids;
}

template <class N>
void TreeTools<N>::GetNodesId(const N* node, std::vector<unsigned int> & ids)
{
	for(unsigned int i = 0; i < node->GetNumberOfChildren(); i++)
	{
		GetNodesId(node->GetChild(i), ids);
	}
	ids.push_back(node->GetId());
}

template <class N>
bool TreeTools<N>::HasNodeWithId(const N* node, unsigned int id)
{
	if(node->GetId() == id) return true;
	else
	{
		for(unsigned int i = 0; i < node->GetNumberOfChildren(); i++)
		{
			if(HasNodeWithId(node->GetChild(i), id)) return true;
		}
		return false;
	}
}

template <class N>
std::vector<N*> TreeTools<N>::SearchNodeWithId(const N* node, unsigned int id)
{
	std::vector<N*> nodes;
	SearchNodeWithId(node, id, nodes);
	return nodes;    
}

template <class N>
void TreeTools<N>::SearchNodeWithId(const N* node, unsigned int id, std::vector<N*> & nodes)
{
	for(unsigned int i = 0; i < node->GetNumberOfChildren(); i++)
	{
		SearchNodeWithId(node->GetChild(i), id, nodes);
	}

	if(node->GetId() == id) 
		nodes.push_back(node);
}

template <class N>
std::vector<N*> TreeTools<N>::SearchNodeWithName(N* node, const QString & name)
{
	std::vector<N*> nodes;
	SearchNodeWithName(node, name, nodes);
	return nodes;    
}

template <class N>
void TreeTools<N>::SearchNodeWithName(N* node, const QString& name, std::vector<N*>& nodes)
{
	for(unsigned int i = 0; i < node->GetNumberOfChildren(); i++)
	{
		SearchNodeWithName(node->GetChild(i), name, nodes);
	}

	if(node->GetName() == name) 
		nodes.push_back(node);
}

template <class N>
bool TreeTools<N>::HasNodeWithName(const N* node, const QString & name)
{
	if(node->GetName() == name) 
	{
		return true;
	}
	else
	{
		for(unsigned int i = 0; i < node->GetNumberOfChildren(); i++)
		{
			if(HasNodeWithName(node->GetChild(i), name)) 
				return true;
		}
		return false;
	}
}


} 

#endif 

