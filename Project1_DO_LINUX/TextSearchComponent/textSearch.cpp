/////////////////////////////////////////////////////////////////////////////
//  TextSearch.cpp: This module defines the interface for
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
//this module provides implementations for textSearch.h. It helps
// in searching a regex string given a set of files.
//
/////////////////////////////////////////////////////////////////////////////
//required files:
//===============
//TextSearch.h
//FileSystem.h
/////////////////////////////////////////////////////////////////////////////
//Public INterface:
//=================
//textSearch::textSearch().
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

#include "TextSearch.h"
#include "FileSystem.h"
#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>
#include <regex>
using namespace std;
using namespace FileSystem;

//this global factory method returns the textSearch object.
ITextSearch* globalCreateTextSearch(){
    return new textSearch;
};




//this method receives list of file paths and a regex search string.
bool textSearch::sendFileListAndSearchString(vector<string>& fileList, string searchString){
    cout<< "textSearch::sendFileList...function executing"<<endl;
    if(fileList.size()==0 || searchString=="")
        return false;
    
    this->fileList = &fileList;
    this->searchString = searchString;
    cout<<"\n textSearch-successfully recvd: fileList size: "<< this->fileList->size()<<endl;
    return true;
};

//this helper method performs string search on the received file list and appends the file and coresponding
//line number string in to a hashmap.
bool textSearch::searchForTheString(unordered_map<string,string>& output){
    try{
        for(int i =0; i< this->fileList->size(); i++){
            
            File fileToRead((*(this->fileList))[i]);
            fileToRead.open(File::in, File::text);
            string s;
            string lineNumberString = "";
            regex rx(this->searchString);
            long count=0;
            while(fileToRead.isGood()){
                s=fileToRead.getLine();
                if(regex_search(s,rx)){
                    
                    lineNumberString+=std::to_string(count) += " ";
                    count++;
                }
            }
            output[(*(this->fileList))[i]]=lineNumberString;
            fileToRead.close();
            

        }
        
        return true;
   }catch(...){
       return false;
   }
};


//clients receive results through this method.
bool textSearch::getResults(unordered_map<string,string>& output){
    if(searchForTheString(output)){
        cout<<"\n Inside TextSearch com: getResults: output size: "<< output.size()<<endl;
        return true;
    }else{
        cout<<"\n Inside TextSearch com: searchForTheString func returned false"<<endl;
        return false;
    };
        
};




