/*
	Name: Maksim Hristov
	FN: 4MI0600466
*/

#include "MessageFileHandler.h"
#include "../../Components/Message/Message.h"

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
	unsigned chatId = message.getChatId();
	unsigned senderId = message.getSenderId();

	fs.write(id);
	fs.write(chatId);
	fs.write(senderId);
	fs.write(message.getText());
	fs.write(message.getFormattedTime());
	fs.file.flush();
}

void MessageFileHandler::printMessages(unsigned chatId) {
	if(!fileHandler->isOpen()) throw std::runtime_error("file cannot be opened");
	if(fileHandler->getFileSize() == 0) {
		std::cout << "There are no messages yet." << '\n';
		return;
	};

	int index = fileHandler->setAtBeginning();
	Message* message = readMessage();
	bool containsSubmissions = false;
	
	while(!fileHandler->file.eof()) {
		if(message->getChatId() == chatId) {
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
	fileHandler->read(message->chatId);
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
