/////////////////////////////////////////////////////////////////////////////
//  ConfigureParser.cpp - builds and configures parsers                    //
//  ver 2.2                                                                //
//                                                                         //
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
This module builds and configures parsers.  It builds the parser
parts and configures them with application specific rules and actions.

Required Files:
===============
Parser.h
SemiExp.h
Tokenizer.h
ActionsAndRules.h
ConfigureParser.h

Maintenance History:
====================
ver 2.2 : 15 Mar 16
- Added new rules and actions and declared them
- Comments are added
ver 2.1 : 19 Feb 16
- Added PrintFunction action to FunctionDefinition rule
ver 2.0 : 01 Jun 11
- Major revisions to begin building a strong code analyzer
ver 1.1 : 01 Feb 06
- cosmetic changes to ConfigureParser.cpp
ver 1.0 : 12 Jan 06
- first release
*/
#include <fstream>
#include "Parser.h"
#include "../SemiExp/SemiExp.h"
#include "../Tokenizer/Tokenizer.h"
#include "ActionsAndRules.h"
#include "ConfigureParser.h"

using namespace Scanner;

//----< destructor releases all parts >------------------------------

ConfigParseToConsole::~ConfigParseToConsole()
{  // when Builder goes out of scope, everything must be deallocated

  delete pHandlePush;
  delete pBeginningOfScope;
  delete pHandlePop;
  delete pEndOfScope;
  delete pPrintFunction;
  delete pPushFunction;
  delete pFunctionDefinition;
  delete pDeclaration;
  delete pShowDeclaration;
  delete pExecutable;
  delete pShowExecutable;
  delete pRepo;
  delete pParser;
  delete pSemi;
  delete pToker;
  delete pForLoop;     //control
  delete pShowForLoop;
  delete pPrintPreproc;   //preprocessor statement
  pIn->close();
  delete pIn;
}
//----< attach toker to a file stream or stringstream >------------

bool ConfigParseToConsole::Attach(const std::string& name, bool isFile)
{
  if(pToker == 0)
    return false;
  pIn = new std::ifstream(name);
  if (!pIn->good())
    return false;
  return pToker->attach(pIn);
}
//----< Here's where alll the parts get assembled >----------------

Parser* ConfigParseToConsole::Build(){
  try  {
	pToker = new Toker;
    pToker->returnComments(false);
    pSemi = new SemiExp(pToker);
    pParser = new Parser(pSemi);
    pRepo = new Repository(pToker);
	pRepo->configRoot();
	pBeginningOfScope = new BeginningOfScope();
    pHandlePush = new HandlePush(pRepo);
    pBeginningOfScope->addAction(pHandlePush);
    pParser->addRule(pBeginningOfScope);    pEndOfScope = new EndOfScope();
    pHandlePop = new HandlePop(pRepo);    pEndOfScope->addAction(pHandlePop);
    pParser->addRule(pEndOfScope);	pFunctionDefinition = new FunctionDefinition;       // configure to detect and act on function definitions                                             
    pPushFunction = new PushFunction(pRepo);  // no action
    pPrintFunction = new PrintFunction(pRepo);
    pFunctionDefinition->addAction(pPushFunction);    pFunctionDefinition->addAction(pPrintFunction);
    pParser->addRule(pFunctionDefinition);	pDeclaration = new Declaration;
    pShowDeclaration = new ShowDeclaration;    pDeclaration->addAction(pShowDeclaration);
    pParser->addRule(pDeclaration);    pExecutable = new Executable;
    pShowExecutable = new ShowExecutable;    pExecutable->addAction(pShowExecutable);
    pParser->addRule(pExecutable);	pForLoop = new ForLoopFunction;	pShowForLoop = new ShowForLoop(pRepo);
	pDispFor = new DispFor(pRepo);	pForLoop->addAction(pShowForLoop);	pForLoop->addAction(pDispFor);	pParser->addRule(pForLoop);
	pElseFunction = new ElseFunction;               //check else
	pShowElse = new ShowElse(pRepo);
	pDispElse = new DispElse(pRepo);	pElseFunction->addAction(pShowElse);
	pElseFunction->addAction(pDispElse);
	pParser->addRule(pElseFunction);
	pPrintPreproc = new PreprocStatement;	//Preprocessor statements
	pPrintPreproc->addAction(new PrintPreproc(pRepo));
	pParser->addRule(pPrintPreproc);
	pClassFunction = new ClassFunction;//class and struct
	pShowclass = new ShowClass(pRepo);
	pDispClass = new DispClass(pRepo);
	pClassFunction->addAction(pShowclass);
	pClassFunction->addAction(pDispClass);
	pParser->addRule(pClassFunction);
	pcheckLambda = new checkLambda; // check lambda
	pLambdaAction = new LambdaAction(pRepo);
	pDispLambda = new DispLambda(pRepo);
	pcheckLambda->addAction(pLambdaAction);
	pcheckLambda->addAction(pDispLambda);
	pParser->addRule(pcheckLambda);
	return pParser;  }
  catch(std::exception& ex) {
    std::cout << "\n\n  " << ex.what() << "\n\n";
    return 0; }
}

