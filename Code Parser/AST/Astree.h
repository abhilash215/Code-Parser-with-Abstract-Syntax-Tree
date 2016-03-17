#ifndef AST_H
#define AST_H
/////////////////////////////////////////////////////////////////////////////
// Astree.h-Package that provides functions for the actions to build       //
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

Build Command:
==============
devenv AST.sln /rebuild debug

Maintenance History:
====================
ver 1.0 : 15 Mar 16
- first release

*/



#include <list>
#include "../Structelement/structelement.h"


class AbstractSyntaxTree { //template								
public:
	void showTree(element*);
	element* rootNode;
	int FindComplexity(element*);   // finding complexity
	int DisplayComplex(element*, int);  // display the complexity
	void TreeAnalysis(element*);       // tree 
	element* getRootNode() { return rootNode; }
	void setNode(element* node) { rootNode = node; }

private:
	//static int complexity;
	int ccount=1;                    //complexity count
	std::list<element*> stack;   // stack is a list of struct element
};


#endif