/////////////////////////////////////////////////////////////////////////////
// FileSystem.cpp - Support file and directory operations                  //
// Linux ver 1.1                                                           //
// Language:    gcc/g++ v4.6.1                                             //
// Platform:    Dell XPS 8300, Core i7, kubuntu v11.10                     //
// Application: Summer Projects, 2012                                      //
// Author:      Jim Fawcett, CST 4-187, Syracuse University                //
//              (315) 443-3948, jfawcett@twcny.rr.com                      //
/////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <stdexcept>
#include <cstdio>
//#include <regex>  // not complete for g++
#include <stdlib.h>
#include <time.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include "FileSystem.h"

using namespace FileSystem;

//----< block constructor taking array iterators >-------------------------

Block::Block(Byte* beg, Byte* end) : bytes_(beg, end) {}

//----< push back block byte >---------------------------------------------

void Block::push_back(Byte b)
{
  bytes_.push_back(b);
}
//----< non-const indexer >------------------------------------------------

Byte& Block::operator[](size_t i)
{
  if(i<0 || bytes_.size() <= i)
    throw std::runtime_error("index out of range in Block");
  return bytes_[i];
}
//----< const indexer >----------------------------------------------------

Byte Block::operator[](size_t i) const
{
  if(i<0 || bytes_.size() <= i)
    throw std::runtime_error("index out of range in Block");
  return bytes_[i];
}
//----< equality comparison >----------------------------------------------

bool Block::operator==(const Block& block) const
{
  return bytes_ == block.bytes_;
}
//----< inequality comparison >--------------------------------------------

bool Block::operator!=(const Block& block) const
{
  return bytes_ != block.bytes_;
}
//----< return number of bytes in block >----------------------------------

size_t Block::size() const
{
  return bytes_.size();
}

//----< File constructor opens file stream >-------------------------------

File::File(const std::string& filespec) 
    : name_(filespec), pIStream(0), pOStream(0), dirn_(in), typ_(text), good_(true)
{
}
//----< File destructor closes file stream >-------------------------------

File::~File() 
{ 
  if(pIStream)
  {
    pIStream->close(); 
    delete pIStream;
  }
  if(pOStream)
  {
    pOStream->close();
    delete pOStream; 
  }
}
//----< open for reading or writing >--------------------------------------

bool File::open(direction dirn, type typ)
{
  dirn_ = dirn;
  typ_ = typ;
  if(dirn == in)
  {
    pIStream = new std::ifstream;
    if(typ == binary)
      pIStream->open(name_.c_str(), std::ios::in | std::ios::binary);
    else
      pIStream->open(name_.c_str(), std::ios::in);
    if(!(*pIStream))
      good_ = false;
      //throw std::runtime_error("\n  open for input failed in File constructor");
  }
  else
  {
    pOStream = new std::ofstream;
    if(typ == binary)
      pOStream->open(name_.c_str(), std::ios::out | std::ios::binary);
    else
      pOStream->open(name_.c_str(), std::ios::out);
    if(!(*pOStream))
      good_ = false;
      //throw std::runtime_error("\n  open for output failed in File constructor");
  }
  return good_;
}
//----< reads one line of a text file >------------------------------------

std::string File::getLine()
{
  if(pIStream == 0 || !pIStream->good())
    throw std::runtime_error("input stream not open");
  if(typ_ == binary)
    throw std::runtime_error("getting text line from binary file");
  if(dirn_ == out)
    throw std::runtime_error("reading output file");
  const int BufSize = 255;
  char buffer[BufSize];
  pIStream->getline(buffer,BufSize);
  return std::string(buffer);
}
//----< writes one line of a text to a file >------------------------------

void File::putLine(const std::string& s, bool wantReturn)
{
  if(pOStream == 0 || !pOStream->good())
    throw std::runtime_error("output stream not open");
  if(typ_ == binary)
    throw std::runtime_error("writing text line to binary file");
  if(dirn_ == in)
    throw std::runtime_error("writing input file");
  for(size_t i=0; i<s.size(); ++i)
    pOStream->put(s[i]);
  if(wantReturn)
    pOStream->put('\n');
  pOStream->flush();
}
//----< reads a block of bytes from binary file >--------------------------

