#ifndef FILESYSTEM_H
#define FILESYSTEM_H
/////////////////////////////////////////////////////////////////////////////
// FileSystem.h - Support file and directory operations                    //
// Linux ver 1.1                                                           //
// Language:    gcc/g++ v4.6.1                                             //
// Platform:    Dell XPS 8300, Core i7, kubuntu v11.10                     //
// Application: Summer Projects, 2012                                      //
// Author:      Jim Fawcett, CST 4-187, Syracuse University                //
//              (315) 443-3948, jfawcett@twcny.rr.com                      //
/////////////////////////////////////////////////////////////////////////////
/*
 * Module Operations:
 * ==================
 * This package provides the namespace FileSystem containing:
 * classes: File, FileInfo, Path, and Directory.
 *
 * The File class supports opening text and binary files for either input 
 * or output.  File objects have names, get and put lines of text, get and
 * put blocks of bytes if binary, can be tested for operational state,
 * cleared of errors, and output File objects can be flushed to their streams.
 *
 * FileInfo class accepts a fully qualified filespec and supports queries
 * concerning name, time and date, size, and attributes.  You can compare
 * FileInfo objects by name, date, and size.
 *
 * Path class provides static methods to turn a relative filespec into an
 * absolute filespec, return the path, name, or extension of the filespec,
 * and build filespecs from path and name.  Path also provides methods to
 * convert filespecs to uppercase and to lowercase.
 *
 * The Directory class supports getting filenames or directories from a 
 * fully qualified filespec, e.g., path + filename pattern using static
 * methods.  It also provides non-static methods to get and set the current
 * directory.
 *
 * Public Interface:
 * =================
 * File f(filespec,File::in,File::binary);
 * File g(filespec,File::out,File::binary);
 * while(f.isGood()) {
 *   block b = f.getBlock();
 *   g.putBlock(b);
 * }
 * FileInfo fi("..\foobar.txt");
 * if(fi.good())
 *   ...
 * std::string filespec = "..\temp.txt";
 * std::string fullyqualified = Path::getFullFileSpec(filespec);
 * std::string path = Path::getPath(fullyqualified);
 * std::string name = Path::getName(fullyqualified);
 * std::string extn = Path::getExt(fullyqualified);
 *
 * Directory d;
 * std::string dir = Directory::getCurrentDirectory();
 * Directory::setCurrentDirectory(dir);
 * std::vector<std::string> files = Directory::getFiles(path, pattern);
 * std::vector<std::string> dirs = Directory::getDirectories(path);
 * 
 * Required Files:
 * ===============
 * FileSystem.h, FileSystem.cpp
 *
 * Build Command:
 * ==============
 * g++ FileSystem.cpp
 *
 * Maintenance History:
 * ====================
 * ver 1.2 : 21 Jan 2015
 * - changed teststub contents to match new directory structure
 * - minor cleanup of code
 * ver 1.1 : 23 Feb 2013
 * - fixed bug in Path::getExt found by Yang Zhou and Kevin Kong
 * ver 1.0 : 06 Jun 2012
 * - first release
 */
#include <fstream>
#include <string>
#include <vector>
#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/stat.h>
#include <sys/sendfile.h>

namespace FileSystem
{
  /////////////////////////////////////////////////////////
  // Block
  
  typedef char Byte;

  class Block
  {
  public:
    Block(size_t size=0) : bytes_(size) {}
    Block(Byte* beg, Byte* end);
    void push_back(Byte b);
    Byte& operator[](size_t i);
    Byte operator[](size_t i) const;
    bool operator==(const Block&) const;
    bool operator!=(const Block&) const;
    size_t size() const;
  private:
    std::vector<Byte> bytes_;
  };

  /////////////////////////////////////////////////////////
  // File
  
  class File
  {
  public:
    enum direction { in, out };
    enum type { text, binary };
    File(const std::string& filespec);
    bool open(direction dirn, type typ=File::text);
    ~File();
    std::string name();
    std::string getLine();
    void putLine(const std::string& line, bool wantReturn=true);
    Block getBlock(size_t size);
    void putBlock(const Block&);
    bool isGood();
    void clear();
    void flush();
    void close();
    static bool exists(const std::string& file);
    static bool copy(const std::string& src, const std::string& dst, bool failIfExists=false);
    static bool remove(const std::string& filespec);
  private:
    std::string name_;
    std::ifstream* pIStream;
    std::ofstream* pOStream;
    direction dirn_;
    type typ_;
    bool good_;
  };

  inline std::string File::name() { return name_; }

  /////////////////////////////////////////////////////////
  // FileInfo

  class FileInfo
  {
  public:
    enum dateFormat { fullformat, timeformat, dateformat };
    FileInfo(const std::string& fileSpec);
    bool good();
    std::string name() const;
    std::string date(dateFormat df=fullformat) const;
    size_t size() const;
    
    bool isArchive() const;
    bool isCompressed() const;
    bool isDirectory() const;
    bool isEncrypted() const;
    bool isHidden() const;
    bool isNormal() const;
    bool isOffLine() const;
    bool isReadOnly() const;
    bool isSystem() const;
    bool isTemporary() const;

    bool operator<(const FileInfo& fi) const;
    bool operator==(const FileInfo& fi) const;
    bool operator>(const FileInfo& fi) const;
    bool earlier(const FileInfo& fi) const;
    bool later(const FileInfo& fi) const;
    bool smaller(const FileInfo& fi) const;
    bool larger(const FileInfo& fi) const;
  private:
    bool error_;
    static std::string intToString(long i);
    struct stat statbuf_;
    std::string fileSpec_;
  };

  /////////////////////////////////////////////////////////
  // Path

  class Path
  {
  public:
    static std::string getFullFileSpec(const std::string& fileSpec);
    static std::string getPath(const std::string& fileSpec);
    static std::string getName(const std::string& fileSpec, bool withExt=true);
    static std::string getExt(const std::string& fileSpec);
    static std::string fileSpec(const std::string& path, const std::string& name);
    static std::string toLower(const std::string& src);
    static std::string toUpper(const std::string& src);
  };
  
  /////////////////////////////////////////////////////////
  // Directory

  class Directory
  {
  public:
    static bool create(const std::string& path);
    static bool remove(const std::string& path);
    static bool exists(const std::string& path);
    static std::string getCurrentDirectory();
    static bool setCurrentDirectory(const std::string& path);
    static std::vector<std::string> getFiles(const std::string& path=".", const std::string& pattern="*.*");
    static std::vector<std::string> getDirectories(const std::string& path=".", const std::string& pattern="*.*");
  private:
    static const int BufSize = 255;
    char buffer[BufSize];
  };
}

#endif
