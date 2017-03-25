#include "FileSystem.hpp"

namespace cg {
Dir::Dir(const std::string & path)
{
	Init(path);
}
Dir::Dir(const Dir & other)
{
	m_path.insert(m_path.begin(), other.m_path.begin(), other.m_path.end());
}
Dir::Dir(Dir && other)
{
	m_path.swap(other.m_path);
}
void Dir::operator=(const Dir & other)
{
	m_path.insert(m_path.begin(), other.m_path.begin(), other.m_path.end());
}
void Dir::operator=(Dir && other)
{
	m_path.swap(other.m_path);
}
bool Dir::MakeDir(const std::string & dir, int mode)
{
#if defined(_WIN32)

	bool created = CreateDirectory(dir.c_str(), 0) == TRUE;
	if (!created)
	{
		auto err = GetLastError();
		if (err == ERROR_ALREADY_EXISTS)
			throw FileSystemException(FileSystemException::AlreadyExists);
		else if (err == ERROR_PATH_NOT_FOUND)
			throw FileSystemException(FileSystemException::PathNotFound);
		else
			throw FileSystemException(FileSystemException::Unknown);
	}
	return created;
#else
	if (mode == 0)
		mode = S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH;
	int status = mkdir(dir.c_str(), mode);
	if (status == 0)
		return true;
	throw FileSystemException(errno);
#endif

}
auto Dir::Status(const std::string & file)
{
	StatStruct sb;
	auto ret = StatFunction(file.c_str(), &sb);
	return sb;
}
std::size_t Dir::FileSize(const std::string & file)
{
	return Status(file).st_size;
}
int Dir::FileExists(const std::string & file)
{
	auto mode = Status(file).st_mode;
	if (mode == 0)
		return 0;
	auto isDir = (mode & S_IFDIR) != 0;
	if (isDir)
		return -1;
	else
		return 1;
}
bool Dir::Touch(const std::string & file)
{
	if (FileExists(file) != 0)
		return true;

	std::ofstream ofs(file.c_str(), std::ios::binary);
	return ofs.good();
}

bool Dir::IsDirectory(const std::string & file)
{
	return FileExists(file) == -1;
}

bool Dir::IsFile(const std::string & file)
{
	return FileExists(file) == 1;
}

std::string Dir::ToString() const
{
	std::string str;
	auto it = m_path.begin();
	auto end = m_path.end();
	for (; it != end; ++it)
	{
		str += *it;
		str += "/";
	}
	return str;
}

bool Dir::CreateStructure() const
{
	if (m_path.empty())
		return false;
	auto it = m_path.begin();
	auto end = m_path.end();
	std::string tPath;
	for (; it != end; ++it)
	{
		tPath += *it;
		if (FileExists(tPath))
		{
			tPath += "/";
			continue;
		}
		else
			if (!MakeDir(tPath))
				return false;
		tPath += "/";
	}
	return true;
}

Dir & Dir::operator+=(const std::string & newDir)
{
	m_path.push_back(newDir);
	return *this;
}

Dir & Dir::operator+=(const Dir & newDir)
{
	m_path.insert(m_path.end(), newDir.m_path.begin(), newDir.m_path.end());
	return *this;
}

Dir Dir::operator+(const std::string & newDir) const
{
	auto copy = *this;
	return copy += newDir;
}

Dir Dir::operator+(const Dir & newDir) const
{
	auto copy = *this;
	return copy += newDir;
}

void Dir::PushFront(const Dir & newDir)
{
	m_path.insert(m_path.begin(), newDir.m_path.begin(), newDir.m_path.end());
}

void Dir::PushFront(const std::string & newDir)
{
	m_path.insert(m_path.begin(), newDir);
}

void Dir::PushBack(const Dir & newDir)
{
	*this += newDir;
}

void Dir::PushBack(const std::string & newDir)
{
	*this += newDir;
}

bool Dir::Create() const
{
	return CreateStructure();
}

void Dir::Init(std::string path)
{
	for (std::size_t i = 0; path[i] != 0; ++i)
	{
		if (path[i] == '\\')
			path[i] = '/';
	}
	std::stringstream ss(path);
	for (std::string chunk; std::getline(ss, chunk, '/');)
	{
		m_path.push_back(chunk);
	}
}

File::File(const cg::Dir & dir, const std::string & name)
	:m_dir(dir), m_name(name) {};

File::File(std::string path)
{
	for (std::size_t i = 0; path[i] != 0; ++i)
	{
		if (path[i] == '\\')
			path[i] = '/';
	}
	std::size_t lastSlash = path.find_last_of('/');
	if (lastSlash != std::string::npos)
	{
		m_dir = cg::Dir(path.substr(0, lastSlash));
		++lastSlash;
		m_name = path.substr(lastSlash);
	}
	else
	{
		m_name = path;
	}
}

bool File::Touch() const
{
	return m_dir.Create() && Dir::Touch(m_dir.ToString() + m_name);
}

bool File::Remove() const
{
	int ret = std::remove((m_dir.ToString() + m_name).c_str());
	if (ret == 0)
		return true;
	else
		return false;
}

}