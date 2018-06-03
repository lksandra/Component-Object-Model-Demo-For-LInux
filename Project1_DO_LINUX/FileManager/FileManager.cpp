/////////////////////////////////////////////////////////////////////////////
//  FileManager.cpp: This module implements the interface for functionality of		 //
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
//this implementation for the shared object helps in searching a given 
//directory for set of files that match the file extension patterns. It 
//also does reference counting.
//
/////////////////////////////////////////////////////////////////////////////
//required files:
//===============
//FileManager.h
//FileSystem.h
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

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "FileSystem.h"
#include "FileManager.h"
#include <exception>

using namespace std;

//retrieves all the macthing files recursively in a given directory.
void FileManager::getAllFiles(string& filePath, vector<string>& filePatterns, vector<string>& output) {
	if (filePath.length() != 0) {
		FileSystem::Directory::setCurrentDirectory(filePath);
            /*	cout << "filePath is:  " << endl;
		cout << filePath << endl;
		cout << "new currentdir is: " << endl;
		cout << FileSystem::Directory::getCurrentDirectory() << endl; */
		vector<string> allFiles = FileSystem::Directory::getFiles();
		//vector<string> filesForEachPattern;
		for (int i=0; i< allFiles.size(); i++) {
			string fullPathOfFile = FileSystem::Path::getFullFileSpec(allFiles[i].c_str())+"/"+allFiles[i];
			string extension = "*." + FileSystem::Path::getExt(allFiles[i]);
			if (find(filePatterns.begin(), filePatterns.end(), extension) != filePatterns.end()) {
				output.push_back(fullPathOfFile);
			};
		};

		vector<string> subDir = FileSystem::Directory::getDirectories();
		for (int i = 0; i < (int)subDir.size(); i++) {
			string dirFullSpec = FileSystem::Path::getFullFileSpec(subDir[i].c_str())+"/"+subDir[i];
			//	replace_if(dirFullSpec.begin(), dirFullSpec.end(), frontSlash, '/');
			getAllFiles(dirFullSpec, filePatterns, output);
			FileSystem::Directory::setCurrentDirectory(filePath);
		};
	};


};

  
 //receives file properties viz. directory search path. file patterns
  bool FileManager::SendFileProperties(string path, vector<string> patterns){
         
	cout << "path string is:\n" << path << endl;

	vector<string> output;
	if (patterns.size() > 0) {
		
		getAllFiles(path, patterns, output);
		if (output.size() > 0) {
			cout<<"\ninside FileManager: files filtered success" <<endl;
		}
		else {
			cout << "\ninside FileManager: files NOT filtered" << endl;
		}
	}
	else {
		cout << "\ninside FileManager: filePatterns transfer failed" << endl;
	};

	for (int i=0; i<output.size(); i++) {
		cout << output[i] << endl;
	};
	this->outputFileList = output;

	
      return true;
  }
    //client method to retireve results. 
  vector<string> FileManager::GetResults(){
      std::cout<<"\ninside FileManager: get results function of FileManager component called";
      try{
          
          return this->getFileListProperty();
          cout<< "\ninside FileManager: successfully returned from getResults of filemanager"<<endl;
      }catch(exception& e){
          cout<<"\ninside FileManager: some error in getResults of filemanager"<<endl;
          cout<< e.what() << endl;
      }
      
  }
  
  //helper method to retrieve results.
  vector<std::string> FileManager::getFileListProperty(){
      try{
          return this->outputFileList;
      }catch(exception& e){
          cout<< e.what() << endl;
      }
      
  }
  
  //implements a unmangled global factory method
IFileManager* globalCreateFileManager()
{
  return new FileManager;
}

