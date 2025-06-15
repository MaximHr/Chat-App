/*
	Name: Maksim Hristov
	FN: 4MI0600466
*/

#include "TextFileHandler.h"

void TextFileHandler::open(const String& str) {
	if(!isOpen()) {
		file.open(str.c_str(), std::ios::in | std::ios::out);
	}
	if(!isOpen()) {
		file.clear();
		file.open(str.c_str(), std::ios::out); // Creates the file
		file.close();
		file.open(str.c_str(), std::ios::in | std::ios::out);
	}
	if(!isOpen()) {
		throw std::runtime_error("File can not be opened 21.");
	}
};

void TextFileHandler::write(const String& str) {
	if(!isOpen()) {
		throw std::runtime_error("File can not be opened.");
	}
	file.seekp(0, std::ios::end);
	file << str.c_str() << delim; 
};

void TextFileHandler::write(unsigned id) {
	if(!isOpen()) throw std::runtime_error("File can not be opened.");

	file.seekp(0, std::ios::end);
	file << id << delim;     
};

void TextFileHandler::read(unsigned& id) {
	if(!isOpen()) {
		throw std::runtime_error("File can not be opened.");
	}
	file >> id;
	file.ignore();

};

void TextFileHandler::read(String& str) {
	if(!isOpen()) throw std::runtime_error("File can not be opened.");
	
	const int maxSize = 2048;
	char buffer[maxSize];
	file.getline(buffer, maxSize, delim);
	str = buffer;

};

void TextFileHandler::copyBytes(std::fstream& output, int bytes) {
	if(!isOpen()) throw std::runtime_error("File cannot be opened.");
	if(bytes <= 0) return;

	const int chunkSize = 2048;

	file.seekg(-bytes, std::ios::cur);
	if(!file) throw std::runtime_error("Seek failed.");

	char buffer[chunkSize];

	while(bytes > 0) {
		int currentChunk = (bytes < chunkSize) ? bytes : chunkSize;
		file.read(buffer, currentChunk);
		output.write(buffer, currentChunk);
		bytes -= currentChunk;
	}
};

void TextFileHandler::skipIds(int bytes) {
	int times = bytes / sizeof(unsigned);
	for(int i = 0;i < times;i++) {
		unsigned id;
		read((unsigned&) id);
	}
};