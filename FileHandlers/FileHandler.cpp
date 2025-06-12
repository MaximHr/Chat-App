/*
	Name: Maksim Hristov
	FN: 4MI0600466
*/

#include "../Utils/Config.h"
#include "FileHandler.h"
#include <iostream>

void FileHandler::changeFile(const char* strFrom, const char* strTo) {
	if(strFrom == nullptr || strTo == nullptr) {
		throw std::invalid_argument("File name is nullptr");
	}
	file.clear();
	file.close();
	file.flush();
	file.clear();
	remove(strTo);
  std::rename(strFrom, strTo);
	open(strTo);
}

bool FileHandler::isOpen() const {
	return file.is_open();
}

void FileHandler::close() {
	file.flush();
	file.close();
}

FileHandler::~FileHandler() {
	close();
}

int FileHandler::getFileSize() {
	if(!isOpen()) throw std::runtime_error("File can not be opened. 85");
	int currentPos = file.tellg();

	file.seekg(0, std::ios::end);
	int fileSize = file.tellg();
	
	file.seekg(currentPos, std::ios::beg);

	return fileSize;
}

int FileHandler::setAtBeginning() {
	int index = file.tellg();
	file.clear();
	file.seekg(0);
	return index;
}