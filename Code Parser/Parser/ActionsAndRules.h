#ifndef ACTIONSANDRULES_H
#define ACTIONSANDRULES_H
/////////////////////////////////////////////////////////////////////////////
//  ActionsAndRules.h - declares new parsing rules and actions             //
//  ver 2.2                                                                //
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
Module Operations:
==================
This module defines several action classes.  Its classes provide
specialized services needed for specific applications.  The modules
Parser, SemiExpression, and Tokenizer, are intended to be reusable
without change.  This module provides a place to put extensions of
these facilities and is not expected to be reusable.

Public Interface:
=================
Toker t(someFile);              // create tokenizer instance
SemiExp se(&t);                 // create a SemiExp attached to tokenizer
Parser parser(se);              // now we have a parser
Rule1 r1;                       // create instance of a derived Rule class
Action1 a1;                     // create a derived action
r1.addAction(&a1);              // register action with the rule
parser.addRule(&r1);            // register rule with parser
while(se.getSemiExp())          // get semi-expression
parser.parse();                 // and parse it

Build Process:
==============
Required files
- Parser.h, Parser.cpp, ScopeStack.h, ScopeStack.cpp,
ActionsAndRules.h, ActionsAndRules.cpp, ConfigureParser.cpp,
ItokCollection.h, SemiExpression.h, SemiExpression.cpp, tokenizer.h, tokenizer.cpp
Build commands (either one)
- devenv CodeAnalysis.sln
- cl /EHsc /DTEST_PARSER parser.cpp ActionsAndRules.cpp \
semiexpression.cpp tokenizer.cpp /link setargv.obj

Required Files:
===============
Parser.h
structelement.h
itokcollection.h
ScopeStack.h
Tokenizer.h
SemiExp.h
Astree.h

Maintenance History:
====================
ver 2.2 : 15 Mar 16
- new rules and actions are added
- comments are added
ver 2.1 : 15 Feb 16
- small functional change to a few of the actions changes display strategy
- preface the (new) Toker and SemiExp with Scanner namespace
ver 2.0 : 01 Jun 11
- added processing on way to building strong code analyzer
ver 1.1 : 17 Jan 09
- changed to accept a pointer to interfaced ITokCollection instead
of a SemiExpression
ver 1.0 : 12 Jan 06
- first release

*/
//
#include <queue>
#include <string>
#include <sstream>
#include "Parser.h"
#include  "../Structelement/structelement.h"
#include "../SemiExp/itokcollection.h"
#include "../ScopeStack/ScopeStack.h"
#include "../Tokenizer/Tokenizer.h"
#include "../SemiExp/SemiExp.h"
#include "../AST/Astree.h"

///////////////////////////////////////////////////////////////
// ScopeStack element is application specific
/* ToDo:
* - chanage lineCount to two fields: lineCountStart and lineCountEnd
*/

//struct element
//{
//	std::string type;
//	std::string name;
//	size_t startlinecount;
//	size_t endlinecount;
//	std::string show()
//	{
//		std::ostringstream temp;
//		temp << "(";
//		temp << type;
//		temp << ", ";
//		temp << name;
//		temp << ", ";
//		temp << startlinecount;
//		temp << ",";
//		temp << endlinecount;
//		temp << ")";
//		return temp.str();
//	}
//};

///////////////////////////////////////////////////////////////
// Repository instance is used to share resources
// among all actions.

class Repository  // application specific
{
	ScopeStack<element*> stack;
	Scanner::Toker* p_Toker;
public:
	//ASTree<element*>* abstree = new ASTree<element*>();
	element* root = new element;

	AbstractSyntaxTree* absTree = new AbstractSyntaxTree;
	void configRoot()
	{
		root->name = "GLOBAL NAMESPACE";
		root->type = "NAMESPACE";
		root->startlinecount = 0;
		root->endlinecount = 1;
		stack.push(root);
		absTree->setNode(root);
	}
	Repository(Scanner::Toker* pToker)
	{
		p_Toker = pToker;
	}
	void printAsTree() {                 
		absTree->showTree(root);
	}
	ScopeStack<element*>& scopeStack()
	{
		return stack;
	}
	Scanner::Toker* Toker()
	{
		return p_Toker;
	}
	size_t startlinecount()
	{
		return (size_t)(p_Toker->currentLineCount());
	}
	size_t endlinecount()
	{
		return (size_t)(p_Toker->currentLineCount());
	}
};

///////////////////////////////////////////////////////////////
// rule to detect beginning of anonymous scope

class BeginningOfScope : public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		if (pTc->find("{") < pTc->length())
		{
			doActions(pTc);
			return true;
		}
		return true;
	}
};
///////////////////////////////////////////////////////////////
// action to handle scope stack at end of scope

