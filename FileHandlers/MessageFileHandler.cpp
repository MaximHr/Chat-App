/*
	Name: Maksim Hristov
	FN: 4MI0600466
*/

#include "MessageFileHandler.h"
#include "../Components/Message/Message.h"

MessageFileHandler& MessageFileHandler::getInstance(const String& str) {
	static MessageFileHandler instance(str);

	return instance;
}

MessageFileHandler::MessageFileHandler(const String& str) {
	fileHandler = FileFactory::createFileHandler(Config::fileExtension);
	fileHandler->open(str);
}

MessageFileHandler::~MessageFileHandler() {
	delete fileHandler;
}

void MessageFileHandler::saveMessage(const Message& message, FileHandler& fs) {
	if(!fs.isOpen()) throw std::runtime_error("file can not be opened");

	unsigned id = message.getId();
	unsigned recieverId = message.getRecieverId();
	unsigned senderId = message.getSenderId();

	fs.write(id);
	fs.write(recieverId);
	fs.write(senderId);
	fs.write(message.getText());
	fs.write(message.getFormattedTime());
}

void MessageFileHandler::deleteMessages(unsigned recieverId) {
	FileHandler* output = FileFactory::createFileHandler(Config::fileExtension);
	output->open(Config::getFile(3).c_str());

	if(!output->isOpen()) throw std::runtime_error("Failed to open temporary file for writing");

	int index = fileHandler->setAtBeginning();
	int bytes = 0;
	Message* message = readMessage(bytes);

	while(fileHandler->file) {
		if(message->getRecieverId() != recieverId) {
			fileHandler->copyBytes(output->file, bytes);
		};
		delete message;
		message = readMessage(bytes);
	}

	delete output;

	fileHandler->changeFile(Config::getFile(3).c_str(), Config::getFile(2).c_str());
	if(index < fileHandler->getFileSize()) {
		fileHandler->file.seekg(index);
	}
	delete message;
}

void MessageFileHandler::printMessages(unsigned recieverId) {
	if(!fileHandler->isOpen()) throw std::runtime_error("file cannot be opened");
	if(fileHandler->getFileSize() == 0) {
		std::cout << "There are no messages yet." << '\n';
		return;
	};

	int index = fileHandler->setAtBeginning();
	Message* message = readMessage();
	bool containsSubmissions = false;
	
	while(!fileHandler->file.eof()) {
		if(message->getRecieverId() == recieverId) {
			containsSubmissions = true;
			std::cout << *message;
		}
		delete message;
		message = readMessage();
	}
	if(!containsSubmissions) {
		std::cout << "There are no messages yet." << '\n';
	}

	fileHandler->file.clear();
	fileHandler->file.seekg(index);
	delete message;
}

int MessageFileHandler::findMessage(unsigned id) {
	if(!fileHandler->isOpen()) throw std::runtime_error("file cannot be opened");
	if(fileHandler->getFileSize() == 0) return -1;

	int index = fileHandler->setAtBeginning();
	Message* message = readMessage();
	int result = 0;

	while(message->getId() != id) {
		if(!fileHandler->file) {
			fileHandler->file.clear();
			delete message;
			return -1;
		}
		result = fileHandler->file.tellg();
		delete message;
		message = readMessage();
	}

	fileHandler->file.clear();
	fileHandler->file.seekg(index);
	delete message;

	return result;
}

Message* MessageFileHandler::readMessage() {
	if(!fileHandler->isOpen()) throw std::runtime_error("file can not be opened");

	Message* message = new Message();
	fileHandler->read(message->id);
	fileHandler->read(message->recieverId);
	fileHandler->read(message->senderId);
	fileHandler->read(message->text);
	fileHandler->read(message->formattedTime);

	return message;
}

Message* MessageFileHandler::readMessage(int& bytes) {
	int start = fileHandler->file.tellg();
	Message* message = readMessage();
	bytes = (int)(fileHandler->file.tellg()) - start;

	return message;
}

// Message* MessageFileHandler::getMessage(unsigned id) {
// 	int pos = findMessage(id);
// 	if(pos == -1)  {
// 		throw std::runtime_error("Message with that id was not found");
// 	}

// 	int current = file.tellg();
// 	file.seekg(pos);
// 	Message* message = readMessage();
// 	file.seekg(current);

// 	return message;
// }
