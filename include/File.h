#pragma once

#include <basicIncludes.h>

namespace File
{
	std::string readAllText(const std::string& filePath);
	std::vector<uint8_t> readAllBytes(const std::string& filePath);
	std::vector<std::string> readAllLines(const std::string& filePath);
	bool writeAllText(const std::string& filePath, const std::string& content);
	bool writeAllBytes(const std::string& filePath, const uint8_t* content, size_t length);
	bool writeAllBytes(const std::string& filePath, const std::vector<uint8_t>& content);
	bool remove(const std::string& filePath);
	bool copy(const std::string& sourcePath, const std::string& destPath, bool overwrite = true);
	bool move(const std::string& sourcePath, const std::string& destPath);
	bool rename(const std::string& oldName, const std::string& newName);
	bool exists(const std::string& filePath);
}
