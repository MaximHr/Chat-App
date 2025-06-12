/*
	Name: Maksim Hristov
	FN: 4MI0600466
*/

#pragma once
#include "FileHandler.h"
#include "../Utils/Config.h"
#include <exception>

class FileFactory {
public:
	FileFactory() = delete;

	static FileHandler* createFileHandler(FileExtension extension);
};