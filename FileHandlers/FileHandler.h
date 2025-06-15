/*
	Name: Maksim Hristov
	FN: 4MI0600466
*/

#pragma once
#include "../Utils/String.h"
#include <fstream>
#include <exception>

class FileHandler {
public:
	std::fstream file;

	FileHandler() = default;
	virtual ~FileHandler();

	virtual void open(const String& str) = 0;
	virtual void write(const String& str) = 0;
	virtual void write(unsigned id) = 0;
	virtual void read(String& str) = 0;
	virtual void read(unsigned& id) = 0;
	virtual void skipIds(int bytes) = 0;
	virtual void copyBytes(std::fstream& output, int bytes) = 0;

	void changeFile(const char* strFrom, const char* strTo);
	bool isOpen() const;
	void close();
	int getFileSize();
	int setAtBeginning();
};