Block File::getBlock(size_t size)
{
  if(pIStream == 0 || !pIStream->good())
    throw std::runtime_error("input stream not open");
  if(typ_ != binary)
    throw std::runtime_error("reading binary from text file");
  if(dirn_ == out)
    throw std::runtime_error("reading output file");
  Block blk;
  if(pIStream)
  {
    for(size_t i=0; i<size; ++i)
    {
      Byte b;
      pIStream->get(b);
      if(pIStream->good())
        blk.push_back(b);
      else
        break;
    }
  }
  return blk;
}
//----< writes a block of bytes to binary file >---------------------------

void File::putBlock(const Block& blk)
{
  if(pOStream == 0 || !pOStream->good())
    throw std::runtime_error("output stream not open");
  if(typ_ != binary)
    throw std::runtime_error("writing binary to text file");
  if(dirn_ == in)
    throw std::runtime_error("writing input file");
  if(!pOStream->good())
    return;
  for(size_t i=0; i<blk.size(); ++i)
  {
    pOStream->put(blk[i]);
  }
}
//----< tests for error free stream state >--------------------------------

bool File::isGood()
{
  if(!good_)
    return false;
  if(pIStream)
    return (good_ = pIStream->good());
  if(pOStream)
    return (good_ = pOStream->good());
  return (good_ = false);
}
//----< flushes output stream to its file >--------------------------------

void File::flush()
{
  if(pOStream)
    pOStream->flush();
  if(pOStream)
    return;
}
//----< clears error state enabling operations again >---------------------

void File::clear()
{
  if(pIStream)
    pIStream->clear();
  if(pOStream)
    pOStream->clear();
}
//----< close file handle >------------------------------------------------

void File::close()
{
  File::flush();
  if(pIStream)
    pIStream->close();
  if(pOStream)
    pOStream->close();
}
//----< file exists >--------------------------------------------------

bool File::exists(const std::string& file)
{
  bool retval;
  std::ifstream test(file.c_str(), std::ios::in);
  retval = test.good();
  test.close();
  return retval;
}
//----< copy file >----------------------------------------------------

bool File::copy(const std::string& src, const std::string& dst, bool failIfExists)
{
  if(failIfExists && exists(dst))
    return false;
  int read_fd, write_fd;
  struct stat stat_buf;
  off_t offset = 0;

  /* Open the input file. */
  read_fd = ::open (src.c_str(), O_RDONLY);
  if(read_fd == -1)
    return false;
  /* Stat the input file to obtain its size. */
  fstat (read_fd, &stat_buf);
  /* Open the output file for writing, with the same permissions as the
    source file. */
  write_fd = ::open (dst.c_str(), O_WRONLY | O_CREAT, stat_buf.st_mode);
  if(write_fd == -1)
    return false;
  /* Blast the bytes from one file to the other. */
  sendfile (write_fd, read_fd, &offset, stat_buf.st_size);
  /* Close up. */
  ::close (read_fd);
  ::close (write_fd);
  return true;
}
//----< remove file >--------------------------------------------------

bool File::remove(const std::string& file)
{
  return std::remove(file.c_str()) != -1;
}
//----< constructor >--------------------------------------------------

FileInfo::FileInfo(const std::string& fileSpec)
{
  fileSpec_ = fileSpec;
  int status = stat(fileSpec.c_str(), &statbuf_);
  if(status != 0)
  {
    error_ = true;
    std::cout << strerror(errno);
  }
  else
    error_ = false;
}
//----< is passed filespec valid? >------------------------------------

bool FileInfo::good()
{
  return !error_;
}
//----< return file name >---------------------------------------------

std::string FileInfo::name() const
{
  return Path::getName(fileSpec_);
}
//----< conversion helper >--------------------------------------------

std::string FileInfo::intToString(long i)
{
  std::ostringstream out;
  out.fill('0');
  out << std::setw(2) << i;
  return out.str();
}
//----< return file date >---------------------------------------------

