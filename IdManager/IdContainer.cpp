/*
	Name: Maksim Hristov
	FN: 4MI0600466
*/

#include "IdContainer.h"

void IdContainer::copyFrom(const IdContainer& other) {
	length = other.length;
	idCreators = new IdCreator[length];
	for(int i = 0;i < length;i++) {
		idCreators[i] = other.idCreators[i];
	}
}

void IdContainer::loadLargestIds() {
	if(fileHandler->getFileSize() == 0) return;
	int index = fileHandler->file.tellg();
	fileHandler->file.seekg(0);

	for(int i = 0;i < length;i++) {
		fileHandler->read(idCreators[i].tableName);
		fileHandler->read(idCreators[i].idCounter);
	}
	fileHandler->file.seekg(index);
}

void IdContainer::saveIdCounters() {
	FileHandler* ofs = FileFactory::createFileHandler(Config::fileExtension);
	ofs->open(Config::getFile(3).c_str());

	if(!ofs->isOpen()) throw std::runtime_error("Failed to open temporary file for writing");

	for(int i = 0;i < length;i++) {
		ofs->write(idCreators[i].tableName);
		ofs->write(idCreators[i].idCounter);
	}
	
	delete ofs;
	fileHandler->changeFile(Config::getFile(3).c_str(), Config::getFile(1).c_str());
}

void IdContainer::free() {
	delete fileHandler;
	delete[] idCreators;
	length = 0;
}

IdContainer::IdContainer(const String& fileName, const String* names, int length) {
	if(names == nullptr) throw std::invalid_argument("argument can not be null pointer");
	
	fileHandler = FileFactory::createFileHandler(Config::fileExtension);
	fileHandler->open(fileName);

	this->length = length;
	idCreators = new IdCreator[length];
	for(int i = 0;i < length;i++) {
		idCreators[i].tableName = names[i];
		idCreators[i].tableName += Config::getFileExtension();
	}
	loadLargestIds();
}

IdContainer::IdContainer(const IdContainer& other) {
	copyFrom(other);
}

IdContainer& IdContainer::operator=(const IdContainer& other) {
	if(this != &other) {
		free();
		copyFrom(other);
	}
	return *this;
}

IdContainer::~IdContainer() {
	free();
}

int IdContainer::findByName(const String& name) const {
	for(int i = 0;i < length;i++) {
		if(idCreators[i].tableName == name) {
			return i;
		}
	}
	return -1;
}

void IdContainer::increment(const String& name) {
	int index = findByName(name);
	if(index == -1) {
		throw std::runtime_error(" was not found");
	}
	idCreators[index].increment();
	saveIdCounters();
}

void IdContainer::setIdCounter(const String& name, unsigned value) {
	int index = findByName(name);
	if(index == -1) {
		throw std::runtime_error("table name was not found");
	}
	idCreators[index].setIdCounter(value);
	saveIdCounters();
}

unsigned IdContainer::getId(const String& name) const {
	int index = findByName(name);
	if(index == -1) {
		throw std::runtime_error("table name was not found");
	}
	return idCreators[index].idCounter;
}