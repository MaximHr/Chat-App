/*
	Name: Maksim Hristov
	FN: 4MI0600466
*/

#include "../Utils/Config.h"
#include "System.h"

System::System() : 
	user(nullptr),
	userFileHandler(UserFileHandler::getInstance(Config::getFile(0))),
	messageFileHandler(MessageFileHandler::getInstance(Config::getFile(2))),
	individualChatFileHandler(IndividualChatFileHandler::getInstance(Config::getFile(4))),
	groupChatFileHandler(
		GroupChatFileHandler::getInstance(Config::getFile(5), Config::getFile(6)) 
	),
	idContainer(Config::getFile(1), Config::fileNames, Config::numberOfFiles) 
{ }

System& System::getInstance() {
	static System instance;
	return instance;
}

unsigned System::addUser(UserType type, const String& name, const String& password) {
	verifier.requireLoggedOut(user);
	verifier.requireUniqueUsername(name);

	unsigned id = idContainer.getId(Config::getFile(0));
	User* newUser = UserFactory::createUser(type, name, password, id);

	userFileHandler.saveUser(newUser, *userFileHandler.fileHandler);
	idContainer.increment(Config::getFile(0));
	delete newUser;

	return id;
}

void System::login(unsigned id, const String& password) {
	verifier.requireLoggedOut(user);
	user = userFileHandler.getUserByPassword(id, password.reverse());
}

void System::logout() {
	delete user;
	user = nullptr;
}

void System::deleteUser(unsigned id) {
	verifier.requireLogged(user);
	verifier.requireAdmin(user);
	verifier.requireNotAdmin(id);
	
	int fileSize = userFileHandler.fileHandler->getFileSize();
	userFileHandler.deleteUser(id);
	
	if(fileSize != userFileHandler.fileHandler->getFileSize()) {
		std::cout << "User deleted succefully!" << '\n';
	} else {
		std::cout << "No user with that id found. " << '\n';
	}
}

void System::changePassword(const String& password) {
	verifier.requireLogged(user);

	const User* updatedUser = UserFactory::createUser(user->getRole(), user->getName(), password, user->getId());
	userFileHandler.updateUser(user->getId(), updatedUser);
	delete updatedUser;
}

bool System::doesUserExist(const String& name) {
	bool exists = false;
	try {
		verifier.requireUniqueUsername(name);
	} catch(const std::exception& e) {
		exists = true;
	}
	return exists;
}

void System::createGroup(const String& groupName, String members[], unsigned length) {
	verifier.requireLogged(user);
	verifier.requireMinimumMembers(length);

	members[0] = user->getName();
	verifier.requireUniqueMembers(members, length);

	GroupChat chat (
		groupName, 
		user->getId(), // by default, the creator is the admin of the group
		idContainer.getId(Config::getFile(5)),
		length
	);
	groupChatFileHandler.saveChat(chat, *groupChatFileHandler.chatFileHandler);
	idContainer.increment(Config::getFile(5));

	unsigned membersIds[length];
	User* currentMember = nullptr;

	for(int i = 0;i < length;i++) {
		currentMember = userFileHandler.getUserByName(members[i]);
		membersIds[i] = currentMember->getId();
		delete currentMember;
	}
	groupChatFileHandler.addMembers(chat, membersIds, length);

}

void System::showGroupStats(unsigned chatId) {
	verifier.requireLogged(user);
	verifier.requireUserInChatOrAdmin(chatId, user);
	GroupChat chat = groupChatFileHandler.getChat(chatId);
	std::cout << chat.getName() << ": " << '\n';
	std::cout << "Chat Id: " << chat.getId() << '\n';
	std::cout << "Total members: " <<  chat.getMembersCount() << '\n';
	std::cout << "Total messages:" << chat.getMessagesCount() << '\n';
}

void System::deleteGroup(unsigned chatId) {
	verifier.requireLogged(user);
	verifier.requireAdmin(user);
	groupChatFileHandler.deleteChat(chatId);
	groupChatFileHandler.deleteGroupMembers(chatId);
}

void System::setGroupAdmin(unsigned chatId, unsigned adminId) {
	verifier.requireLogged(user);
	verifier.requireChatAdmin(chatId, user->getId());
	verifier.requireUserInChat(chatId, adminId);
	groupChatFileHandler.setGroupAdmin(chatId, adminId);
}

void System::messageIndividual(const String& name) {
	verifier.requireLogged(user);
	User* reciever = userFileHandler.getUserByName(name);
	// int pos = individualChatFileHandler.findChat(user->getName(), name);
	// if(pos == -1) {
	// 	IndividualChat chat = individualChatFileHandler.readChat();
	// 	individualChatFileHandler.saveChat(chat, *chatDispatcher.fileHandler);
	// } else {
	// 	//printMessages and call enterMessage
	// }

	delete reciever;
}

System::~System() {
	delete user;
	user = nullptr;
}
