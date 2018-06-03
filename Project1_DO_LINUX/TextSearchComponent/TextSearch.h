/////////////////////////////////////////////////////////////////////////////
//  TextSearch.h: This module defines the interface for
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
//implementation for reference counting methods.
//
/////////////////////////////////////////////////////////////////////////////
//required files:
//===============
//ITextSearch.h
/////////////////////////////////////////////////////////////////////////////
//Public INterface:
//=================
//textSearch::textSearch()
//this method creates a ITextSearch object and initiates ref counting.
//
//--------------------------------------------------------------------------
//bool sendFileListAndSearchString(vector<string>&fileList, string searchString);
//this method is implemented to send the regex string to search for,
// and list of files.
//---------------------------------------------------------------------------
//bool getResults(unordered_map<string,string>& output);
//this method is used to retrieve results i.e a hashmap of files 
//and lineNumber strings
//---------------------------------------------------------------------------
// ~ITextSearch();
//---------------------------------------------------------------------------
//ITextSearch* globalCreateTextSearch();
//this unmangled global factory method returns *textSearch. 
//---------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////
//maintenance history: V1.0, 23/04/2018.
//===================================
//NIL
/////////////////////////////////////////////////////////////////////////////


#ifndef TEXTSEARCH_H
#define TEXTSEARCH_H
#include "ITextSearch.h"
#include <unordered_map>
#include <vector>
#include <string>
using namespace std;

class textSearch : public ITextSearch{
    public:
      
        textSearch(){
            cout<<"textSearch() being executed"<<endl;
          addRef();  
        };
        
        textSearch(textSearch& tS){
            cout<<"textSearch( tS& ) being executed"<<endl;
          tS.addRef();  
          *this = tS;  
        };
        
        textSearch& operator=( textSearch& tS){
            tS.addRef();
            cout<<"textSearch operator=(tS&) being executed"<<endl;
            return tS;
        };
        bool getResults(unordered_map<string,string>& output);
       
        bool sendFileListAndSearchString(vector<string>&fileList, 
                                                        string searchString);
        ~textSearch(){
            cout<<"\n~textSearch is being executed"<<endl;
        };
    private:
        bool searchForTheString(unordered_map<string,string>& output);
       
        string searchString;
        vector<string>* fileList;
            
        
};

extern "C" ITextSearch* globalCreateTextSearch();

#endif /* TEXTSEARCH_H */

