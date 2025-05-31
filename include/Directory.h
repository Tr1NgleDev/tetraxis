#pragma once

#include <basicIncludes.h>

namespace Directory
{
	std::vector<std::string> getFiles(const std::string& path, bool recursive = true);
	std::vector<std::string> getDirectories(const std::string& path, bool recursive = true);
	bool create(const std::string& path);
	bool remove(const std::string& path);
	void move(const std::string& sourcePath, const std::string& destPath);
	bool copy(const std::string& sourcePath, const std::string& destPath, bool overwrite = true, bool recursive = true);
	void rename(const std::string& oldName, const std::string& newName);
	bool exists(const std::string& path);
}