class HandlePush : public IAction
{
	Repository* p_Repos;
public:
	HandlePush(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
	//	std::cout << "\n--BeginningOfScope rule";
		element* elem = new element;;
		elem->type = "unknown";
		elem->name = "anonymous";
	//	elem->startlinecount = p_Repos->startlinecount();
		p_Repos->scopeStack().top()->_children.push_back(elem);
		p_Repos->scopeStack().push(elem);
	}
};

///////////////////////////////////////////////////////////////
// rule to detect end of scope

class EndOfScope : public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		if (pTc->find("}") < pTc->length())
		{
			doActions(pTc);
			return true;
		}
		return true;
	}
};
///////////////////////////////////////////////////////////////
// action to handle scope stack at end of scope

class HandlePop : public IAction
{
	Repository* p_Repos;
public:
	HandlePop(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		//std::cout << "\n--EndOfScope rule";
		if (p_Repos->scopeStack().size() == 0)
			return;
		element* elem = p_Repos->scopeStack().pop();
		elem->endlinecount = p_Repos->endlinecount();
		if (elem->type == "function")
		{
			//     std::cout << "\nHandlePop \n ";
			//std::cout << "\n--popping at line count = " << p_Repos->endlinecount() - 1 << std::endl;
			//std::cout << "\nstart line " << elem->startlinecount - 1 << std::endl;
			//std::cout << "end line " << elem->endlinecount - 1 << std::endl;
			//std::cout << "Function Name: " << elem->name << ", No of function Lines = " << elem->endlinecount - elem->startlinecount + 1;
			//std::cout << "\n";
		}
	}
};
///////////////////////////////////////////////////////////////
// rule to detect preprocessor statements

class PreprocStatement : public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		if (pTc->find("#") < pTc->length())
		{
			doActions(pTc);
			return true;
		}
		return true;
	}
};

///////////////////////////////////////////////////////////////
// action to print preprocessor statement to console

class PrintPreproc : public IAction
{
	Repository* p_Repos;
public:
	PrintPreproc(Repository* pRepos) {
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		std::cout << "Preprocessor  Statementt: " << pTc->show().c_str();
	}
};

///////////////////////////////////////////////////////////////
// rule to detect for loop

class ForLoopFunction : public IRule
{
public:
	bool iscontrol(const std::string& tok)
	{
		const static std::string keys[]
			= { "for", "while","if", "else","switch","try", "catch"};
		for (int i = 0; i <5; ++i)
			if (tok == keys[i])
				return true;
		return false;
	}
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		if (tc[tc.length() - 1] == "{")
		{
			size_t len = tc.find("(");
			if ((len < tc.length() && iscontrol(tc[len - 1])))
			{
				doActions(pTc);
				return true;
			}
		}
		return true;
	}
};

class ShowForLoop : public IAction
{
	Repository* p_Repos;
public:
	ShowForLoop(Repository* pRepos) {
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		//p_Repos->scopeStack().pop();
		std::string name = (*pTc)[pTc->find("(") - 1];

		p_Repos->scopeStack().pop();
		p_Repos->scopeStack().top()->_children.pop_back();

		element* elem=new element;
		elem->type = "control statement";
		elem->name = name;
		p_Repos->scopeStack().top()->_children.push_back(elem);
		elem->startlinecount = p_Repos->startlinecount();
		p_Repos->scopeStack().push(elem);
	}
};

class DispFor : public IAction
{
	Repository* p_Repos;
public:
	DispFor(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		std::string name = (*pTc)[pTc->find("(") - 1];
		if (name == "for")
			std::cout << "\n  For Statement: " << pTc->show().c_str();
		else if (name == "if")
			std::cout << "\n  if Condition " << pTc->show().c_str();
		else if (name == "while")
			std::cout << "\n  while statement: " << pTc->show().c_str();
		else if (name == "switch")
			std::cout << "\n switch Statement: " << pTc->show().c_str();
		//else
			//std::cout << "\n else: " << pTc->show().c_str();
	}
};

///////////////////////////////////////////////////////////////
// rule to detect class or structure

