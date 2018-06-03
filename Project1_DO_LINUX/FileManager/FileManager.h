/////////////////////////////////////////////////////////////////////////////
//  FileManager.h: This module defines the interface for functionality of		 //
//                      FileManager component                               //
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
//this shared object helps in searching a given directory for set of files
//that match the file extension patterns. It also does reference counting  //
//
/////////////////////////////////////////////////////////////////////////////
//required files:
//===============
//IFileManager.h
/////////////////////////////////////////////////////////////////////////////
//Public INterface:
//=================
//FileManager::FileManager().
//this method creates a FileManager object.
//
//--------------------------------------------------------------------------
//FileManager::SendFileProperties(string path, vector<string> patterns)
//this method can be called to send the directory path to search for and file patterns
//that need to be matched.
//---------------------------------------------------------------------------
//vector<string> GetResults();
//this method is used to retrieve results i.e list of files
//---------------------------------------------------------------------------
//IFileManager* globalCreateFileManager();
//this unmangled global factory method is used to create a FileManager pointer on heap.
/////////////////////////////////////////////////////////////////////////////
//maintenance history: V1.0, 23/04/2018.
//===================================
//NIL
/////////////////////////////////////////////////////////////////////////////

#ifndef FILEMANAGER_H
#define FILEMANAGER_H
#include "IFileManager.h"
#include <vector>
#include <string>
using namespace std;


class FileManager: public IFileManager{
    
public:
    
    
    FileManager(){
        cout<<"FileManager() being executed"<<endl;
        addRef();
    };
    
    FileManager( FileManager& fM){
        cout<<"FileManager(Fm&) being executed"<<endl;
     /*   this->outputFileList = fM.outputFileList;
       // fM.addRef();
        *this = fM; */
    };
    
    FileManager& operator=( FileManager& fM){
        cout<<"FileManager operator=( fM&) being executed"<<endl;
     /*  // fM.addRef();
        this->outputFileList = fM.outputFileList;
      //  *this = fM;
      //  return (*this); */
        return fM;
    };
 
     bool SendFileProperties(string path, vector<string> patterns);
    
     vector<string> GetResults();
     
     ~FileManager(){
         cout<<"~FileManager() being executed"<<endl;
     };
     
     
     
private:
     void getAllFiles(string& filePath, vector<string>& filePatterns, vector<string>& output);
     std::vector<std::string> getFileListProperty();
     std::vector<std::string> outputFileList;

     
   
};

extern "C" IFileManager* globalCreateFileManager();



#endif /* FILEMANAGER_H */

