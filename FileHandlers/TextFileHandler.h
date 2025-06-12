/*
	Name: Maksim Hristov
	FN: 4MI0600466
*/

#pragma once
#include "FileHandler.h"

class TextFileHandler: public FileHandler {
	const char delim = '|';
public:
	TextFileHandler() = default;

	void open(const String& str) override;
	void write(const String& str) override;
	void read(String& str) override;
	void copyBytes(std::fstream& output, int bytes) override;
	void read(unsigned& id) override;
	void write(unsigned id) override;

};