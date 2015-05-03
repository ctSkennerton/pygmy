//=======================================================================
// Copyright 2008, Dalhousie University
// Author: Donovan Parks
//
// The contents of this file are licensed under the 
// Attribution-ShareAlike Creative Commons License:
// http://creativecommons.org/licenses/by-sa/3.0/
//=======================================================================

#include "NewickIO.hpp"
#include <QFileInfo>
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include <QtDebug>

using namespace pygmy;
using namespace utils;
using namespace std;

bool NewickIO::Read(Tree<NodePhylo>::Ptr tree, const QString& filename)
{
	// Set name of tree to the filename
    QFileInfo file(filename);
    tree->SetName(file.baseName());


	// Parse Newick file
    QFile input(filename);
    if (!input.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        //TODO: Put in error dialog
        return false;
    }

    QTextStream instream(&input);
    bool bLoaded = Read(tree, instream);
	input.close();

	if(bLoaded)
		tree->CalculateStatistics();

	return bLoaded;
}

bool NewickIO::Read(Tree<NodePhylo>::Ptr tree, QTextStream & in)
{	
	
	// We concatenate all line in file till we reach the ending semi colon:
    QString temp, newickStr;

	// Read entire input stream
    do {
        temp = in.readLine();
        QString::size_type index = temp.indexOf(";");
        if(index != -1)
        {
            newickStr += temp.mid(0, index + 1);
            break;
        }
        else
        {
            newickStr += temp;
        }
    }
    while (!temp.isNull());

    QString newickStr2 = newickStr.remove(QRegularExpression("\\[.*\\]"));

    return ParseNewickString(tree, newickStr2);
}

void NewickIO::ParseNodeInfo(NodePhylo* node, QString& nodeInfo, bool bLeafNode)
{
    QString length;
    QString name;
    QString supportValue;

	// check if this element has length
    int colon = nodeInfo.lastIndexOf(':');
  if(colon != -1)
  {
        length = nodeInfo.mid(colon + 1).simplified();
        nodeInfo = nodeInfo.mid(0, colon).simplified();
  }

	// check for name and/or support value
    int lastP = nodeInfo.lastIndexOf('\'');
    int firstP = nodeInfo.indexOf('\'');
	if(firstP != -1)
	{
        name = nodeInfo.mid(firstP+1, lastP-firstP-1);
        name = name.replace( '_', ' ');
        supportValue = nodeInfo.mid(lastP+1).simplified();
	}
	else
	{
        int spacePos = nodeInfo.indexOf(' ');
		if(spacePos != -1)
		{
			// parse the name and support value
            name = nodeInfo.mid(0, spacePos-1);
            name = name.replace('_', ' ');
            supportValue = nodeInfo.mid(spacePos+1).simplified();
		}
		else
		{
			// The remaining description is either a name of support value depending 
			// on whether this is a leaf or internal node.
			if(bLeafNode)
			{
                name = nodeInfo.simplified();
                name = name.replace( '_', ' ');
			}
			else
                supportValue = nodeInfo.simplified();
		}
	}	

    if(!name.isEmpty())
		node->SetName(name);

    if(!length.isEmpty())
        node->SetDistanceToParent(length.toDouble());

    if(!supportValue.isEmpty())
        node->SetBootstrapToParent(supportValue.toInt());
}

