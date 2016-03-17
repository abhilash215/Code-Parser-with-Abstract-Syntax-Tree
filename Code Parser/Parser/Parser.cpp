/////////////////////////////////////////////////////////////////////////////
//  Parser.cpp - Analyzes C++ language constructs                          //
//  ver 1.5                                                                //
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
Analyzes C++ language constructs

Public Interface:
=================
addRule(IRule* pRule)           //add rules to the parser
next()                          // to next rule
loop()                          // to test
parse()                         // parse the semiexp
Toker t(someFile);              // create tokenizer instance
SemiExp se(&t);                 // create a SemiExp attached to tokenizer
Parser parser(se);              // now we have a parser
Rule1 r1;                       // create instance of a derived Rule class
Action1 a1;                     // create a derived action
r1.addAction(&a1);              // register action with the rule
parser.addRule(&r1);            // register rule with parser
while(se.getSemiExp())          // get semi-expression
parser.parse();               //   and parse it


Required Files:
===============
Utilities.h
Tokenizer.h
SemiExp.h
Parser.h
ActionsAndRules.h
ConfigureParser.h

Maintenance History:
====================
ver 1.6 : 15 Mar 16
-comments are added

*/


#include <iostream>
#include <string>
#include "../Utilities/Utilities.h"
#include "../Tokenizer/Tokenizer.h"
#include "../SemiExp/SemiExp.h"
#include "Parser.h"
#include "ActionsAndRules.h"
#include "ConfigureParser.h"

using namespace Scanner;
using namespace Utilities;

//----< register parsing rule >--------------------------------

void Parser::addRule(IRule* pRule)
{
  rules.push_back(pRule);
}
//----< get next ITokCollection >------------------------------

/*for (int i = 0; i < 10; i++)
{
	std::cout << "hello";
}*/

bool Parser::next() 
{ 
  // default operation of doFold(pTokColl) is to simply return
  // - code analysis provides specific rules for handling
  //   for statements and operator expressions which are
  //   bound in ConfigureParser.cpp

  bool succeeded = pTokColl->get();
  if(!succeeded)
    return false;
  return true;
}

/////////Testing /////////////////////////////////////////////////////////////////////

bool Parser::loop()
{
	for (int i = 0; i < 10; i++)
	{
		std::cout << "For loop";
	}
	int a=0, b=0;
	if (a ==  b)
	{
		std::cout << "hello1";
	}
	std::cout << "hello2";
	switch (a)
	{
	case 't':	std::cout << "hello3";
		break;
	} 
	return true;
}


std::function<std::string(int)> CreateLambda2()
{
	// this std function converts an integer to a string

	std::function<std::string(int)> f = [](int i)
	{
		std::ostringstream convert;
		convert << i;                      // lambda captures value of i
		return convert.str();
	};
	return f;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

//----< parse the SemiExp by applying all rules to it >--------

bool Parser::parse()
{
  bool succeeded = false;
  for(size_t i=0; i<rules.size(); ++i)
  {
    if(rules[i]->doTest(pTokColl))
      succeeded = true;
  }
  return succeeded;
}
//----< register action with a rule >--------------------------

void IRule::addAction(IAction *pAction)
{
  actions.push_back(pAction);
}
//----< invoke all actions associated with a rule >------------

void IRule::doActions(ITokCollection*& pTokColl)
{
  if(actions.size() > 0)
    for(size_t i=0; i<actions.size(); ++i)
      actions[i]->doAction(pTokColl);
}

//----< test stub >--------------------------------------------

#include "../FileSystem-Windows/FileSystemDemo/FileSystem.h"

#ifdef TEST_PARSER

#include <queue>
#include <string>
#define Util StringHelper


//main function

int main(int argc, char* argv[])
{
  Util::Title("Testing Parser Class", '=');
  putline();

  // collecting tokens from files, named on the command line

  if(argc < 2)
  {
    std::cout 
      << "\n  please enter name of file to process on command line\n\n";
    return 1;
  }

  for(int i=1; i<argc; ++i)
  {
    std::string fileSpec = FileSystem::Path::getFullFileSpec(argv[i]);
    std::string msg = "Processing file" + fileSpec;
    Util::title(msg);
    putline();

    ConfigParseToConsole configure;
    Parser* pParser = configure.Build();
    try
    {
      if(pParser)
      {
        if(!configure.Attach(argv[i]))
        {
          std::cout << "\n  could not open file " << fileSpec << std::endl;
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
      std::cout << "\n";
    }
    catch(std::exception& ex)
    {
      std::cout << "\n\n    " << ex.what() << "\n\n";
    }
	configure.PrintTree();
  }
  std::cout << "\n";
}

#endif
