/////////////////////////////////////////////////////////////////////////////
// Astree.cpp-Package that implements functions for the actions to build   //
//            an Abstract Syntax Tree and dsiplay the tree.                //
// ver 1.0                                                                 //
//Language :    C++, Visual Studio 2015                                    // 
// Application: Code Parser with AST,CSE687 - Object Oriented Design,SP16  //
//Platform:     Dell Inspiron,6th gen Core i5, Windows 10                  //
// Author:      Abhilash Udayashankar, SUID:778388557                      //
//              audayash@syr.edu, (774)-540-1234                           // 
//Original author:Jim Fawcett, CST 4-187, Syracuse University              //
//              (315) 443-3948, jfawcett@twcny.rr.com                      // 
//              jfawcett@twcny.rr.com                                      //
/////////////////////////////////////////////////////////////////////////////
/*
Package Operations:
===================
A package that provides functionality for the actions to build an Abstract Syntax Tree.
Provides an interface for building and for extracting scope information from the tree.

Public Interface:
=================
ScopeStack<element> stack;
// define some element type
element elem;
-showTree()
Function to display the tree
-findComplexity()
Function to find the complexity
-setRootNode()
Function to set the root node

Required Files:
===============
structelement.h
Astree.h

Build Command:
==============
devenv AST.sln /rebuild debug

Maintenance History:
====================
ver 1.0 : 15 Mar 16
- first release
*/
#include "Astree.h"

int complexity = 0;
void AbstractSyntaxTree::showTree(element *pItem)    // display the tree
{
	static size_t indentLevel = 0;
	std::cout << "\n  " << std::string(2 * indentLevel, ' ') << pItem->show();
	auto iter = pItem->_children.begin();
	++indentLevel;
	while (iter != pItem->_children.end())// till iter is not pointing to last element
	{
		showTree(*iter);
		++iter;
	}
	--indentLevel;
}


int AbstractSyntaxTree::DisplayComplex(element* pitem, int cnt)    // display the complexity
{
	complexity = cnt;
	++complexity;
	auto iter = pitem->_children.begin();
	while (iter != pitem->_children.end())
	{
		DisplayComplex(*iter, complexity);
		++iter;
	}
	return complexity;
}


void AbstractSyntaxTree::TreeAnalysis(element * node)  //demonstrate the complexity table
{ 
	if(ccount == 1) {
		std::cout << std::endl << "\n\n\t  complexity Table is displayed below" << std::endl;
		std::cout << "--------------------------------------------------------------------------------" << std::endl;
		std::cout << "\n";
		std::cout << " Type\t\t\t " << "  " << "Name" << " \t\t   " << "No of Lines";
		std::cout << " \t\t" << "cyclometric  Complexity" << std::endl; ccount++;
		std::cout << std::endl << std::endl;
	}
	std::string lineDisplay;
	std::string lineDisplay2;
	//if(node->name.size>)*/
	if (node->type == "function") {//
		lineDisplay = "\t\t";
		if (node->name.size()>7 && node->name.size() <= 12)lineDisplay2 = "\t\t\t";
		else if (node->name.size() >= 13)lineDisplay2 = "\t\t";
		else lineDisplay2 = "\t\t\t\t";
		std::cout << node->type << lineDisplay << node->name << lineDisplay2 << (node->endlinecount - node->startlinecount + 1);
		std::cout << " \t\t\t " << FindComplexity(node) << std::endl;
		auto iter = node->_children.begin();
		while (iter != node->_children.end())
		{
			TreeAnalysis(*iter);
			++iter;
		}
	}
	else {
		auto iter = node->_children.begin();
		while (iter != node->_children.end())
		{
			TreeAnalysis(*iter);
			++iter;
		}
	}

}

int AbstractSyntaxTree::FindComplexity(element* node)    // find the complexity
{
	return DisplayComplex(node, 0);
}
