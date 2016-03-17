
//////////////////////////////////////////////////////////////////////////////
// MetricExecutive.cpp -Used to display the function and complexity of files//
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



class Executive
{
public :
	void TestR3()   //testing for requirement
	{
		std::cout << "\n";
		std::cout << "Demonstrating Requirement 1\n";
		std::cout << "------------------------------------------------------------------------------------------\n";
		std::cout << "Visual Studio 2015 and its C++ Windows Console Projects are used to develop the projects.\n";
		std::cout << "\n";


		std::cout << "\n";
		std::cout << "Demonstrating Requirement 2\n";
		std::cout << "------------------------------------------------------------------------------------------\n";
		std::cout << " C++ standard library's streams for all I/O and new and delete for all heap-based memory management is used.\n";
		std::cout << "\n";



		std::cout << "\n";
		std::cout << "Demonstrating Requirement 3\n";
		std::cout << "------------------------------------------------------------------------------------------\n";
		std::cout << "C++ packages for analyzing function size and complexity metrics for a set of specified packages are provided.\n";
		std::cout << "\n";
	}

	void TestR4()    //testing for requirement
	{
		std::cout << "\n";
		std::cout << "Demonstrating Requirement 4\n";
		std::cout << "------------------------------------------------------------------------------------------\n";
		std::cout << "A Parser package with Parser class that defines all rules and actions is provided.\n";
		std::cout << "\n";
	}

	void TestR5() //testing for requirement
	{
		std::cout << "\n";
		std::cout << "Demonstrating Requirement 5\n";
		std::cout << "------------------------------------------------------------------------------------------\n";
		std::cout << "A Rules and Action Package is provided to detect the rules .\n";
		std::cout << "\n";
	}

	void TestR6()   //testing for requirement
	{
		std::cout << "\n";
		std::cout << "Demonstrating Requirement 6\n";
		std::cout << "------------------------------------------------------------------------------------------\n";
		std::cout << " The Abstract Sysntax Tree is built and displayed .\n";
		std::string fileSpec = FileSystem::Path::getFullFileSpec("Testing.cpp");
		std::string msg = "Processing file" + fileSpec;
		

		ConfigParseToConsole configure;
		Parser* pParser = configure.Build();
		try
		{
			if (pParser)
			{
				if (!configure.Attach("Testing.cpp"))
				{
					std::cout << "\n  could not open file " << fileSpec << std::endl;
				}
			}
			else
			{
				std::cout << "\n\n  Parser not built\n\n";
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
		std::cout << " The Abstract Sysntax Tree is built and displayed .\n";
		std::cout << "\n";
	}


	void TestR7()   //testing for requirement
	{
		std::cout << "\n";
		std::cout << "Demonstrating Requirement 7\n";
		std::cout << "------------------------------------------------------------------------------------------\n";
		std::cout << " FileMgr package that supports finding files and/or directories in a directory tree rooted at a specified\n path is provided .\n";

		DataStore ds;
		FileMgr fm("..", ds);
		//fm.addPattern("*.h");
		fm.addPattern("*.cpp");
		//fm.addPattern("*.partial");
		fm.search();

		std::cout << "\n\n  contents of DataStore";
		std::cout << "\n -----------------------";
		for (auto fs : ds)
		{
			std::cout << "\n  " << fs;
		}
		std::cout << "\n\n";
	}


	void TestR8and9(std::string pattern, std::string path)            //testing for requirement
	{
		std::cout << "\n";
		std::cout << "Demonstrating Requirement 8 and 9 \n";
		std::cout << "------------------------------------------------------------------------------------------\n";
		std::cout << " MetricsAnalysis and MetricsExecutive package for evaluating and displaying the size and complexity is provided .\n";

		DataStore ds;
		FileMgr fm(path, ds);
		fm.addPattern(pattern);
		fm.search();
		for (auto fs : ds)
		{
			std::string fileSpec = FileSystem::Path::getFullFileSpec(fs);
			std::string msg = "Processing file" + fileSpec;	
			ConfigParseToConsole configure;
			Parser* pParser = configure.Build2();
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
				}
				while (pParser->next())
					pParser->parse();
				std::cout << "\n";
			}
			catch (std::exception& ex)
			{
				std::cout << "\n\n    " << ex.what() << "\n\n";
			}
			configure.PrintTree2();
			std::cout << "\n  " << fs;
		}
		std::cout << "\n\n";
		std::cout << "\n";
	}
};
#ifdef MET_ANALYSIS

int main(int argc, char* argv[])
{	Executive test;
	test.TestR3();           //calling the test methods
	test.TestR4();
	test.TestR5();
	test.TestR6();
	test.TestR7();
	test.TestR8and9(argv[1], argv[2]);
	DataStore ds;                         // filemanager 
	FileMgr fm("..", ds);
	fm.addPattern("Parser.cpp");
	//fm.addPattern("*.partial");
	fm.search();
	std::cout << "\n\n  contents of DataStore";
	std::cout << "\n -----------------------";
	for (auto fs : ds)
	{
		std::string fileSpec = FileSystem::Path::getFullFileSpec(fs);
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
			}// now that parser is built, use it
			while (pParser->next())
				pParser->parse();
			std::cout << "\n";
		}
		catch (std::exception& ex)
		{
			std::cout << "\n\n    " << ex.what() << "\n\n";
		}
		configure.PrintTree();
		std::cout << "\n";
		std::cout << "Demonstrating Requirement 10\n";
		std::cout << "------------------------------------------------------------------------------------------\n";
		std::cout << " All the project requirements are demonstrated in the MetricsExecutive Package and displayed .\n";
		std::cout << "\n";
	}
}


#endif
