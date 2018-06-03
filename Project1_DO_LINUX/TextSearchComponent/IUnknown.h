/////////////////////////////////////////////////////////////////////////////
//  IUknown.h:   This module defines the primary interface for
//                  reference counting.                                      //
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
//this interface provides ownership management of objects.
//
/////////////////////////////////////////////////////////////////////////////
//required files:
//===============
//NIL
/////////////////////////////////////////////////////////////////////////////
//Public INterface:
//=================
//Iunknown::Iunknown().
//this method creates a Iunknown object and initiates reference counting
//
//--------------------------------------------------------------------------
//virtual size_t addRef();
//this already implemented method is called by any user who creates a pointer to
//IFileManager. returns the number of shared owners.
//---------------------------------------------------------------------------
//virtual size_t release();
//this method is called by every client for destructing its ownerhsip.
//returns the number of shared owners.
//---------------------------------------------------------------------------
//virtual ~Iunknown();
/////////////////////////////////////////////////////////////////////////////
//maintenance history: V1.0, 23/04/2018.
//===================================
//NIL
/////////////////////////////////////////////////////////////////////////////

#ifndef IUNKNOWN_H
#define IUNKNOWN_H
#include <atomic>
#include <iostream>
using namespace std;

class Iunknown{
    
public:
    
   /* virtual bool addRef() = 0;
    virtual bool release() = 0;
    
    virtual ~Iunknown(){
        
    }*/
    
    Iunknown(){
        cout<<"Iunknown() being executed"<<endl;
        refCount.store(0);
    };
    
    //client needs to call this when an object or pointer is created.
    virtual  size_t addRef(){
         cout<<"\naddRef() in Iunknown being executed"<<endl;
         cout<<"refcount before increment: "<< refCount<<endl;
        ++refCount;
        return refCount.load();
    };
    ////client needs to call this for relinquishing the ownerhip.
    virtual  size_t release(){
        cout<<"\nrelease in Iunknown being executed"<<endl;
        cout<<"refcount before decrement: "<< refCount<<endl;
        --refCount;
        if(refCount.load()==0){
            delete this;
            return 0;
        }
        return refCount.load();
    };
    
    virtual ~Iunknown(){
        cout<<"\n~Iunknown is being executed"<<endl;
    };
    
private:
    atomic<size_t> refCount;
};


#endif /* IUNKNOWN_H */

