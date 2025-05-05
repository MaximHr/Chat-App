#include "FileHandler.h"
#include "iostream"

FileHandler::FileHandler(const String& str) {
	open(str);
}

void FileHandler::open(const String& str) {
	file.open(str.c_str(), std::ios::binary | std::ios::in | std::ios::out);
	if(!isOpen()) {
		throw std::runtime_error("File can not be opened. Are you sure that it exists?");
	}
}

void FileHandler::write(const String& str) {
	if(!isOpen()) {
		throw std::runtime_error("File can not be opened. Are you sure that it exists?");
	}
	file.seekp(0, std::ios::end);
	unsigned length = str.getSize();
	file.write((const char*)& length, sizeof(unsigned));
	file.write((const char*) str.c_str(), length);
}

void FileHandler::write(const char* data, unsigned bytes) {
	if(data == nullptr) throw std::invalid_argument("Argument cannot be null pointer");
	if(!isOpen()) throw std::runtime_error("File can not be opened. Are you sure that it exists?");

	file.seekp(0, std::ios::end);
	file.write(data, bytes);

}

void FileHandler::read(String& str) {
	if(!isOpen()) throw std::runtime_error("File can not be opened. Are you sure that it exists?");
	unsigned length;
	file.read((char*)&length, sizeof(unsigned));
	std::cout << "Length: " << length << '\n';
	char buffer[length];
	file.read((char*)buffer, length);
	str = buffer;
}

bool FileHandler::isOpen() const {
	return file.is_open();
}

void FileHandler::close() {
	file.close();
}

FileHandler& FileHandler::operator=(const String& str) {
	close();
	open(str);

	return *this;
}

FileHandler::~FileHandler() {
	close();
}