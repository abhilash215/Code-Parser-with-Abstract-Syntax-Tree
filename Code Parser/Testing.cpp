/////////////////////////////////////////////////////////////////////////////
// FileMngr.cpp-A package that navigates through a directory tree rooted at// 
//              some specified path and returns names of all the files     //
//              and/or directories matching a pattern.                     //
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
A package that navigates through a directory tree rooted at some specified path
and returns names of all the files and/or directories matching a pattern.

Maintenance History:
====================

ver 1.0 : 12 Jan 06
- first release

Public Interface:
=================
addPattern()                //adefine pattern of files required
addPattern()                // serch the path or file

Required Files:
===============
fileMngr.h
DataStore.h

*/

#include "../FileMngr/FileMngr.h"
#include "../FileMngr/DataStore.h"
#include <iostream>

#ifdef TEST_FILEMGR

int main()
{
	std::cout << "\n  Testing FileMgr";
	std::cout << "\n =================";

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
	return 0;
}
#endif
