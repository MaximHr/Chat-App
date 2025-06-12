/*
	Name: Maksim Hristov
	FN: 4MI0600466
*/

#include "BinaryFileHandler.h"

void BinaryFileHandler::open(const String& str) {
	if(!isOpen()) {
		file.open(str.c_str(), std::ios::binary | std::ios::in | std::ios::out);
	}
	if(!isOpen()) {
		file.clear();
		file.open(str.c_str(), std::ios::out | std::ios::binary); // Creates the file
		file.close();
		file.open(str.c_str(), std::ios::in | std::ios::out | std::ios::binary);
	}
	if(!isOpen()) {
		throw std::runtime_error("File can not be opened 21.");
	}
}

void BinaryFileHandler::write(const String& str) {
	if(!isOpen()) {
		throw std::runtime_error("File can not be opened 27.");
	}
	file.seekp(0, std::ios::end);
	unsigned length = str.getSize();
	file.write((const char*)& length, sizeof(unsigned));
	file.write((const char*) str.c_str(), length);
}

void BinaryFileHandler::write(unsigned id) {
	if(!isOpen()) throw std::runtime_error("File can not be opened.");

	file.seekp(0, std::ios::end);
	file.write((const char*)& id, sizeof(id));
};

void BinaryFileHandler::read(String& str) {
	if(!isOpen()) throw std::runtime_error("File can not be opened 49.");

	unsigned length;
	file.read((char*)&length, sizeof(unsigned));
	char buffer[length + 1];
	file.read((char*)buffer, length);
	buffer[length] = '\0';
	str = buffer;
}

void BinaryFileHandler::read(unsigned& id) {
	if(!isOpen()) throw std::runtime_error("File can not be opened 49.");
	file.read((char *)& id, sizeof(unsigned));
}

void BinaryFileHandler::copyBytes(std::fstream& output, int bytes) {
	if(!isOpen()) throw std::runtime_error("File can not be opened 49.");
	if(bytes <= 0) return;

	const int chunkSize = 2048;
	file.seekg(-bytes, std::ios::cur);

	char buffer[chunkSize];
	while(bytes > 0) {
		int currentChunk = (bytes < chunkSize) ? bytes : chunkSize;
		file.read(buffer, currentChunk);
		output.write(buffer, currentChunk);
		bytes -= currentChunk;
	}

}
