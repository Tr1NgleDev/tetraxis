#include <Directory.h>

bool Directory::create(const std::string& path)
{
	return std::filesystem::create_directory(path);
}

bool Directory::copy(const std::string& sourcePath, const std::string& destPath, bool overwrite, bool recursive)
{
	if (overwrite || !exists(destPath))
	{
		try
		{
			std::filesystem::copy(sourcePath, destPath, recursive ? std::filesystem::copy_options::recursive : std::filesystem::copy_options::none);
			return true;
		}
		catch (const std::filesystem::filesystem_error&)
		{
		}
	}
	return false;
}

void Directory::move(const std::string& sourcePath, const std::string& destPath)
{
	std::filesystem::rename(sourcePath, destPath);
}

void Directory::rename(const std::string& oldName, const std::string& newName)
{
	move(oldName, newName);
}

bool Directory::exists(const std::string& path)
{
	return std::filesystem::exists(path) && std::filesystem::is_directory(path);
}

bool Directory::remove(const std::string& path)
{
	return std::filesystem::remove_all(path) > 0;
}

std::vector<std::string> Directory::getDirectories(const std::string& path, bool recursive)
{
	std::vector<std::string> directories;

	if (recursive)
	{
		for (const auto& entry : std::filesystem::recursive_directory_iterator(path))
			if (std::filesystem::is_directory(entry.path()))
				directories.emplace_back(entry.path().string());
	}
	else
	{
		for (const auto& entry : std::filesystem::directory_iterator(path))
			if (std::filesystem::is_directory(entry.path()))
				directories.emplace_back(entry.path().string());
	}

	return directories;
}

std::vector<std::string> Directory::getFiles(const std::string& path, bool recursive)
{
	std::vector<std::string> files;

	if (recursive)
	{
		for (const auto& entry : std::filesystem::recursive_directory_iterator(path))
			if (std::filesystem::is_regular_file(entry.path()))
				files.emplace_back(entry.path().string());
	}
	else
	{
		for (const auto& entry : std::filesystem::directory_iterator(path))
			if (std::filesystem::is_regular_file(entry.path()))
				files.emplace_back(entry.path().string());
	}

	return files;
}
