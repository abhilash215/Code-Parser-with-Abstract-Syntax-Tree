#ifndef DATASTORE_H
#define DATASTORE_H
/////////////////////////////////////////////////////////////////////////////
// DataStore.h - Store results of file search                              //
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
Store results of file search

Public Interface:
=================
save(const std::string& filespec)            //save the file
save(const std::string& filename, const std::string& path)   //save the file and path

*/

#include <vector>

// store the results of file search
class DataStore
{
public:
	using iterator = std::vector<std::string>::iterator;
	void save(const std::string& filespec) { store.push_back(filespec); }     //save the file
	void save(const std::string& filename, const std::string& path) {}     //save the file with the path
	iterator begin() { return store.begin(); }
	iterator end() { return store.end(); }
private:
	std::vector<std::string> store;
};
#endif