// build2 to assign the action and rules
Parser* ConfigParseToConsole::Build2() {
	try {
		pToker = new Toker;
		pToker->returnComments(false);
		pSemi = new SemiExp(pToker);
		pParser = new Parser(pSemi);
		pRepo = new Repository(pToker);
		pRepo->configRoot();
		pBeginningOfScope = new BeginningOfScope();
		pHandlePush = new HandlePush(pRepo);
		pBeginningOfScope->addAction(pHandlePush);
		pParser->addRule(pBeginningOfScope);    pEndOfScope = new EndOfScope();
		pHandlePop = new HandlePop(pRepo);    pEndOfScope->addAction(pHandlePop);
		pParser->addRule(pEndOfScope);	pFunctionDefinition = new FunctionDefinition;       // configure to detect and act on function definitions                                             
		pPushFunction = new PushFunction(pRepo);  // no action
		//pPrintFunction = new PrintFunction(pRepo);
		pFunctionDefinition->addAction(pPushFunction);   
		pParser->addRule(pFunctionDefinition);	pDeclaration = new Declaration;
		//pShowDeclaration = new ShowDeclaration;    pDeclaration->addAction(pShowDeclaration);
		pParser->addRule(pDeclaration);    pExecutable = new Executable;
		//pShowExecutable = new ShowExecutable;    pExecutable->addAction(pShowExecutable);
		pParser->addRule(pExecutable);	pForLoop = new ForLoopFunction;	//pShowForLoop = new ShowForLoop(pRepo);
		//pDispFor = new DispFor(pRepo);	pForLoop->addAction(pShowForLoop);	pForLoop->addAction(pDispFor);
		pParser->addRule(pForLoop);
		pcheckLambda = new checkLambda; // check lambda
		pLambdaAction = new LambdaAction(pRepo);
		//pDispLambda = new DispLambda(pRepo);
		pcheckLambda->addAction(pLambdaAction);
		//pcheckLambda->addAction(pDispLambda);
		pParser->addRule(pcheckLambda);
		return pParser;
	}
	catch (std::exception& ex) {
		std::cout << "\n\n  " << ex.what() << "\n\n";
		return 0;
	}
}


//used to print the tree
void ConfigParseToConsole::PrintTree() {
	pRepo->printAsTree();

	std::cout << "\n Complexity is = "
		<< pRepo->absTree->FindComplexity(pRepo->absTree->getRootNode()) << std::endl;
	//std::cout << "\n  Complexity  is ";
	pRepo->absTree->TreeAnalysis(pRepo->absTree->getRootNode());
}

//used to print the tree
void ConfigParseToConsole::PrintTree2() {
	//pRepo->printAsTree();

	std::cout << "\n Complexity is = "
		<< pRepo->absTree->FindComplexity(pRepo->absTree->getRootNode()) << std::endl;
	//std::cout << "\n  Complexity  is ";
	pRepo->absTree->TreeAnalysis(pRepo->absTree->getRootNode());
}
#ifdef TEST_CONFIGUREPARSER

#include <queue>
#include <string>

int main(int argc, char* argv[])
{
  std::cout << "\n  Testing ConfigureParser module\n "
            << std::string(32,'=') << std::endl;

  // collecting tokens from files, named on the command line

  if(argc < 2)
  {
    std::cout 
      << "\n  please enter name of file to process on command line\n\n";
    return 1;
  }

  for(int i=1; i<argc; ++i)
  {
    std::cout << "\n  Processing file " << argv[i];
    std::cout << "\n  " << std::string(16 + strlen(argv[i]),'-');

    ConfigParseToConsole configure;
    Parser* pParser = configure.Build();
    try
    {
      if(pParser)
      {
        if(!configure.Attach(argv[i]))
        {
          std::cout << "\n  could not open file " << argv[i] << std::endl;
          continue;
        }
      }
      else
      {
        std::cout << "\n\n  Parser not built\n\n";
        return 1;
      }
      // now that parser is built, use it

      while(pParser->next())
        pParser->parse();
      std::cout << "\n\n";
    }
    catch(std::exception& ex)
    {
      std::cout << "\n\n    " << ex.what() << "\n\n";
    }
    std::cout << "\n\n";
  }
}

#endif
