
//////////////////////////////////////////////////////////////////////////////
// MetricAnalysis.cpp - Used to display the function and complexity of files//
// ver 1.0                                                                  //
//Language :    C++, Visual Studio 2015                                     // 
// Application: Code Parser with AST,CSE687 - Object Oriented Design,SP16   //
//Platform:     Dell Inspiron,6th gen Core i5, Windows 10                   //
// Author:      Abhilash Udayashankar, SUID:778388557                       //
//              audayash@syr.edu, (774)-540-1234                            // 
//Original author:Jim Fawcett, CST 4-187, Syracuse University               //
//              (315) 443-3948, jfawcett@twcny.rr.com                       // 
//              jfawcett@twcny.rr.com                                       //
//////////////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
Used to display the function and complexity of files

Required files
=================
Parser.h
FileMngr.h
DataStore.h
ConfigureParser.h

Public Interface
=================

main()
used to test the MetricAnalysis

Maintenance History:
====================
ver 1.0 : 15 Mar 16
- first release


*/
#include "../Parser/Parser.h"
#include "../FileMngr/FileMngr.h"
#include "../FileMngr/DataStore.h"
#include"../Parser/ConfigureParser.h"
#include<iostream>
#include <queue>
#include <string>
#define Util StringHelper

#ifdef MET_ANALYSIS

int main()                                           // main function
{
	std::cout << "\n  Testing Metric Analyser";
	std::cout << "\n =================";
	DataStore ds;                                     
	FileMgr fm("..", ds);
	//fm.addPattern("*.h");
	fm.addPattern("*.cpp");                            //pattern
	//fm.addPattern("*.partial");
	fm.search();                                        //search
	std::cout << "\n\n  contents of DataStore";
	std::cout << "\n -----------------------";
	for (auto fs : ds)
	{	std::string fileSpec = FileSystem::Path::getFullFileSpec(fs);
			std::string msg = "Processing file" + fileSpec;
			ConfigParseToConsole configure;
			Parser* pParser = configure.Build();
			try
			{
				if (pParser)
				{
					if (!configure.Attach(fs))
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
					while (pParser->next())
					pParser->parse();
				std::cout << "\n";
			}
			catch (std::exception& ex)
			{
				std::cout << "\n\n    " << ex.what() << "\n\n";
			}
			configure.PrintTree();
		}
	}
	

#endif