std::string FileInfo::date(dateFormat df) const
{
  struct timespec filetime = statbuf_.st_mtim;
  tm* pTm = localtime(&filetime.tv_sec);
  std::string dateStr, timeStr;
  dateStr = intToString(1+pTm->tm_mon)
          + '/' + intToString(pTm->tm_mday) 
          + '/' + intToString(1900+pTm->tm_year);
  timeStr = intToString(pTm->tm_hour)
          + ':' + intToString(pTm->tm_min)
          + ':' + intToString(pTm->tm_sec);
  if(df == dateformat)
    return dateStr;
  if(df == timeformat)
    return timeStr;
  return dateStr + " " + timeStr;
}
//----< return file size >---------------------------------------------

size_t FileInfo::size() const
{
  return (size_t)(statbuf_.st_size);
}
//----< is type archive? >---------------------------------------------

bool FileInfo::isArchive() const
{
  return false;
}
//----< is type compressed? >------------------------------------------

bool FileInfo::isCompressed() const
{
  return false;
}
//----< is type directory? >-------------------------------------------

bool FileInfo::isDirectory() const
{
  return S_ISDIR(statbuf_.st_mode);
}
//----< is type encrypted? >---------------------------------------------

bool FileInfo::isEncrypted() const
{
  return false;
}
//----< is type hiddent? >---------------------------------------------

bool FileInfo::isHidden() const
{
  return Path::getName(fileSpec_)[0] == '.';
}
//----< is type normal? >---------------------------------------------

bool FileInfo::isNormal() const
{
  return S_ISREG(statbuf_.st_mode);
}
//----< is type offline? >---------------------------------------------

bool FileInfo::isOffLine() const
{
  return false;
}
//----< is type readonly? >--------------------------------------------

bool FileInfo::isReadOnly() const
{
  File temp("./temp");
  return !temp.isGood();
}
//----< is type system? >----------------------------------------------

bool FileInfo::isSystem() const
{
  return false;
}
//----< is type temporary? >-------------------------------------------

bool FileInfo::isTemporary() const
{
  return false;
}
//----< compare names alphabetically >---------------------------------

bool FileInfo::operator<(const FileInfo& fi) const
{
  return Path::getName(fileSpec_) < Path::getName(fi.fileSpec_);
}
//----< compare names alphabetically >---------------------------------

bool FileInfo::operator>(const FileInfo& fi) const
{
  return Path::getName(fileSpec_) > Path::getName(fi.fileSpec_);
}
//----< compare names alphabetically >---------------------------------

bool FileInfo::operator==(const FileInfo& fi) const
{
  return Path::getName(fileSpec_) == Path::getName(fi.fileSpec_);
}
//----< compare file times >-------------------------------------------

bool FileInfo::earlier(const FileInfo& fi) const
{
  return statbuf_.st_mtime < fi.statbuf_.st_mtime;
}
//----< compare file times >-------------------------------------------

bool FileInfo::later(const FileInfo& fi) const
{
  return statbuf_.st_mtime > fi.statbuf_.st_mtime;
}
//----< smaller >------------------------------------------------------

bool FileInfo::smaller(const FileInfo &fi) const
{
  return size() < fi.size();
}
//----< larger >-------------------------------------------------------

bool FileInfo::larger(const FileInfo &fi) const
{
  return size() > fi.size();
}
//----< convert string to lower case chars >---------------------------

std::string Path::toLower(const std::string& src)
{
  std::string temp;
  for(size_t i=0; i<src.length(); ++i)
    temp += tolower(src[i]);
  return temp;
}
//----< convert string to upper case chars >---------------------------

  
std::string Path::toUpper(const std::string& src)
{
  std::string temp;
  for(size_t i=0; i<src.length(); ++i)
    temp += toupper(src[i]);
  return temp;
}
//----< get path from fileSpec >---------------------------------------

std::string Path::getName(const std::string &fileSpec, bool withExt)
{
  size_t pos = fileSpec.find_last_of("/");
  if(pos >= fileSpec.length())
    pos = fileSpec.find_last_of("\\");
  if(pos >= fileSpec.length())
  {
    // no path prepended
    if(withExt)
      return fileSpec;
    else
    {
      // remove ext
      size_t pos = fileSpec.find(".");
      if(pos > fileSpec.size())
        return fileSpec;
      return fileSpec.substr(0, pos-1);
    }
  }
  size_t end = fileSpec.length() - pos;
  std::string name = fileSpec.substr(pos+1,fileSpec.length()-pos);
  if(withExt)
    return name;
  pos = name.find(".");
  return name.substr(0,pos);
}
//----< get extension from fileSpec >----------------------------------
  
