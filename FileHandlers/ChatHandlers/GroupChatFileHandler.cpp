/*
	Name: Maksim Hristov
	FN: 4MI0600466
*/

#include "GroupChatFileHandler.h"
#include "../../Components/Chats/GroupChat.h"

GroupChatFileHandler::GroupChatFileHandler(const String& chatFile, const String& membersFile) : messageFileHandler(Config::getFile(2)){
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

void GroupChatFileHandler::updateChatMatcher(unsigned id, unsigned adminId, GroupChatUpdates type, int value) {
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
			if(type == GroupChatUpdates::changeAdmin) {
				chat.setAdminId(adminId);
				saveChat(chat, *output);
			} else if(type == GroupChatUpdates::changeMembersCount) {
				chat.incrementMembersCount(value);
				saveChat(chat, *output);
			} else if(type == GroupChatUpdates::changeMessagesCount) {
				chat.incrementMessagesCount();
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

void GroupChatFileHandler::updateMessagesCount(unsigned chatId) {
	updateChatMatcher(chatId, 0, GroupChatUpdates::changeMessagesCount, 1);
}

void GroupChatFileHandler::changeGroupMembers(unsigned chatId, unsigned value) {
	updateChatMatcher(chatId, 0, GroupChatUpdates::changeMembersCount, value);
}

void GroupChatFileHandler::deleteChat(unsigned chatId) {
	updateChatMatcher(chatId, 0, GroupChatUpdates::deleteUser, 0);
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
	updateChatMatcher(chatId, newAdminId, GroupChatUpdates::changeAdmin, 0);
}

void GroupChatFileHandler::updateMembersMatcher(unsigned chatId, unsigned searchedId, GroupMemberUpdates type) {
	FileHandler* output = FileFactory::createFileHandler(Config::fileExtension);
	output->open(Config::getFile(3).c_str());

	if(!output->isOpen()) throw std::runtime_error("Failed to open temporary file for writing");

	int index = membersFileHandler->setAtBeginning();
	MemberDetails memberDetails;

	while (true) {
		membersFileHandler->read(memberDetails.chatId);
		if (!membersFileHandler->file) break; // Check stream state after read

		membersFileHandler->read(memberDetails.membersCount);
		if (!membersFileHandler->file) break;

		if (memberDetails.chatId == chatId) {
			if (type != GroupMemberUpdates::deleteChatMembers) {
				output->write(memberDetails.chatId);
			} else {
				membersFileHandler->skipIds(sizeof(unsigned) * memberDetails.membersCount);
			}
			if (type == GroupMemberUpdates::deleteMember) {
				output->write(memberDetails.membersCount - 1);
				for (int i = 0; i < memberDetails.membersCount; i++) {
					unsigned memberId;
					membersFileHandler->read(memberId);
					if (!membersFileHandler->file) break;
					if (memberId != searchedId) {
						output->write(memberId);
					}
				}
			}
			if (type == GroupMemberUpdates::addMember) {
				output->write(memberDetails.membersCount + 1);
				for (int i = 0; i < memberDetails.membersCount; i++) {
					unsigned memberId;
					membersFileHandler->read(memberId);
					if (!membersFileHandler->file) break;
					output->write(memberId);
				}
				output->write(searchedId);
			}
		} else {
			output->write(memberDetails.chatId);
			output->write(memberDetails.membersCount);
			for (int i = 0; i < memberDetails.membersCount; i++) {
				unsigned memberId;
				membersFileHandler->read(memberId);
				if (!membersFileHandler->file) break;
				output->write(memberId);
			}
		}
	};

	delete output;
	membersFileHandler->file.clear();
	membersFileHandler->changeFile(Config::getFile(3).c_str(), Config::getFile(6).c_str());
	if(index < membersFileHandler->getFileSize()) {
		membersFileHandler->file.seekg(index);
	}
}

int GroupChatFileHandler::findMember(unsigned chatId, unsigned searchedId) {
	if(!membersFileHandler->isOpen()) throw std::runtime_error("file cannot be opened");
	if(membersFileHandler->getFileSize() == 0) return -1;

	int index = membersFileHandler->setAtBeginning();
	int result = 0;
	MemberDetails memberDetails;

	do {
		if(membersFileHandler->file.eof()) {
			membersFileHandler->file.clear();
			membersFileHandler->file.seekg(index);
			return -1;
		}
		membersFileHandler->read(memberDetails.chatId);
		membersFileHandler->read(memberDetails.membersCount);
		if(memberDetails.chatId != chatId) {
			membersFileHandler->skipIds(sizeof(unsigned) * memberDetails.membersCount);
		} else {
			for(int i = 0;i < memberDetails.membersCount;i++) {
				unsigned memberId;
				membersFileHandler->read(memberId);
				if(searchedId == memberId) {
					return i;
				}
			}
		}
		
	} while(true);

	membersFileHandler->file.clear();
	membersFileHandler->file.seekg(index);
}
	
void GroupChatFileHandler::addMember(unsigned chatId, unsigned memberId) {
	updateMembersMatcher(chatId, memberId, GroupMemberUpdates::addMember);
	changeGroupMembers(chatId, +1);
}

void GroupChatFileHandler::deleteGroupMembers(unsigned chatId) {
	updateMembersMatcher(chatId, 0, GroupMemberUpdates::deleteChatMembers);
}

void GroupChatFileHandler::deleteMember(unsigned chatId, unsigned memberId) {
	updateMembersMatcher(chatId, memberId, GroupMemberUpdates::deleteMember);
	changeGroupMembers(chatId, -1);
}

void GroupChatFileHandler::saveMembers(const GroupChat& chat, FileHandler& fs , const unsigned memberIds[], unsigned length) {
	if(!chatFileHandler->isOpen()) throw std::runtime_error("file can not be opened");
	fs.write(chat.getId());
	fs.write(length);
	for(int i = 0;i < length;i++) {
		fs.write(memberIds[i]);
	}
	fs.file.flush();
}

void GroupChatFileHandler::printChats(bool shouldViewAllChats, unsigned userId) {
	if(!chatFileHandler->isOpen()) throw std::runtime_error("file cannot be opened");
	if(chatFileHandler->getFileSize() == 0)  {
		std::cout << "No group chats yet" << '\n';
	};

	int index = chatFileHandler->setAtBeginning();
	GroupChat chat = readChat();
	while (!chatFileHandler->file.eof())
	{
		if(shouldViewAllChats) {
			std::cout << "Group chat "<< chat.getName() << " (" << chat.getMembersCount() << " members)" << '\n';
		} else {
			int pos = findMember(chat.getId(), userId);
			if(pos != -1) {
				std::cout << "Group chat "<< chat.getName() << " (" << chat.getMembersCount() << " members)" << '\n';
			}
		}
		chat = readChat();
	}

	chatFileHandler->file.clear();
	chatFileHandler->file.seekg(index);
}