class ClassFunction : public IRule
{
public:
	bool isClass(const std::string& tok)
	{
		const static std::string keys[] = { "class", "struct" };
		for (int i = 0; i <= 1; i++)
			if (tok == keys[i])
				return false;
		return false;
	}
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		if (tc[tc.length() - 1] == "{")
		{
			if ((pTc->find("class") < pTc->length()) || (pTc->find("struct") < pTc->length()))
			{
				doActions(pTc);
				return true;
			}
		}
		return true;
	}
};
//////////////////////////////////////////////////////////////////
class ShowClass : public IAction
{
	Repository* p_Repos;
public:
	ShowClass(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		//p_Repos->scopeStack().pop();
		p_Repos->scopeStack().pop();
		p_Repos->scopeStack().top()->_children.pop_back();
		std::string name = (*pTc)[pTc->find("{") - 1];
		element* elem=new element;
		elem->type = "Class or struct";
		elem->name = name;
		//p_Repos->scopeStack().push(elem);
		p_Repos->scopeStack().top()->_children.push_back(elem);
		p_Repos->scopeStack().push(elem);


	}
};

// Class to displlay the Class or struct name
class DispClass : public IAction
{
	Repository* p_Repo;
public:
	DispClass(Repository* pRepo)
	{
		p_Repo = pRepo;
	}
	void doAction(ITokCollection*& pTc)
	{
		std::cout << "\n Class/ Struct name: " << pTc->show().c_str();
	}
};


////////////////////////////////////////////////////////////////////////////////////else/////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
//rule for else statement

class ElseFunction :public IRule
{
public:
	bool iscontrol(const std::string& tok)
	{
		const static std::string keys = "else";
		if (tok == keys)
			return true;
		return false;
	}
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		if (tc[tc.length() - 1] == "{")
		{
			if (pTc->find("else") < pTc->length()) 
			{
				doActions(pTc);
				return true;
			}
		}
		return true;
	}
};

///////////////////////////////////////////////////////////////
// For Else  action
class ShowElse :public IAction
{
	Repository* p_Repos;
public:
	ShowElse(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		p_Repos->scopeStack().pop();
		std::string name = (*pTc)[pTc->find("{") - 1];
		element* elem= new element;
		elem->type = "Else ";
		elem->name = name;
		//p_Repos->scopeStack().push(elem);
		p_Repos->scopeStack().top()->_children.push_back(elem);
		p_Repos->scopeStack().push(elem);
	}
};
//////////////////////////////////////////////////////////
//Display else statement 

class DispElse : public IAction
{
	Repository* p_Repos;
public:
	DispElse(Repository* pRepos)
	{
		p_Repos = pRepos;
	}

	void doAction(ITokCollection*& pTc)
	{
		std::cout << "\nElse statement  " << pTc->show().c_str();
	}
};
//////////////////////////////////////////////////////////////////////////else/////////////////////////////////////////////////////////////////////////////
//////Lambda Rule Check ///////////

class checkLambda :public IRule
{
public:
	bool isspecialKeyWord(const std::string& tok)
	{
		const static std::string keys[] = { "for","if", "while","switch" };
		for (int i = 0; i < 5; i++)
		{
			if (tok==keys[i])
				return true;
			return false;
		}
	}

	bool doTest(ITokCollection *& pTc)
	{
		ITokCollection& tc = *pTc;
		/*p_Repos->scopeStack().pop();
		p_Repos->scopeStack().top()->_children.pop_back();*/

		//if (tc[tc.length() - 1] == "{")
		//{
		size_t len = tc.find("(");
		size_t len1 = tc.find("]");
		size_t len2 = tc.find("[");
		size_t len3 = tc.find("=");
		if ((len - len1 == 1) || (len2 - len3 == 1) /* && !isSpecialKeyWord(tc[len - 1])*/)
		{
			doActions(pTc);
			return true;
		}
		//}
		return true;
	}
};

class LambdaAction : public IAction
{
	Repository* p_Repos;
public:
	LambdaAction(Repository* pRepo)
	{
		p_Repos = pRepo;
	}

	void doAction(ITokCollection*& pTc)
	{
		// pop anonymous scop
		// push function scope
		p_Repos->scopeStack().pop();
		p_Repos->scopeStack().top()->_children.pop_back();
		//std::string name = (*pTc)[pTc->find("=") - 1];
		element* elem=new element;
		elem->type = "lambda";
		elem->name = "function";
		elem->startlinecount = p_Repos->startlinecount();
		p_Repos->scopeStack().top()->_children.push_back(elem);
		p_Repos->scopeStack().push(elem);
	}
};

class DispLambda : public IAction
{
	Repository* p_Repo;
public:
	DispLambda(Repository* pRepo)
	{
		p_Repo = pRepo;
	}
	void doAction(ITokCollection*& pTc)
	{
		std::cout << "\n Lambdas " << pTc->show().c_str();
	}
};

///////////////////////////////////////////////////////////////
// rule to detect function definitions