std::string Path::getExt(const std::string& fileSpec)
{
  size_t pos1 = fileSpec.find_last_of('/');
  size_t pos2 = fileSpec.find_last_of('\\');
  size_t pos = fileSpec.find_last_of('.');
  // handle ../ or ..\\ with no extension
  if(pos1 < fileSpec.length() || pos2 < fileSpec.length())
  {
    if(pos < std::min(pos1, pos2))
      return std::string("");
  }
  // only . is extension delimiter
  if(0 <= pos && pos < fileSpec.length())
    return toLower(fileSpec.substr(pos+1,fileSpec.length()-pos));
  return std::string("");
}
//----< get path from fileSpec >---------------------------------------

std::string Path::getPath(const std::string &fileSpec)
{
  size_t pos = fileSpec.find_last_of("/");
  if(pos >= fileSpec.length())
    pos = fileSpec.find_last_of("\\");
  if(pos >= fileSpec.length())
    return ".";
  if(fileSpec.find(".",pos+1))
    return toLower(fileSpec.substr(0,pos+1));
  return fileSpec;
}
//----< get absoluth path from fileSpec >------------------------------

std::string Path::getFullFileSpec(const std::string &fileSpec)
{
  char buffer[PATH_MAX];
  const std::string fullPath
    = realpath(getPath(fileSpec.c_str()).c_str(), buffer);
  //std::string name = Path::getName(fileSpec);
  //std::string path = Path::fileSpec(fullPath, name);
  if(fullPath[fullPath.size()-1] == '.')
    return fullPath.substr(0, fullPath.size()-2);
  return fullPath;
}
//----< create file spec from path and name >--------------------------

std::string Path::fileSpec(const std::string &path, const std::string &name)
{
  std::string fs;
  size_t len = path.size();
  if(path[len-1] == '/' || path[len-1] == '\\')
    fs = path + name;
  else
  {
    if(path.find("/") < path.size())
      fs = path + "/" + name;
    else if(path.find("\\") < path.size())
      fs = path + "\\" + name;
    else
      fs = path + "/" + name;
  }
  return fs;
}
//----< create a directory >-----------------------------------------------

bool Directory::create(const std::string& path)
{
  mode_t mode = 0777;
  struct stat statbuf;
  int    status = 0;

  if (stat(path.c_str(), &statbuf) != 0)
  {
      /* Directory does not exist */
      if (mkdir(path.c_str(), mode) != 0)
          status = -1;
      else
        status = chmod(path.c_str(), mode);
  }
  else if (!S_ISDIR(statbuf.st_mode))
  {
      errno = ENOTDIR;
      status = -1;
  }
  return(status != -1);
}
//----< create a directory >-----------------------------------------------

bool Directory::remove(const std::string& path)
{
  mode_t mode = S_IRUSR;
  struct stat statbuf;
  int    status = 0;

  if (stat(path.c_str(), &statbuf) == 0)
  {
      /* Directory exists */
      if (rmdir(path.c_str()) != 0)
          status = -1;
  }
  else if (!S_ISDIR(statbuf.st_mode))
  {
      errno = ENOTDIR;
      status = -1;
  }
  return(status != -1);  
}
//----< create a directory >-----------------------------------------------

bool Directory::exists(const std::string& path)
{
  struct stat statbuf;
  return stat(path.c_str(), &statbuf) == 0;
}
//----< return name of the current directory >-----------------------------

std::string Directory::getCurrentDirectory()
{
  char buffer[PATH_MAX];
  std::string path = getcwd(buffer,PATH_MAX);
  if(path[path.size()-1] == '.')
    return path.substr(0,path.size()-2);
  return path;
}
//----< change the current directory to path >-----------------------------

bool Directory::setCurrentDirectory(const std::string& path)
{
  return chdir(path.c_str()) == 0;
  //std::string fullPath = Path::getFullFileSpec(path);
  //int status = chdir(fullPath.c_str());
}
//----< check match - incomplete >-----------------------------------------
/*
 * Can correctly resolve only the wild cards:
 *   "*.*", "*.someExt", and "someName.*"
 * Will complete when g++ finishes C++11 regex package.
 */
