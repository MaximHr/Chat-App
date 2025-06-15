/*
	Name: Maksim Hristov
	FN: 4MI0600466
*/

#include "GroupChatFileHandler.h"
#include "../../Components/Chats/GroupChat.h"

GroupChatFileHandler::GroupChatFileHandler(const String& chatFile, const String& membersFile) {
	chatFileHandler = FileFactory::createFileHandler(Config::fileExtension);
	chatFileHandler->open(chatFile);

	membersFileHandler = FileFactory::createFileHandler(Config::fileExtension);
	membersFileHandler->open(membersFile);
}

GroupChatFileHandler& GroupChatFileHandler::getInstance(const String& chatFile, const String& membersFile) {
	static GroupChatFileHandler instance(chatFile, membersFile);

	return instance;
}

GroupChatFileHandler::~GroupChatFileHandler() {
	delete chatFileHandler;
	delete membersFileHandler;
}

void GroupChatFileHandler::saveChat(const GroupChat& chat, FileHandler& fs)  {
	if(!fs.isOpen()) throw std::runtime_error("file can not be opened");

	fs.write((unsigned)chat.getType());
	fs.write(chat.getId());
	fs.write(chat.getAdminId());
	fs.write(chat.getName());
	fs.write(chat.getMembersCount());
	fs.write(chat.getMessagesCount());
	fs.file.flush();
}

GroupChat GroupChatFileHandler::readChat(int& bytes) {
	int start = chatFileHandler->file.tellg();
	GroupChat chat = readChat();
	bytes = (int)(chatFileHandler->file.tellg()) - start;

	return chat;
}

GroupChat GroupChatFileHandler::readChat() {
	if(!chatFileHandler->isOpen()) throw std::runtime_error("file can not be opened");

	chatFileHandler->skipIds(sizeof(unsigned));
	GroupChat chat;
	chatFileHandler->read(chat.id);
	chatFileHandler->read(chat.adminId);
	chatFileHandler->read(chat.name);
	chatFileHandler->read(chat.membersCount);
	chatFileHandler->read(chat.messagesCount);

	return chat;
}

void GroupChatFileHandler::addMessage(unsigned chatId, unsigned messageId) {

}

int GroupChatFileHandler::findChat(unsigned chatId) {
	if(!chatFileHandler->isOpen()) throw std::runtime_error("file cannot be opened");
	if(chatFileHandler->getFileSize() == 0) return -1;

	int index = chatFileHandler->setAtBeginning();
	GroupChat chat = readChat();
	int result = 0;

	while (chat.getId() != chatId)
	{
		if(chatFileHandler->file.eof()) {
			chatFileHandler->file.clear();
			return -1;
		}
		result = chatFileHandler->file.tellg();
		chat = readChat();
	}

	chatFileHandler->file.clear();
	chatFileHandler->file.seekg(index);
	return result;
}

void GroupChatFileHandler::updateChatMatcher(unsigned id, unsigned adminId, bool shouldDeleteUser) {
	FileHandler* output = FileFactory::createFileHandler(Config::fileExtension);
	output->open(Config::getFile(3).c_str());

	if(!output->isOpen()) throw std::runtime_error("Failed to open temporary file for writing");

	int index = chatFileHandler->setAtBeginning();
	int bytes = 0;
	GroupChat chat = readChat(bytes);

	while(chatFileHandler->file) {
		if(chat.getId() != id) {
			chatFileHandler->copyBytes(output->file, bytes);
		} else {
			if(!shouldDeleteUser) {
				chat.setAdminId(adminId);
				saveChat(chat, *output);
			}
		}
		chat = readChat(bytes);
	}

	delete output;

	chatFileHandler->changeFile(Config::getFile(3).c_str(), Config::getFile(5).c_str());
	if(index < chatFileHandler->getFileSize()) {
		chatFileHandler->file.seekg(index);
	}
}

void GroupChatFileHandler::deleteChat(unsigned chatId) {
	updateChatMatcher(chatId, 0, true);
}

GroupChat GroupChatFileHandler::getChat(unsigned chatId) {
	int pos = findChat(chatId);
	if(pos == -1)  {
		throw std::runtime_error("Chat was not found.");
	}

	int current = chatFileHandler->file.tellg();
	chatFileHandler->file.seekg(pos);
	GroupChat chat = readChat();
	chatFileHandler->file.seekg(current);

	return chat;
}

void GroupChatFileHandler::setGroupAdmin(unsigned chatId, unsigned newAdminId) {
	updateChatMatcher(chatId, newAdminId, false);
}

void GroupChatFileHandler::addMember(const GroupChat& chat, unsigned memberId) {
	const unsigned membersIds[1] = {memberId};
	addMembers(chat, membersIds, 1);
}

void GroupChatFileHandler::deleteGroupMembers(unsigned chatId) {

}

void GroupChatFileHandler::deleteMember(unsigned chatId, unsigned memberId) {

}

void GroupChatFileHandler::findMember(unsigned chatId, unsigned memberId) {

}

void GroupChatFileHandler::addMembers(const GroupChat& chat,const unsigned memberIds[], unsigned length) {

}