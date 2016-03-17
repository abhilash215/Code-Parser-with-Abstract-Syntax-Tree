#ifndef ITOKCOLLECTION_H
#define ITOKCOLLECTION_H
/////////////////////////////////////////////////////////////////////////////
//  ITokCollection.h - package for the ITokCollection interface            //
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
/*
  Module Purpose:
  ===============
  ITokCollection is an interface that supports substitution of different
  types of scanners for parsing.  In this solution, we illustrate that
  by binding two different types of collections, SemiExp and XmlParts,
  to this interface.  This is illustrated in the test stubs for the
  SemiExpression and XmlElementParts modules.

  Maintenance History:
  ====================
  ver 1.3 :15 Mar 2016
  - comments and interface added
  ver 1.2 : 15 Feb 2016
  - removed merge, added non-const operator[] overload
  ver 1.1 : 02 Jun 11
  - added merge, remove overload, and default param in get
  ver 1.0 : 17 Jan 09
  - first release

  Public Interface
  * ------------------
  * find(const std::string& tok)
  * -function to find the position of token
  *remove(const std::string& tok)
  * -function to remove the token from particular position
  * remove(size_t n)
  * - function to remove the token
  * push_back(const std::string& tok)
  * -function to load the vector with tokens
  * length()
  *function to find the length of the vector
  
*/
#include <string> 

struct ITokCollection
{
  virtual bool get(bool clear=true)=0;
  virtual size_t length()=0;             //find the length
  virtual std::string& operator[](size_t n) = 0;
  virtual std::string operator[](size_t n) const = 0;
  virtual size_t find(const std::string& tok)=0;             // find token
  virtual void push_back(const std::string& tok)=0;
  virtual bool remove(const std::string& tok)=0;    // remove the token
  virtual bool remove(size_t i)=0;
  virtual void toLower()=0;                     //convert to lower
  virtual void trimFront()=0;
  virtual void clear()=0;                 //clear the token
  virtual std::string show(bool showNewLines=false)=0;
  virtual bool isComment(const std::string& tok) = 0;
  virtual ~ITokCollection() {};
};

#endif