bool match(const std::string& input, const std::string& pattern)
{
  // exact match or "*.*"
  if(input == pattern || pattern == "*.*")
    return true;
  // attempting to match entire extension pattern like *.cpp
  size_t pos = pattern.find("*.");
  if(pos < pattern.size())
  {
    std::string ext_in = Path::getExt(input);
    std::string ext_pt = Path::getExt(pattern);
    if(ext_in == ext_pt)
      return true;
  }
  // attempting to match entire name pattern like foo.*
  pos = pattern.find(".*");
  if(pos < pattern.size())
  {
    // return name without extension
    std::string name_in = Path::getName(input, false);
    std::string name_pt = Path::getName(pattern, false);
    if(name_in == name_pt)
      return true;
  }  
  return false;
}
//----< get names of all the files matching pattern (path:name) >----------

std::vector<std::string> 
Directory::getFiles(const std::string& path, const std::string& pattern)
{
  std::vector<std::string> files;
  DIR* dir = opendir(path.c_str());
  if(dir == 0)
    return files;
  dirent* dp;
  while(true)
  {
    dp = readdir(dir);
    if(dp == 0)
      return files;
    if(dp->d_type == 8)
    {
      std::string name = Path::getName(dp->d_name);
      if(match(name,pattern))
        files.push_back(dp->d_name);
    }
  }
  return files;
}
//----< get names of all directories matching pattern (path:name) >--------

std::vector<std::string> 
Directory::getDirectories(const std::string& path, const std::string& pattern)
{
  std::vector<std::string> dirs;
  std::vector<std::string> files;
  DIR* dir = opendir(path.c_str());
  if(dir == 0)
    return files;
  dirent* dp;
  while(true)
  {
    dp = readdir(dir);
    if(dp == 0)
      return files;
    if(dp->d_type == 4)
    {
      if(dp->d_name[0] == '.' && dp->d_name[1] == 0)
        continue;  // "."
      if(dp->d_name[0] == '.' && dp->d_name[1] == '.' && dp->d_name[2] == 0 )
        continue;  // ".."
      files.push_back(dp->d_name);
    }
  }
  return dirs;
}
//----< test stub >--------------------------------------------------------

#ifdef TEST_FILESYSTEM

