/*
	Name: Maksim Hristov
	FN: 4MI0600466
*/

#pragma once
#include "String.h"

enum class FileExtension {
	txt,
	dat
};

class Config {
	static const FileExtension fileExtension;

public:
	static const unsigned adminId;
	static const String adminPassword;

	static const String getFileExtension();
	static const int numberOfFiles = 4;
	static const String fileNames[numberOfFiles];
	static const String getFile(unsigned i);

};
