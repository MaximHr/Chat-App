#pragma once
#include "../utils/String.h"
#include "fstream"
#include "exception"

//TODO? make it not instantiatable

struct FileHandler {
	std::fstream file;

	FileHandler()=default;
	FileHandler(const String& str);
	void open(const String& str);
	bool isOpen() const;
	void close();
	FileHandler& operator=(const String& str);
	~FileHandler();
	void write(const String& str);
	void read(String& str);
	void write(const char* data, unsigned bytes);
};