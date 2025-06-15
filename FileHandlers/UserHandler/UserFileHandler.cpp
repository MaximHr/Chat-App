/*
	Name: Maksim Hristov
	FN: 4MI0600466
*/

#include "UserFileHandler.h"
#include "../../Users/User.h"
#include "../../Users/UserFactory.h"
#include "../../Utils/Config.h"
#include "../../System/System.h"

UserFileHandler& UserFileHandler::getInstance(const String& str) {
	static UserFileHandler instance(str);
	return instance;
}

UserFileHandler::~UserFileHandler() {
	delete fileHandler;
}

UserFileHandler::UserFileHandler(const String& str) {
	fileHandler = FileFactory::createFileHandler(Config::fileExtension);
	fileHandler->open(str);
}

void UserFileHandler::saveUser(const User* user, FileHandler& fs) {
	if(user == nullptr) throw std::invalid_argument("User can not be null pointer");
	if(!fs.isOpen()) throw std::runtime_error("file can not be opened");
	UserType type = user->getRole();

	fs.write((unsigned) type);
	fs.write(user->getName());
	fs.write(user->getHashedPassword());
	unsigned id = user->getId();
	fs.write(id);
	fs.file.flush();
}

User* UserFileHandler::readUser() {
	UserType role;
	
	fileHandler->read((unsigned&) role);
	User* newUser = UserFactory::createUser(role);

	fileHandler->read(newUser->name);
	fileHandler->read(newUser->hashedPassword);
	fileHandler->read(newUser->id);

	return newUser;
}

User* UserFileHandler::getUserMatcher(unsigned id, const String& hashedPassword, FindType type, const String& name) {
	int pos = -1;
	switch (type) {
		case(FindType::byId) :
			pos = findUser(id);
			break;
		case(FindType::byIdAndPassword) :
			pos = findUserWithPassword(id, hashedPassword);
			break;
		case(FindType::byName) :
			pos = findUserByName(name);
			break;
		default :
			throw std::invalid_argument("invalid find type");
			break;
	}
	
	if(pos == -1)  {
		throw std::runtime_error("User was not found.");
	}

	int current = fileHandler->file.tellg();
	fileHandler->file.seekg(pos);
	User* user = readUser();
	fileHandler->file.seekg(current);
	return user;
}

User* UserFileHandler::getUser(unsigned id) {
	return getUserMatcher(id, "", FindType::byId, "");
}

User* UserFileHandler::getUserByName(const String& name) {
	return getUserMatcher(-1, "", FindType::byName, name);
}

User* UserFileHandler::getUserByPassword(unsigned id, const String& hashedPassword) {
	return getUserMatcher(id, hashedPassword, FindType::byIdAndPassword, "");
}

User* UserFileHandler::readUser(int& sizeInBytes) {
	int start = fileHandler->file.tellg();
	User* user = readUser();
	sizeInBytes = (int)(fileHandler->file.tellg()) - start;

	return user;
}

int UserFileHandler::findUser(unsigned id) {
	return findUserMatcher(id, "", "", FindType::byId);
}

int UserFileHandler::findUserWithPassword(unsigned id, const String& hashedPassword) {
	return findUserMatcher(id, hashedPassword, "", FindType::byIdAndPassword);
}

int UserFileHandler::findUserByName(const String& name) {
	return findUserMatcher(0, "", name, FindType::byName);
}

int UserFileHandler::findUserMatcher(unsigned id, const String& hashedPassword, const String& name, FindType findType) {
	if(!fileHandler->isOpen()) throw std::runtime_error("file cannot be opened");
	if(fileHandler->getFileSize() == 0) return -1;

	int index = fileHandler->setAtBeginning();
	User* tempUser = readUser();
	int result = 0;

	while (
		(findType == FindType::byIdAndPassword && (tempUser->getId() != id || tempUser->getHashedPassword() != hashedPassword))
		|| (findType == FindType::byId && (tempUser->getId() != id))
		|| (findType == FindType::byName && (tempUser->getName() != name)))
	{
		if(fileHandler->file.eof()) {
			fileHandler->file.clear();
			delete tempUser;
			return -1;
		}
		result = fileHandler->file.tellg();
		delete tempUser;
		tempUser = readUser();
	}

	fileHandler->file.clear();
	fileHandler->file.seekg(index);
	delete tempUser;
	return result;
}

void UserFileHandler::deleteUser(unsigned id) {
	updateUserMatcher(id, nullptr);
}

void UserFileHandler::updateUser(unsigned id, const User* updatedUser) {
	updateUserMatcher(id, updatedUser);
}

void UserFileHandler::updateUserMatcher(unsigned id, const User* updatedUser) {
	FileHandler* output = FileFactory::createFileHandler(Config::fileExtension);
	output->open(Config::getFile(3).c_str());

	if(!output->isOpen()) throw std::runtime_error("Failed to open temporary file for writing");

	int index = fileHandler->setAtBeginning();
	int bytes = 0;
	User* tempUser = readUser(bytes);

	while(fileHandler->file) {
		if(tempUser->getId() != id) {
			fileHandler->copyBytes(output->file, bytes);
		} else {
			if(updatedUser != nullptr) {
				saveUser(updatedUser, *output);
			}
		}
		delete tempUser;
		tempUser = readUser(bytes);
	}

	delete output;

	fileHandler->changeFile(Config::getFile(3).c_str(), Config::getFile(0).c_str());
	if(index < fileHandler->getFileSize()) {
		fileHandler->file.seekg(index);
	}
	delete tempUser;
}
