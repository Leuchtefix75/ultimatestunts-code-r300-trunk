/***************************************************************************
                          CString.h  -  bones very own easy string class ;-)
                             -------------------
    begin                : Mon Sep 2 2002
    copyright            : (C) 2002 by bones
    email                : boesemar@users.sourceforge.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef _CString_H
#define _CString_H


// #include <std/bastring.h>
#include <string>

//  class CString : public std::basic_string<char>
 class CString : public std::string
 {
  private:
    char c_up(const char c) const;
    char c_down(const char c) const;

 public:
	//Constructors:
   CString(const char *s);
   CString();

	//Bones functions:
   CString & trimL();
   CString & trimR();
   CString & Trim();

   CString & toUpper();
   CString & toLower();
   CString & subStr(const int unsigned pos = 0, const int n = -1) const;
   CString & operator= (const char *str);
   CString getFirstLine();
   int countChar(const char c) const;
   int toInt();
   CString & dump() const;

	//CJP functions:
	//char *Cast() const; //instead, use c_str method

	//Operators are probably provided by std::string:
	CString operator+(CString const &val); //std didn't do what I wanted
	CString operator+(const char *val);
	//CString const &operator=(CString const &val);
	//CString const &operator+=(char const &val);
	//bool operator==(CString const &val) const;
	//bool operator!=(CString const &val) const;

	//int len() const; //instead, use length()

	//Don't know how to do these with std::string:
	CString mid(int i, int l);
	int instr(char c);

	//char getChar(int i); //instead, use operator[]
	//void putChar(int i, char c); //instead, use operator[]
	//void addChar(char c); //operator+= (?)

	//Don't know how to do this with std::string:
	float toFloat();

};

CString operator+(const char *val1, CString const &val2);

#endif

