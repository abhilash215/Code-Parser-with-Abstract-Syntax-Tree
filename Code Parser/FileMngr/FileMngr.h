#ifndef FILEMGR_H
#define FILEMGR_H
/////////////////////////////////////////////////////////////////////////////
// FileMgr.h - find files matching specified patterns                      //
//             on a specified path                                         //
// ver 2.1                                                                 //
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
-Find files matching specified patterns on a specified path. 

Required Files:
===============
FileSystem.h
DataDtore.h

Public Interface:
=================
addPattern()                //adefine pattern of files required
addPattern()                // serch the path or file


Maintenance History:
====================
* ver 2.1 : 15 Mar 2016
* ver 2.0 : 12 Mar 2016
* - fixes bug in directory recursion in find(path)
* - reduces scope of for loop in find for efficiency
* ver 1.0 : 19 Feb 2016
* - first release
*/
#include "FileSystem.h"
#include "DataStore.h"
#include <iostream>


//find the files matching
class FileMgr
{
public:
	using iterator = DataStore::iterator;
	using patterns = std::vector<std::string>;

	FileMgr(const std::string& path, DataStore& ds) : path_(path), store_(ds)
	{
		patterns_.push_back("*.*");
	}

	void addPattern(const std::string& patt)                        // add different patterns for choosing files
	{
		if (patterns_.size() == 1 && patterns_[0] == "*.*")
			patterns_.pop_back();
		patterns_.push_back(patt);
	}

	void search()                                            // search the file path
	{
		find(path_);
	}

	void find(const std::string& path)    //find the path
	{
		std::string fpath = FileSystem::Path::getFullFileSpec(path);

		//std::cout << "\n  ++ " << fpath;
		for (auto patt : patterns_)  // the scope of this for loop was
		{                            // changed to improve efficiency
			std::vector<std::string> files = FileSystem::Directory::getFiles(fpath, patt);
			for (auto f : files)
			{
			//	std::cout << "\n  --   " << f;
				store_.save(fpath + "\\" +f);
			}
		}
		std::vector<std::string> dirs = FileSystem::Directory::getDirectories(fpath);
		for (auto d : dirs)
		{
			if (d == "." || d == "..")
				continue;
			std::string dpath = fpath + "\\" + d;  // here's the fix
			find(dpath);
		}
	}
private:
	std::string path_;
	DataStore& store_;
	patterns patterns_;
};

#endif
