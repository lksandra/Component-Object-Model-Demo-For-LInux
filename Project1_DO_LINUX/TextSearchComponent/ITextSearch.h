/////////////////////////////////////////////////////////////////////////////
//  ITextSearch.h: This module defines the primary interface for
//                  functionality of TextSearch component.                 //
//  ver 1.0                                                                //
//  Language:     C++                                                       //
//  Platform:     Ubuntu 17.10 toshiba satellite                             //
//  Application:  Demonstrating Project 1 for CSE775. To search for matching//
//                regex string in a given list of files.
//				  												   //
//                                                              //
//                                                                         //
//  Author:       Lakshmi kanth sandra, 229653990                          //
//  Reference:    Prof Jim Fawcett                                         //
/////////////////////////////////////////////////////////////////////////////
//Module Operations:
//==================
//this interface for textSearch shared object helps in searching a regex string
//given a set of files. It also provides 
//implementation for reference counting methods. this is the interface for any
//client who wants to use TextSearch shared object.
//
/////////////////////////////////////////////////////////////////////////////
//required files:
//===============
//IUnknown.h
/////////////////////////////////////////////////////////////////////////////
//Public INterface:
//=================
//ITextSearch::ITextSearch().
//this method creates a ITextSearch object.
//
//--------------------------------------------------------------------------
//virtual bool sendFileListAndSearchString(vector<string>&fileList, string searchString) = 0
//this pure virtual method can be implemented to send the regex string to search for,
// and list of files.
//---------------------------------------------------------------------------
//virtual bool getResults(unordered_map<string,string>& output) = 0;
//this pure virtual method is used to retrieve results i.e a hashmap of files 
//and lineNumber string
//---------------------------------------------------------------------------
//virtual size_t addRef();
//this already implemented method is called by any user who creates a pointer to
//ITextSearch type.
//---------------------------------------------------------------------------
//virtual size_t release();
//this method is called by every client for destructing its ownership.
//---------------------------------------------------------------------------
//virtual ~ITextSearch();
/////////////////////////////////////////////////////////////////////////////
//maintenance history: V1.0, 23/04/2018.
//===================================
//NIL
/////////////////////////////////////////////////////////////////////////////

#ifndef ITEXTSEARCH_H
#define ITEXTSEARCH_H
#include "IUnknown.h"
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

class ITextSearch : Iunknown{
    public:
        
        ITextSearch(){
            cout<<"ITextSearch() being executed"<<endl;
        };
        virtual bool getResults(unordered_map<string,string>& output) = 0;
        virtual bool sendFileListAndSearchString(vector<string>&fileList, string searchString) = 0;
        virtual size_t addRef(){
            cout<<"addRef in ItextSearch being executed"<<endl;
            return Iunknown::addRef();
        };
        
        virtual size_t release(){
            cout<<"\nrelease in ITextSearch being executed"<<endl;
            return Iunknown::release();
        };
        
        virtual ~ITextSearch(){
            cout<< "\n~ITextSearch is being executed"<<endl;
        };
};

#endif /* ITEXTSEARCH_H */