void title(const std::string& title, char ch='=')
{
  std::cout << "\n  " << title;
  std::cout << "\n " << std::string(title.size()+2, ch);
}
int main(int argc, char* argv[])
{
  title("Demonstrate Path Class");

  std::string fs = Path::fileSpec(".","temp.txt");
  std::cout << "\n  Path::fileSpec(\".\",\"temp.txt\") = " << fs;

  std::string path = Path::getPath(fs);
  std::cout << "\n  Path::getPath(\"" + fs + "\") = " << path;
  
  std::string ffs = Path::getFullFileSpec(fs);
  std::cout << "\n  Path::getFullFileSpec(\"" + fs + "\") = " << ffs;
  
  std::string name = Path::getName(fs);
  std::cout << "\n  Path::getName(\"" + fs + "\") = " << name;

  std::string ext = Path::getExt(fs);
  std::cout << "\n  Path::getExt(\"" + fs + "\") = " << ext;

  std::string upper = Path::toUpper("temp.txt");
  std::cout << "\n  Path::toUpper(\"temp.txt\") = " << upper;

  std::string lower = Path::toLower("Temp.Txt");
  std::cout << "\n  Path::toLower(\"Temp.Txt\") = " << lower;
  std::cout << std::endl;

  title("Demonstrate Directory class");

  // Display contents of current directory

  Directory d;
  std::cout << "\n  current directory is:\n    " << d.getCurrentDirectory();
  std::cout << "\n  It contains files:";
  std::vector<std::string> currfiles = Directory::getFiles();
  ///////////////////////////////////////////////////////
  // This works too
  // std::vector<std::string> currfiles = d.getFiles();  
  for(size_t i=0; i<currfiles.size(); ++i)
    std::cout << "\n    " << currfiles[i].c_str();
  std::cout << "\n  and contains directories:";
  std::vector<std::string> currdirs = Directory::getDirectories();
  for(size_t i=0; i<currdirs.size(); ++i)
    std::cout << "\n    " << currdirs[i].c_str();
  std::cout << "\n";

  // Display contents of non-current directory

  std::string selectedPath = std::string(getenv("HOME"));
  std::cout << "\n  files residing in " + selectedPath;
  currfiles = d.getFiles(selectedPath, "*.*");
  for(size_t i=0; i<currfiles.size(); ++i)
    std::cout << "\n    " << currfiles[i].c_str();
  std::cout << "\n";
  std::cout << "\n  *.txt files residing in " + selectedPath;
  currfiles = d.getFiles(selectedPath, "*.txt");
  for(size_t i=0; i<currfiles.size(); ++i)
    std::cout << "\n    " << currfiles[i].c_str();
  std::cout << "\n";
  std::cout << "\n   FileSystem.* files residing in " + selectedPath;
  currfiles = d.getFiles(selectedPath, "FileSystem.*");
  for(size_t i=0; i<currfiles.size(); ++i)
    std::cout << "\n    " << currfiles[i].c_str();
  std::cout << "\n";
  
  std::cout << "\n  directories residing in " + selectedPath;
  currdirs = d.getDirectories(selectedPath);
  for(size_t i=0; i<currdirs.size(); ++i)
    std::cout << "\n    " << currdirs[i].c_str();
  std::cout << "\n";
  
  title("Demonstrate FileInfo Class Operations", '=');
  std::cout << "\n";

  Directory temp;
  if(temp.setCurrentDirectory("."))
    std::cout << "\n  current path is \"" << temp.getCurrentDirectory() << "\n";
  else
      std::cout << "\n  setCurrentDirectory(\".\") failed" << "\n";
  
  std::string fn1;
  if(argc > 1)
    fn1 = argv[1];
  else
    fn1 = std::string(getenv("HOME")) + "/NetBeansProjects/FileSystemDemo/FileSystem.h";
  
  std::cout << "\n  Test file spec is: " << fn1;
  
  FileInfo fi(fn1);

  if(fi.good())
  {
    std::cout << "\n  name: " << "\t" << fi.name();
    std::cout << "\n  date: " << "\t" << fi.date();
    std::cout << "\n  date: " << "\t" << fi.date(FileInfo::dateformat);
    std::cout << "\n  date: " << "\t" << fi.date(FileInfo::timeformat);
    std::cout << "\n  size: " << "\t" << fi.size() << " bytes";
    if(fi.isArchive())
      std::cout << "\n  is archive";
    else
      std::cout << "\n  is not archive";
    if(fi.isCompressed())
      std::cout << "\n  is compressed";
    else
      std::cout << "\n  is not compressed";
    if(fi.isDirectory())
      std::cout << "\n  is directory";
    else
      std::cout << "\n  is not directory";
    if(fi.isEncrypted())
      std::cout << "\n  is encrypted";
    else
      std::cout << "\n  is not encrypted";
    if(fi.isHidden())
      std::cout << "\n  is hidden";
    else
      std::cout << "\n  is not hidden";
    if(fi.isNormal())
      std::cout << "\n  is normal";
    else
      std::cout << "\n  is not normal";
    if(fi.isOffLine())
      std::cout << "\n  is offline";
    else
      std::cout << "\n  is not offline";
    if(fi.isReadOnly())
      std::cout << "\n  is readonly";
    else
      std::cout << "\n  is not readonly";
    if(fi.isSystem())
      std::cout << "\n  is system";
    else
      std::cout << "\n  is not system";
    if(fi.isTemporary())
      std::cout << "\n  is temporary";
    else
      std::cout << "\n  is not temporary";
  }
  else
    std::cout << "\n  filename " << fn1 << " is not valid in this context\n";

  std::string fn2;
  if(argc > 2)
  {
    fn1 = argv[1];
    fn2 = argv[2];
  }
  else
  {
    fn1 = std::string(getenv("HOME")) + "/NetBeansProjects/FileSystemDemo/FileSystem.h";
    fn2 = std::string(getenv("HOME")) + "/NetBeansProjects/FileSystemDemo/FileSystem.cpp";
  }
  FileInfo fi1(fn1);
  FileInfo fi2(fn2);
  if(fi1.good() && fi2.good())
  {
    if(fi1 == fi1)
      std::cout << "\n  " << fi1.name() << " == " << fi1.name();
    else
      std::cout << "\n  " << fi1.name() << " != " << fi1.name();
    if(fi1 < fi1)
      std::cout << "\n  " << fi1.name() << " < " << fi1.name();
    else
      std::cout << "\n  " << fi1.name() << " >= " << fi1.name();
    if(fi1 == fi2)
      std::cout << "\n  " << fi1.name() << " == " << fi2.name();
    else
      std::cout << "\n  " << fi1.name() << " != " << fi2.name();
    if(fi1 < fi2)
      std::cout << "\n  " << fi1.name() << " < " << fi2.name();
    else
      std::cout << "\n  " << fi1.name() << " >= " << fi2.name();
    if(fi1.smaller(fi2))
      std::cout << "\n  " << fi1.name() << " is smaller than " << fi2.name();
    else
      std::cout << "\n  " << fi1.name() << " is not smaller than " << fi2.name();
    if(fi1.earlier(fi2))
      std::cout << "\n  " << fi1.name() << " is earlier than " << fi2.name();
    else
      std::cout << "\n  " << fi1.name() << " is not earlier than " << fi2.name();
    std::cout << std::endl;
  }
  else
    std::cout << "\n  files " << fn1 << " and " << fn2 << " are not valid in this context\n";
  
  title("Demonstrate File class operations", '=');
  std::cout << "\n";

  // copy binary file from one directory to another

  std::string srcFile = "/build/Debug/GNU-Linux-x86/FileSystem.o";
  std::string src = Directory::getCurrentDirectory() + srcFile;
  std::cout << "\n  src = \"" << src << "\"";
  std::string dstFile = "/copy.o";
  std::string dst = std::string(getenv("HOME")) + "/Documents" + dstFile;
  
  File me(src);
  me.open(File::in, File::binary);
  std::cout << "\n  copying:\n    " << me.name().c_str() << "\n  to:\n    " << dst;
  if(!me.isGood())
  {
    std::cout << "\n  can't open file\n";
    std::cout << "\n  looking for:\n  ";
    std::cout << Path::getFullFileSpec(me.name()) << "\n";
  }
  else
  {
    File you(dst);
    you.open(File::out, File::binary);
    if(you.isGood())
    {
      while(me.isGood())
      {
        Block b = me.getBlock(1024);
        std::cout << "\n    reading block of " << b.size() << " bytes";
        you.putBlock(b);
        std::cout << "\n    writing block of " << b.size() << " bytes"; 
      }
      std::cout << "\n";
    }
    else
    {
      std::cout << "\n  failed to open destination file\n";
    }
  }

  // save some filespecs of text files in a vector for File demonstrations

  std::vector<std::string> files;
  if(argc == 1)
  {
    std::cout << "\n\n  Please enter, on the command line, a filename to process.\n";
  }

  for(int i=1; i<argc; ++i)
  {
    files.push_back(argv[i]);
  }
  files.push_back("FileSystem.cpp");
  files.push_back("FileSystem.h"); // file from project directory
  files.push_back("test.txt");     // file in executable's debug directory
  files.push_back("foobar");       // doesn't exist

  // open each file and display a few lines of text

  for(size_t i=0; i<files.size(); ++i)
  {
    File file(files[i]);
    file.open(File::in);
    if(!file.isGood())
    {
      std::cout << "\n  Can't open file " << file.name();
      std::cout << "\n  Here's what the program can't find:\n  " << Path::getFullFileSpec(file.name()); 
      continue;
    }
    std::string temp = std::string("Processing file ") + files[i];
    title(temp, '-');
    for(int j=0; j<10; ++j)
    {
      if(!file.isGood())
        break;
      std::cout << "\n  " << file.getLine().c_str();
    }
    std::cout << "\n";
  }
  std::cout << "\n";

  // read text file and write to another text file

  title("writing to anothertest.txt", '-');
  File in("./test.txt");
  in.open(File::in);
  File out("./anothertest.txt");
  out.open(File::out);
  while(in.isGood() && out.isGood())
  {
    std::string temp = in.getLine();
    std::cout << "\n  " << temp.c_str();
    out.putLine(temp);
    out.putLine("\n");
  }
  
  std::cout << "\n\n";
}
#endif
