#pragma once

#include <string>
#include <sstream>
#include <fstream>
#include <vector>

#include "OSInclude.hpp"
#include "LogAdaptor.hpp"
#include "exception.hpp"
#include "ArrayView.hpp"

#if defined(_WIN32)
#define StatFunction _stat64
using StatStruct = struct _stat64;
#else

#endif

namespace cg {

class FileSystemException :public cg::Exception
{
public:
	/**The error type for this exception.*/
	enum Error
	{
		/**The error is unknown.*/
		Unknown,
		/**The directory or file already exists. Any clobbers are off.*/
		AlreadyExists = EEXIST,
		/**The path cound not be found or accessed.*/
		PathNotFound = ERROR_PATH_NOT_FOUND,
		/**The file could not be found.*/
		FileNotFound,
		/**Invalid permissions for writing to the file or parent directory.*/
		BadPermission = EACCES,
		/**There are too many levels of sym links.*/
		SymbolicLoopLimit = ELOOP,
		/**Too many links in the parent directory.*/
		LinkCount = EMLINK,
		/**File name too long*/
		NameTooLong = ENAMETOOLONG,
		/**There is not enough space.*/
		NoSpace = ENOSPC,
		/**Tried to create in a read only filesystem.*/
		ReadOnly = EROFS,
		/**Part of the directory prefix is not a directory,or does not exist.*/
		BadPrefix = ENOTDIR,
		/**Invalid file name*/
		InvalidParameter = EINVAL,
		/**An invalid file descriptor.*/
		BadFileDiscriptor = EBADF,
		/**Bad address*/
		BadAddress = EFAULT,
		/**No memory*/
		NoMemory = ENOMEM,
		/**One of the variables are to big to reprisent.*/
		Overflow = EOVERFLOW,

	};
	/**Create the exception.
	\param e The error for the exception.*/
	FileSystemException(Error e = Unknown) : m_e(e) {};
	/**Get the message of the exception.*/
	inline virtual std::string ToString() const override {
		std::string msg = "FilesystemException: ";
		switch (m_e)
		{
		case cg::FileSystemException::AlreadyExists:
			msg += "The directory or file already exists.";
			break;
		case cg::FileSystemException::PathNotFound:
			msg += "The path could not be found.";
			break;
		case cg::FileSystemException::FileNotFound:
			msg += "The file could not be found.";
			break;
		case cg::FileSystemException::BadPermission:
			msg += "Does not have permission to write to the file or dir.";
			break;
		case cg::FileSystemException::SymbolicLoopLimit:
			msg += "There are too many levels of symbolic loops.";
			break;
		case cg::FileSystemException::LinkCount:
			msg += "There are too many links in the parent directory.";
			break;
		case cg::FileSystemException::NameTooLong:
			msg += "The name is too long.";
			break;
		case cg::FileSystemException::NoSpace:
			msg += "There is not enough space.";
			break;
		case cg::FileSystemException::NoMemory:
			msg += "There is not enough memory for the operation.";
			break;
		case cg::FileSystemException::BadAddress:
			msg += "The supplied address is invalid.";
			break;
		case cg::FileSystemException::ReadOnly:
			msg += "The file location is read only.";
			break;
		case cg::FileSystemException::InvalidParameter:
			msg += "The parameter is invalid.";
			break;
		case cg::FileSystemException::BadFileDiscriptor:
			msg += "The file discriptor is invalid.";
			break;
		case cg::FileSystemException::Overflow:
			msg += "One of the file info attributes is too big to reprisent.";
			break;
		case cg::FileSystemException::BadPrefix:
			msg += "A prefix to the file is either not a directory or is ";
			msg += "invalid.";
			break;
		default:
			msg += "An unknown error occured.";
		}
		return msg;
	}
private:
	/**The code for the error.*/
	Error m_e;
};

/**Static global options for file systems.*/
class FileSystem
{
public:
	/**Change the option of automatically creating directories and files.
	\param opt True to have all directories and files created if they can be.*/
	static void AutoCreate(bool opt);
	/**Determine if the file system should auto creat files and directories.
	\return True if files should be created automatically.*/
	static bool AutoCreate();
	/**Create a directory.
	\param dir The directory to create.
	\param mode Any arguments that may be used for creation. This is primarily for
	linux systems and creating a specific type of file. Man stat(2) for modes.
	\return True if a directory was created.*/
	static bool MakeDir(const std::string& dir, int mode = 0);
	/**Get the status object for a file.
	\param file The file to check into.
	\return A struct object with the status of the file.*/
	static auto Status(const std::string& file);
	/**Get the file size of the a file.
	\param file The name of the file.
	\return The size of the file.*/
	static std::size_t FileSize(const std::string& file);
	/**Determine if a file or directory exists.
	\param path The path of the file or dir.
	\return -1 if it is a directory. 0 if the file does not exist. 1 if the file
	does exist, but is not a directory.*/
	static int FileExists(const std::string& file);
	/**Create a file in an existing directory with no space.
	\param file The name of the file to create.
	\return True if the file was created or exists already.*/
	static bool Touch(const std::string& file);
	/**Determine if a file is a directory.
	\param name The name of the file.
	\return True if the path leads to a directory.*/
	static bool IsDirectory(const std::string& file);
	/**Determine if a file is not a directory.
	\param name The name of the file.
	\return True if the file is not a directory.*/
	static bool IsFile(const std::string& file);
private:
	/**The option for creating files auto.*/
	static bool ms_autoCreate;
};

/**A directory object. Options to create the directory if it does not exist.*/
class Dir : public cg::LogAdaptor<Dir>
{
public:
	/**Create the file without checking if it exists or trying to create it.
	\param filename The name of the file and its directory.*/
	Dir(const std::string& path = "");
	/**Copy a directory from another.
	\param other The other dir to copy.*/
	Dir(const Dir& other);
	/**Move a directory from another.
	\param other The other dir to move.*/
	Dir(Dir&& other);
	/**Copy a directory from another.
	\param other The other dir to copy.*/
	void operator=(const Dir& other);
	/**Move a directory from another.
	\param other The other dir to move.*/
	void operator=(Dir&& other);
	/*Add another directory to the end of this one.
	\param newDir The the dir(s) to add to this one.
	\return A ref to this object.*/
	Dir& operator+=(const std::string& newDir);
	/*Add another directory to the end of this one.
	\param newDir The the dir(s) to add to this one.
	\return A ref to this object.*/
	Dir& operator+=(const Dir& newDir);
	/*Add another directory to the end of this one.
	\param newDir The the dir(s) to add to this one.
	\return A new Dir object.*/
	Dir operator+(const std::string& newDir) const;
	/*Add another directory to the end of this one.
	\param newDir The the dir(s) to add to this one.
	\return A new Dir object.*/
	Dir operator+(const Dir& newDir) const;
	/**Insert paths at the front of the dir.
	\param newDir the dir(s) to insert.*/
	void PushFront(const Dir& newDir);
	/**Insert paths at the front of the dir.
	\param newDir the dir(s) to insert.*/
	void PushFront(const std::string& newDir);
	/**Insert paths at the back of the dir.
	\param newDir the dir(s) to insert.*/
	void PushBack(const Dir& newDir);
	/**Insert paths at the back of the dir.
	\param newDir the dir(s) to insert.*/
	void PushBack(const std::string& newDir);
	/**Creat the directory structure associated with this dir.
	\return True if the structure was created and a file can now be make in the
	directory. False otherwise.*/
	bool Touch() const;
	/**Get the string for this dir.
	\return The string to that is the path.*/
	std::string ToString() const;
private:
	using cg::LogAdaptor<Dir>::EnableLogs;
	using cg::LogAdaptor<Dir>::LogNote;
	using cg::LogAdaptor<Dir>::LogWarn;
	using cg::LogAdaptor<Dir>::LogError;
	using cg::LogAdaptor<Dir>::Log;
	using cg::LogAdaptor<Dir>::ms_log;
	using cg::LogAdaptor<Dir>::ms_name;
	/**Make sure that the structure is valid.
	\return True if the directory structure is valid or is created and valid.*/
	bool CreateStructure() const;
	/**Setup the path, making sure that slashes are normalized.
	\param path The path to setup.*/
	void Init(std::string path);
	/**The path to the file.*/
	std::vector<std::string> m_path;
};

/**A file object containing a cg::Dir path and file name.*/
class File : public cg::LogAdaptor<File>
{
public:
	/**Create the file.
	\param dir The directory the file will exist in.
	\param name The name of the file.*/
	File(const cg::Dir& dir, const std::string& name);
	/**Create the file.
	\param path The directory and name of the file.*/
	File(std::string path);
	/**Touch the file, making sure it exists.
	\return True if the file exists or was created. False if the file does not
	exist and could not be created.*/
	bool Touch() const;
	/**Delete this file only. The directory will not be removed.
	\return True if the file was removed.*/
	bool Remove() const;
	/**Write to this file.
	\param pos The position to write. Use 0 to write at the begining of the 
	file.  The internal stream position will automatically advance to the new
	position pos+size. If pos = -1, than the data will write where ever the
	stream is currently at.
	\param data The data to write.
	\param size The amount of data to write.
	\return True if data was written.*/
	bool Write(const char* data, std::size_t size, std::ptrdiff_t pos = -1);
	/**Write to this file.
	\param pos The position to write. Use 0 to write at the begining of the
	file.  The internal stream position will automatically advance to the new
	position pos+size. If pos = -1, than the data will write where ever the
	stream is currently at.
	\param data An array view with data to write.
	\return True if data was written.*/
	bool Write(const cg::ArrayView& data, std::ptrdiff_t pos = -1);
	/**Read from the file.
	\param pos The position to read from.
	\param data The place to put the data.
	\param size The amount of data to read.
	\return True if data was read, False if there was an issue.*/
	bool Read(char* data, std::size_t size, std::ptrdiff_t pos = -1);
	/**Read from the file.
	\param pos The position to read from.
	\param data An array view that is initialized to a size that will be the 
	size of the data to read.
	\return True if data was read, False if there was an issue.*/
	bool Read(cg::ArrayView& data, std::ptrdiff_t pos = -1);
	/**Get the full path of the file and its directories.
	\return A string with the full path of the file.*/
	std::string FullPath() const;
	/**Get a string with the path.
	\return A string with just the file name.*/
	std::string ToString() const;
	/**Get the extention of the file.
	\return A string with just the extension of the file.  If the file has no
	extension, then a blank string will be returned.*/
	std::string Ext() const;
private:
	using cg::LogAdaptor<File>::EnableLogs;
	using cg::LogAdaptor<File>::LogNote;
	using cg::LogAdaptor<File>::LogWarn;
	using cg::LogAdaptor<File>::LogError;
	using cg::LogAdaptor<File>::Log;
	using cg::LogAdaptor<File>::ms_log;
	using cg::LogAdaptor<File>::ms_name;
	/**an  for writing or reading.*/
	std::fstream m_stream;
	/**The directory for which the file will exist.*/
	cg::Dir m_dir;
	/**The name of the file.*/
	std::string m_name;
};

}