bool NewickIO::ParseNewickString(Tree<NodePhylo>::Ptr tree, const QString& newickStr)
{
	// create root node
	uint processedElement = 0;
	NodePhylo* root(new NodePhylo(processedElement++));
	tree->SetRootNode(root);
	root->SetDistanceToParent(0.0f);

    int lastP  = newickStr.lastIndexOf(')');
    int firstP = newickStr.indexOf('(');
    int semi = newickStr.lastIndexOf(';');

    QString content = newickStr.mid(firstP + 1, lastP - firstP);
    QString rootElements = newickStr.mid(lastP + 1, semi - lastP - 1);
  
	ParseNodeInfo(root, rootElements, false);

	// parse newick string
	std::stack<NodePhylo*> nodeStack;
	nodeStack.push(root);
    QString nodeInfo;
	NodePhylo* activeNode = NULL;
	for(uint i = 0; i < content.size(); ++i)
	{
        QChar ch = content.at(i);

		if(ch == '(')
		{
			// create a new internal node which will be the child 
			// of the node on the top of the stack
			NodePhylo* node(new NodePhylo(processedElement++));
			nodeStack.top()->AddChild(node);
			nodeStack.push(node);
		}
		else if(ch == ')')
		{
			if(activeNode)
			{
				// if there is a currently active node, then we are
				// processing an internal node
				ParseNodeInfo(activeNode, nodeInfo, false);
			}
			else
			{
				// if there is no currently active node, then we
				// must create a new leaf node
				NodePhylo* node(new NodePhylo(processedElement++));
				nodeStack.top()->AddChild(node);

				ParseNodeInfo(node, nodeInfo, true);				
			}

			// we are finished processing all children of the node
			// on the top of the stack
			activeNode = nodeStack.top();
			nodeStack.pop();

            nodeInfo = "";
		}
		else if(ch == ',')
		{
			if(activeNode)
			{
				// if there is a currently active node, then we are
				// processing an internal node
				ParseNodeInfo(activeNode, nodeInfo, false);
				activeNode = NULL;
                nodeInfo = "";
			}
			else
			{
				// if there is no currently active node, then we
				// must create a new leaf node
				NodePhylo* node(new NodePhylo(processedElement++));
				nodeStack.top()->AddChild(node);

				ParseNodeInfo(node, nodeInfo, true);
                nodeInfo = "";
			}
		}
		else
		{
			// character is indicate the properties of a node
			nodeInfo += ch;
		}
	}

	if(!nodeStack.empty())
	{
		//Log::Inst().Error("Failed to parse Newick string. There does not appear to be an even number of opening and closing parentheses.");
		return false;
	}
	
	return true; 
}

void NewickIO::Write(Tree<NodePhylo>::Ptr tree, QTextStream &out) const
{

	out << "(";

	NodePhylo* root = tree->GetRootNode();
	
  if(tree->GetNumberOfLeaves() == 0)
  {
    out << "'" << root->GetName() << "'";

		float dist = root->GetDistanceToParent();
		if(dist != NodePhylo::NO_DISTANCE)
			out << " " << dist;
  }
  else
  {
		WriteNodes(tree, out, root);
  }
	out << ")";
	
	// Output the name of the root if it has one
    if(!(root->GetName().isEmpty()))
        out << "'" << root->GetName() << "'";
	
	out << ";" << endl;
}

void NewickIO::WriteNodes(Tree<NodePhylo>::Ptr tree, QTextStream& out, NodePhylo* parent) const
{
	vector<unsigned int> childrenId = parent->GetChildrenIds();
	assert(childrenId.size() > 0);

	WriteElements(tree, out, parent, parent->GetChild(0));
	for(uint i = 1; i < childrenId.size(); i++)
	{
		out << ",";
		WriteElements(tree, out, parent, parent->GetChild(i));
	}
}

void NewickIO::WriteElements(Tree<NodePhylo>::Ptr tree, QTextStream& out, NodePhylo* parent, NodePhylo* child) const
{	
  if(child->GetNumberOfChildren() != 0)
  {
    out << "(";
		WriteNodes(tree, out, child); 
    out << ")";
  }

    if(!child->GetName().isEmpty())
        out << "'" << child->GetName() << "'";
	
	if(child->GetBootstrapToParent() != NodePhylo::NO_DISTANCE)
		out << " " << ((NodePhylo*)child)->GetBootstrapToParent();

	if(child->GetDistanceToParent() != NodePhylo::NO_DISTANCE)
		out << ":" << child->GetDistanceToParent();
}



