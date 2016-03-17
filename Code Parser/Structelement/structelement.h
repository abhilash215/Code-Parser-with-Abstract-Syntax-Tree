
/////////////////////////////////////////////////////////////////////////////
//  Structelement.h - package for the struct element                       //
//  ver 1.3                                                                //
//Language :    C++, Visual Studio 2015                                    // 
// Application: Code Parser with AST,CSE687 - Object Oriented Design,SP16  //
//Platform:     Dell Inspiron,6th gen Core i5, Windows 10                  //
// Author:      Abhilash Udayashankar, SUID:778388557                      //
//              audayash@syr.edu, (774)-540-1234                           // 
//Original author:Jim Fawcett, CST 4-187, Syracuse University              //
//              (315) 443-3948, jfawcett@twcny.rr.com                      // 
//              jfawcett@twcny.rr.com                                      //
/////////////////////////////////////////////////////////////////////////////
#pragma once
#include<string>
#include <vector>
#include <iostream>
#include <sstream>
struct element   // element is a struct
{
	std::string type;
	std::string name;
	size_t startlinecount;
	size_t endlinecount;
	std::string show() // displays output in the format (name,type,startline,endline)
	{
		std::ostringstream temp;		//temp is of type ostringstream
		temp << "(";
		temp << type;
		temp << ", ";
		temp << name;
		temp << ", ";
		temp << startlinecount;
		temp << ", ";
		temp << endlinecount;
		temp << ")";
		return temp.str();// converts the format above to a string object to be displayed
	}
	std::vector<element*> _children; //children is a vector of element pointers
};