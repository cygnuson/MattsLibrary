#include "FileSystem.hpp"

namespace cg {
Dir::Dir(const std::string & path)
{
	Init(path);
	EnableLogs(true, "FileSystem::Dir");
}
Dir::Dir(const Dir & other)
{
	m_path.insert(m_path.begin(), other.m_path.begin(), other.m_path.end());
	EnableLogs(true, "FileSystem::Dir");
}
Dir::Dir(Dir && other)
{
	m_path.swap(other.m_path);
	EnableLogs(true, "FileSystem::Dir");
}
void Dir::operator=(const Dir & other)
{
	m_path.insert(m_path.begin(), other.m_path.begin(), other.m_path.end());
}
void Dir::operator=(Dir && other)
{
	m_path.swap(other.m_path);
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
		if (FileSystem::FileExists(tPath))
		{
			tPath += "/";
			continue;
		}
		else
			if (!FileSystem::MakeDir(tPath))
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

bool Dir::Touch() const
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
/************************************************************************************************/
File::File(const cg::Dir & dir, const std::string & name)
	:m_dir(dir), m_name(name)
{
	EnableLogs(true, "FileSystem::File");
};

File::File(std::string path)
{
	EnableLogs(true, "FileSystem::File");
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
	return m_dir.Touch() && FileSystem::Touch(m_dir.ToString() + m_name);
}

bool File::Remove() const
{
	int ret = std::remove(FullPath().c_str());
	if (ret == 0)
		return true;
	else
		return false;
}

bool File::Write(const char * data, std::size_t size, std::ptrdiff_t pos)
{
	if (FileSystem::AutoCreate())
	{
		m_dir.Touch();
		FileSystem::Touch(FullPath());
	}
	if (!m_stream.is_open())
		m_stream.open(FullPath().c_str());
	if (!m_stream.is_open())
	{
		LogError("The file could not be opened: ", FullPath());
		return false;
	}
	if (pos != -1)
		m_stream.seekg(pos);
	m_stream.write(data, size);
	m_stream.close();
	return true;
}

bool File::Write(const cg::ArrayView & data, std::ptrdiff_t pos)
{
	return Write(data.data(), data.size(), pos);
}

bool File::Read(char * data, std::size_t size, std::ptrdiff_t pos)
{
	if (!m_stream.is_open())
		m_stream.open(FullPath().c_str());
	if (!m_stream.is_open())
	{
		LogError("The file could not be opened: ", FullPath());
		return false;
	}
	if (pos != -1)
		m_stream.seekg(pos);
	m_stream.read(data, size);
	m_stream.close();
	return true;
}

bool File::Read(cg::ArrayView& data, std::ptrdiff_t pos)
{
	return Read(data.data(), data.size(), pos);
}

std::string File::FullPath() const
{
	return (m_dir.ToString() + m_name);
}

std::string File::ToString() const
{
	return m_name;
}

std::string File::Ext() const
{
	auto dot = m_name.find_last_of('.');
	if (dot == std::string::npos)
		return "";
	return m_name.substr(dot + 1);
}
void File::ShiftRight(std::size_t pos, std::size_t amt,
	char fillByte, bool arrayLike)
{
	auto size = FileSystem::FileSize(FullPath()) - pos;
	cg::ArrayView data(size);
	m_stream.open(FullPath().c_str(), std::ios::binary | std::ios::in);
	m_stream.seekg(pos);
	m_stream.read(data.data(), size);
	m_stream.close();
	m_stream.open(FullPath().c_str(), std::ios::binary
		| std::ios::out
		| std::ios::in);
	m_stream.seekp(pos + amt);
	if (arrayLike)
		m_stream.write(data.data(), size - amt);
	else
		m_stream.write(data.data(), size);
	m_stream.seekp(pos);
	while (amt-- > 0)
		m_stream.write(&fillByte, 1);
	m_stream.close();
}
void File::ShiftLeft(std::size_t pos, std::size_t amt, char fillByte)
{

	auto size = pos;
	cg::ArrayView data(size);
	m_stream.open(FullPath().c_str(), std::ios::binary | std::ios::in);
	m_stream.seekg(amt);
	m_stream.read(data.data(), size - amt);
	m_stream.close();
	m_stream.open(FullPath().c_str(), std::ios::binary
		| std::ios::out
		| std::ios::in);
	m_stream.seekp(0);
	m_stream.write(data.data(), size - amt);
	m_stream.seekp(pos);
	while (amt-- > 0)
		m_stream.write(&fillByte, 1);
	m_stream.close();
}
std::size_t File::Size() const
{
	return FileSystem::FileSize(FullPath());
}
/*************************************************************************************************/

bool FileSystem::ms_autoCreate = false;

void FileSystem::AutoCreate(bool opt)
{
	ms_autoCreate = opt;
}

bool FileSystem::AutoCreate()
{
	return ms_autoCreate;
}

bool FileSystem::MakeDir(const std::string & dir, int mode)
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
auto FileSystem::Status(const std::string & file)
{
	StatStruct sb;
	auto ret = StatFunction(file.c_str(), &sb);
	return sb;
}
std::size_t FileSystem::FileSize(const std::string & file)
{
	return Status(file).st_size;
}
int FileSystem::FileExists(const std::string & file)
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
bool FileSystem::Touch(const std::string & file)
{
	if (FileExists(file) != 0)
		return true;

	std::ofstream ofs(file.c_str(), std::ios::binary);
	return ofs.good();
}

bool FileSystem::IsDirectory(const std::string & file)
{
	return FileExists(file) == -1;
}

bool FileSystem::IsFile(const std::string & file)
{
	return FileExists(file) == 1;
}
}