class FunctionDefinition : public IRule
{
public:
	bool isSpecialKeyWord(const std::string& tok)
	{
		const static std::string keys[]
			= { "for", "while", "switch", "if", "catch" };
		for (int i = 0; i<5; ++i)
			if (tok == keys[i])
				return true;
		return false;
	}
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		if (tc[tc.length() - 1] == "{")
		{
			size_t len = tc.find("(");
			size_t len1 = tc.find("]");
			size_t len2 = tc.find("[");
			size_t len3 = tc.find("=");
			if (len < tc.length() && !isSpecialKeyWord(tc[len - 1]) && !((len - len1) == 1) && !(len2 - len3 == 1))
			{
				doActions(pTc);
				return true;
			}
		}
		return true;
	}
};
///////////////////////////////////////////////////////////////
// action to push function name onto ScopeStack

class PushFunction : public IAction
{
	Repository* p_Repos;
public:
	PushFunction(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		// pop anonymous scope
		p_Repos->scopeStack().pop();
		p_Repos->scopeStack().top()->_children.pop_back();
		// push function scope
		std::string name = (*pTc)[pTc->find("(") - 1];
		element* elem=new element;
		elem->type = "function";
		elem->name = name;
		elem->startlinecount = p_Repos->startlinecount();
		p_Repos->scopeStack().top()->_children.push_back(elem);
		p_Repos->scopeStack().push(elem);
	}
};

///////////////////////////////////////////////////////////////
// action to send semi-expression that starts a function def
// to console

class PrintFunction : public IAction
{
	Repository* p_Repos;
public:
	PrintFunction(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		std::cout << "\n  FuncDef: " << pTc->show().c_str();
	}
};

///////////////////////////////////////////////////////////////
// action to send signature of a function def to console

class PrettyPrintFunction : public IAction
{
public:
	void doAction(ITokCollection*& pTc)
	{
		pTc->remove("public");
		pTc->remove(":");
		pTc->trimFront();
		size_t len = pTc->find(")");
		std::cout << "\n\n  Pretty Stmt:    ";
		for (int i = 0; i<len + 1; ++i)
			std::cout << (*pTc)[i] << " ";
		std::cout << "\n";
	}
};

///////////////////////////////////////////////////////////////
// rule to detect declaration

class Declaration : public IRule          // declar ends in semicolon
{                                         // has type, name, modifiers &
public:                                   // initializers.  So eliminate
	bool isModifier(const std::string& tok) // modifiers and initializers.
	{                                       // If you have two things left
		const size_t numKeys = 22;            // its declar else executable.
		const static std::string keys[numKeys] = {
			"const", "extern", "friend", "mutable", "signed", "static",
			"typedef", "typename", "unsigned", "volatile", "&", "*", "std", "::",
			"public", "protected", "private", ":", "typename", "typedef", "++", "--"
		};
		for (int i = 0; i<numKeys; ++i)
			if (tok == keys[i])
				return true;
		return false;
	}
	void condenseTemplateTypes(ITokCollection& tc)
	{
		size_t start = tc.find("<");
		size_t end = tc.find(">");
		if (start >= end || start == 0)
			return;
		else
		{
			if (end == tc.length())
				end = tc.find(">::");
			if (end == tc.length())
				return;
		}
		std::string save = tc[end];
		std::string tok = tc[start - 1];
		for (size_t i = start; i < end + 1; ++i)
			tok += tc[i];
		for (size_t i = start; i < end + 1; ++i)
			tc.remove(start);
		if (save == ">::")
		{
			tok += tc[start + 1];
			tc.remove(start);
		}
		tc[start - 1] = tok;
		//std::cout << "\n  -- " << tc.show();
	}
	bool isSpecialKeyWord(const std::string& tok)
	{
		const static std::string keys[]
			= { "for", "while", "switch", "if", "catch" };
		for (int i = 0; i<5; ++i)
			if (tok == keys[i])
				return true;
		return false;
	}
	void removeInvocationParens(ITokCollection& tc)
	{
		size_t start = tc.find("(");
		size_t end = tc.find(")");
		if (start >= end || end == tc.length() || start == 0)
			return;
		if (isSpecialKeyWord(tc[start - 1]))
			return;
		for (size_t i = start; i < end + 1; ++i)
			tc.remove(start);
		//std::cout << "\n  -- " << tc.show();
	}
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& in = *pTc;
		Scanner::SemiExp tc;
		for (size_t i = 0; i<in.length(); ++i)
			tc.push_back(in[i]);

