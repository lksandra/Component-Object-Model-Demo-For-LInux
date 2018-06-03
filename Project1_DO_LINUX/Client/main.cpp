/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: omkar07
 *
 * Created on 17 April, 2018, 1:33 AM
 */

//#include <cstdlib>
#include "../FileManager/IFileManager.h"
#include "../TextSearchComponent/ITextSearch.h"
#include <dlfcn.h> 
#include <string>
#include <vector>
#include <unistd.h>
#include <exception>
#include <unordered_map>
using namespace std;

/*
 * 
 */


vector<string> getFilePatterns(int argc, char* argv[]) {
	vector<string> output;
	for (int i = 2; i < argc-1; i++) {
		output.push_back(string(argv[i]));
                cout<<"filePattern-"<<i-1<<"-"<<string(argv[i])<<endl;

	};
	return output;
};

int main(int argc, char** argv) {
    cout<<"\ndemonstrataed R6 by starting the project using bash script"<<endl;
    cout<<"================================================================="<<endl;
    cout<<"\nDemonstrating R2, R3"<<endl;
    cout<<"================================================================="<<endl;
    
    cout<<"\nDirectory file path is:\n"<< argv[1]<<endl;
    
    cout<<"\nfile patterns are:\n"<<argv[2]<<endl<<endl;
    vector<string> patterns = getFilePatterns(argc, argv);
    cout<<"================================================================="<<endl;
    
    cout<<"\nDemonstrating R4: loading textSearch component"<<endl;
    void* handleToTextSearch = dlopen("libtextSearch.so", RTLD_NOW);
     cout<<"================================================================="<<endl;
   
    cout<<"\nDemonstrating R5: loading fileManager component"<<endl;
    cout<<"================================================================="<<endl;
   
    void* handle = dlopen("libFileManager.so", RTLD_NOW);
  if (!handle)
  {
    std::cout << "\n  failed to load libFileManager.so\n\n"<< dlerror() <<endl;
    return 1;
  }
  typedef IFileManager*(*GCreate)();
  GCreate gCreate = (GCreate)dlsym(handle, "globalCreateFileManager");
  if (!gCreate)
  {
    std::cout << dlerror()<<std::endl;
    return 0;
  }

  cout<<"\nAttempting to create object of fileManager component"<<endl;
  IFileManager* pTest = gCreate();
  
  cout<<"\nattempting to send file path and patterns to fileManager component"<<endl;
  if (pTest->SendFileProperties(argv[1], patterns))
    std::cout << "\n\nclient successfully sent file path and patterns to fileManager component";
  else
    std::cout << "\n client's attempt to send file proeprties to fileManager compoentn failerd"<<endl;

 
  std::cout << "\n\n"; 
  vector<string> tempResultsFromFm;
  
  try{
      cout<<"\nDemonstrating R5:"<<endl;
      cout<<"============================================="<<endl;
      cout<<"\nclient: trying to get results from fileManager"<<endl;
      tempResultsFromFm = pTest->GetResults();
      cout<<"\nsize of the results: "<< tempResultsFromFm.size() <<endl;
      cout<<"\nclient: successfully retrieved results from fileManager"<<endl;
  }catch(exception e){
      cout<< e.what()<<endl;
      cout<< "\nclient: unsuccess attempt to retrieve results from fileManager"<<endl;
  };
  
  
  
  
  
  cout<<"\nDemonstrating R4 by checking if the textSearch library is loaded"<<endl;
  if (!handleToTextSearch)
  {
    std::cout << "\n  failed to load libtextSearch.so\n\n"<< dlerror() <<endl;
    return 0;
  }else{
      cout<<"\n successfully loaded textSearch library"<<endl;
  };
  typedef ITextSearch*(*HCreate)();
  HCreate hCreate = (HCreate)dlsym(handleToTextSearch, "globalCreateTextSearch");
  if (!hCreate)
  {
    std::cout << dlerror()<<std::endl;
    return 0;
  };
  
  cout<<"\nattempting to create an object of textSearch component"<<endl;
  ITextSearch* pTextSearch = hCreate();
 
  
  
  if(pTextSearch->sendFileListAndSearchString(tempResultsFromFm, argv[argc-1] )){
      cout<<"\n client successfully send FileList and SearchString to textSearch component"<<endl;
  }else{
      cout<<"\nclient could not send filelist and searchstring to textSearch"<<endl;
  };
  
  cout<<"\n client: trying to get results from the textSearch"<<endl;
  unordered_map<string, string> lineNumbers;
  if(pTextSearch->getResults(lineNumbers)){
      cout<<"client-successfully retrieved results from textSearch. size of results: "<< lineNumbers.size()<<endl;
      for(auto each : lineNumbers){
          cout<<"file name"<<endl;
          cout<<each.first<<endl;
          cout<<"linenumber string"<<endl;
          cout<< each.second<<endl;
      };
  }else{
    cout<<"client-Unsuccessful attempt tor retrieve results from textSearch "<<endl; 
  
  };
  
  cout<<"\nDemonstrating reference counting by creating a new pointer and calling addRef"<<endl;
  cout<<"================================================================="<<endl;
   
  IFileManager* pTest2;
  cout<<"\ncalling pTest2->addRef()"<<endl;
  
  pTest2 = pTest;
  pTest2->addRef();
  
      
  
 
  int refFm;
  if((refFm=pTest->release())==0){
      cout<<"\nunloading FileManager component"<<endl;
      dlclose(handle);
  }else{
      cout<<"\nnot unloading FileManager component as RefCount: "<< refFm<<endl;
  };
  
  int refTs;
  
  if((refTs=pTextSearch->release())==0){
       cout<<"\nunloading TextSearch component"<<endl;
      dlclose(handleToTextSearch);
  }else{
      cout<<"\nnot unloading TextSearch component as RefCount: "<< refTs<<endl;
  };
  
  
  
    return 0;
};





