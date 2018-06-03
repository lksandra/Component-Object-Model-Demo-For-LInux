/////////////////////////////////////////////////////////////////////////////
//  FileManager.h: This module defines the primary interface for
//                  functionality of FileManager component.                 //
//  ver 1.0                                                                //
//  Language:     C++                                                       //
//  Platform:     Ubuntu 17.10 toshiba satellite                             //
//  Application:  Demonstrating Project 1 for CSE775. To search for files
//					in a directory for given file patterns.
//				  												   //
//                                                              //
//                                                                         //
//  Author:       Lakshmi kanth sandra, 229653990                          //
//  Reference:    Prof Jim Fawcett                                         //
/////////////////////////////////////////////////////////////////////////////
//Module Operations:
//==================
//this interface for shared object helps in searching a given directory for 
//set of files that match the file extension patterns. It also provides 
//implementation for reference counting methods.
//
/////////////////////////////////////////////////////////////////////////////
//required files:
//===============
//IUnknown.h
/////////////////////////////////////////////////////////////////////////////
//Public INterface:
//=================
//IFileManager::IFileManager().
//this method creates a IFileManager object.
//
//--------------------------------------------------------------------------
//IFileManager::SendFileProperties(string path, vector<string> patterns)
//this pure virtual method can be implemented to send the directory path to search for,
// and file patterns that need to be matched.
//---------------------------------------------------------------------------
//IFileManager::vector<string> GetResults();
//this pure virtual method is used to retrieve results i.e list of files
//---------------------------------------------------------------------------
//IFileManager* globalCreateFileManager();
//this unmangled global factory method is used to create a FileManager pointer on heap.
//---------------------------------------------------------------------------
//virtual size_t addRef();
//this already implemented method is called by any user who creates a pointer to
//IFileManager.
//---------------------------------------------------------------------------
//virtual size_t release();
//this method is called by every client for destructing its ownerhsip.
//---------------------------------------------------------------------------
//virtual ~IFileManager();
/////////////////////////////////////////////////////////////////////////////
//maintenance history: V1.0, 23/04/2018.
//===================================
//NIL
/////////////////////////////////////////////////////////////////////////////

#ifndef IFILEMANAGER_H
#define IFILEMANAGER_H
#include <iostream>
#include <string>
#include <vector>
#include "IUnknown.h"
using namespace std;

class IFileManager: public Iunknown{
    
public:
  
    IFileManager(){
        cout<<"IFileManager() being executed"<<endl;
    };
    
    IFileManager(const IFileManager& IfM){
        cout<<"IFileManager(IFileManager& IfM) being executed"<<endl;
     /*   *this = IfM;
        cout<<"IFileManager(IFileManager& IfM) being executed"<<endl; */
    };
    
    IFileManager& operator=(const IFileManager& IfM){
     /*   cout<<"IFileManager& operator=(IFileManager& IfM) being executed"<<endl;
        *this = IfM; */
        return *this;
    };
    virtual bool SendFileProperties(string path, vector<string> patterns) = 0;
    
    virtual vector<string> GetResults() = 0;
    
    virtual size_t addRef(){
            cout<<"addRef in IFileManager being executed"<<endl;
            return Iunknown::addRef();
        };
        
        virtual size_t release(){
            cout<<"\nrelease in IFileManager being executed"<<endl;
            return Iunknown::release();
        };
        
    virtual ~IFileManager(){
        cout<< "\n~IFileManager is being executed"<<endl;
    };
};



#endif /* IFILEMANAGER_H */