		if (tc[tc.length() - 1] == ";" && tc.length() > 2)
		{
			//std::cout << "\n  ++ " << tc.show();
			removeInvocationParens(tc);
			condenseTemplateTypes(tc);
			// remove modifiers, comments, newlines, returns, and initializers
			Scanner::SemiExp se;
			for (size_t i = 0; i < tc.length(); ++i)
			{
				if (isModifier(tc[i]))
					continue;
				if (se.isComment(tc[i]) || tc[i] == "\n" || tc[i] == "return")
					continue;
				if (tc[i] == "=" || tc[i] == ";")
					break;
				else
					se.push_back(tc[i]);
			}
			if (se.length() == 2)  // type & name, so declaration
			{
				doActions(pTc);
				return true;
			}
		}
		return true;
	}
};

///////////////////////////////////////////////////////////////
// action to show declaration

class ShowDeclaration : public IAction
{
public:
	void doAction(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		// remove comments
		Scanner::SemiExp se;
		for (size_t i = 0; i<tc.length(); ++i)
			if (!se.isComment(tc[i]))
				se.push_back(tc[i]);
		// show cleaned semiExp
		std::cout << "\n  Declaration: " << se.show();
	}
};

///////////////////////////////////////////////////////////////
// rule to detect executable

class Executable : public IRule           // declar ends in semicolon
{                                         // has type, name, modifiers &
public:                                   // initializers.  So eliminate
	bool isModifier(const std::string& tok) // modifiers and initializers.
	{                                       // If you have two things left
		const size_t numKeys = 22;            // its declar else executable.
		const static std::string keys[numKeys] = {
			"const", "extern", "friend", "mutable", "signed", "static",
			"typedef", "typename", "unsigned", "volatile", "&", "*", "std", "::",
			"public", "protected", "private", ":", "typename", "typedef", "++", "--"
		};
		for (int i = 0; i<numKeys; ++i)
			if (tok == keys[i])
				return true;
		return false;
	}
	void condenseTemplateTypes(ITokCollection& tc)
	{
		size_t start = tc.find("<");
		size_t end = tc.find(">");
		if (start >= end || start == 0)
			return;
		else
		{
			if (end == tc.length())
				end = tc.find(">::");
			if (end == tc.length())
				return;
		}
		std::string save = tc[end];
		std::string tok = tc[start - 1];
		for (size_t i = start; i < end + 1; ++i)
			tok += tc[i];
		for (size_t i = start; i < end + 1; ++i)
			tc.remove(start);
		if (save == ">::")
		{
			tok += tc[start + 1];
			tc.remove(start);
		}
		tc[start - 1] = tok;
		//std::cout << "\n  -- " << tc.show();
	}

	bool isSpecialKeyWord(const std::string& tok)
	{
		const static std::string keys[]
			= { "for", "while", "switch", "if", "catch" };
		for (int i = 0; i<5; ++i)
			if (tok == keys[i])
				return true;
		return false;
	}
	void removeInvocationParens(ITokCollection& tc)
	{
		size_t start = tc.find("(");
		size_t end = tc.find(")");
		if (start >= end || end == tc.length() || start == 0)
			return;
		if (isSpecialKeyWord(tc[start - 1]))
			return;
		for (size_t i = start; i < end + 1; ++i)
			tc.remove(start);
		//std::cout << "\n  -- " << tc.show();
	}
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& in = *pTc;
		Scanner::SemiExp tc;
		//std::cout << in.length() << "\n";
		for (size_t i = 0; i<in.length(); ++i)
			tc.push_back(in[i]);

		if (tc[tc.length() - 1] == ";" && tc.length() > 2)
		{
			//std::cout << "\n  ++ " << tc.show();
			removeInvocationParens(tc);
			condenseTemplateTypes(tc);

			// remove modifiers, comments, newlines, returns, and initializers

			Scanner::SemiExp se;
			for (size_t i = 0; i < tc.length(); ++i)
			{
				if (isModifier(tc[i]))
					continue;
				if (se.isComment(tc[i]) || tc[i] == "\n" || tc[i] == "return")
					continue;
				if (tc[i] == "=" || tc[i] == ";")
					break;
				else
					se.push_back(tc[i]);
			}
			//std::cout << "\n  ** " << se.show();
			if (se.length() != 2)  // not a declaration
			{
				doActions(pTc);
				return true;
			}
		}
		return true;
	}
};

///////////////////////////////////////////////////////////////
// action to show executable

class ShowExecutable : public IAction
{
public:
	void doAction(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		// remove comments
		Scanner::SemiExp se;
		for (size_t i = 0; i < tc.length(); ++i)
		{
			if (!se.isComment(tc[i]))
				se.push_back(tc[i]);
		}
		// show cleaned semiExp
		std::cout << "\n  Executable: " << se.show();
	}
};

#endif