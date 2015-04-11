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

#include "../core/Precompiled.hpp"

#include "Node.hpp"

#include "../utils/StringTools.hpp"

using namespace utils;
using namespace std;

Node::Node(const Node& node):
  m_id(node.GetId()), m_name(node.GetName()), m_parent(node.GetParent()),
		m_distanceToParent(node.GetDistanceToParent()), 
		m_depth(node.GetDepth()), m_height(node.GetHeight()), m_distanceToRoot(node.GetDistanceToRoot())
{
	m_children = node.GetChildren();
}

Node& Node::operator=(const Node& node)
{
  m_id = node.GetId();
  m_name = node.GetName();
  m_parent = node.GetParent();
	m_distanceToParent = node.GetDistanceToParent();
	m_children = node.GetChildren();

	m_depth = node.GetDepth();
	m_height = node.GetHeight();
	m_distanceToRoot = node.GetDistanceToRoot();

  return * this;
}

     
std::vector<unsigned int> Node::GetChildrenIds() const
{
	std::vector<unsigned int> childrenId(m_children.size());
	for(unsigned int i = 0; i < m_children.size(); i++) 
	{
		childrenId.at(i) = m_children.at(i)->GetId();
	}

	return childrenId;
}

Node* Node::GetChild(unsigned int pos) const
{
	assert(pos < m_children.size());
	return m_children.at(pos);
} 

void Node::AddChild(unsigned int pos, Node* node)
{
	m_children.insert(m_children.begin() + pos, node);
	node->SetParent(this);
}

void Node::AddChild(Node* node)
{
	m_children.push_back(node);
	node->SetParent(this);
}

void Node::RemoveChild(unsigned int pos)
{
	assert(pos < m_children.size());
	m_children.erase(m_children.begin() + pos);
}

void Node::RemoveChildren() 
{  
	while(m_children.size() != 0) 
		m_children.erase(m_children.begin()); 
}

void Node::RemoveChild(Node* node)
{
	for(unsigned int i = 0; i < m_children.size(); i++)
	{
		if(m_children.at(i) == node)
		{
			m_children.erase(m_children.begin() + i);
			return;
		}
	}
}
void Node::Swap(unsigned int pos1, unsigned int pos2)
{
  Node* node1 = GetChild(pos1);
  Node* node2 = GetChild(pos2);

	// remove higher position item first!
	if(pos1 < pos2)
	{
		RemoveChild(pos2);
		RemoveChild(pos1);
	}
	else
	{
		RemoveChild(pos1);
		RemoveChild(pos2);
	}

  AddChild(pos1, node2);
  AddChild(pos2, node1);
}

    
vector<Node*> Node::GetNeighbors() const
{
	vector<Node*> neighbors;

	Node* parent = GetParent();

  for(unsigned int i = 0; i < parent->GetNumberOfChildren(); i++) 
	{
		if(parent->GetChild(i) != this)
			neighbors.push_back(parent->GetChild(i));
	}

  return neighbors;
}

unsigned int Node::GetChildPosition(Node* child) const
{
  for(unsigned int i = 0; i < m_children.size(); i++) 
	{
    if(m_children.at(i) == child) 
			return i;
  }

	assert(false);

	return m_children.size();	// invalid position
}
