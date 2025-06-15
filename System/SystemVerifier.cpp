/*
	Name: Maksim Hristov
	FN: 4MI0600466
*/

#include "SystemVerifier.h"

SystemVerifier::SystemVerifier() : 
	userFileHandler(UserFileHandler::getInstance(Config::getFile(0))) ,
	groupChatFileHandler(
		GroupChatFileHandler::getInstance(Config::getFile(5), Config::getFile(6))
	)
{ }

void SystemVerifier::requireLogged(const User* user) const {
	if(user == nullptr) {
		throw std::runtime_error("Access denied: user not logged in.");
	}
}

void SystemVerifier::requireLoggedOut(const User* user) const {
	if(user != nullptr) {
		throw std::runtime_error("You must logout first.");
	}
}

void SystemVerifier::requireAdmin(const User* user) const {
	if(user->getRole() != UserType::Admin) {
		throw std::runtime_error("Access denied: requires Admin role.");
	}
}

void SystemVerifier::requireClient(const User* user) const {
	if(user->getRole() != UserType::Client) {
		throw std::runtime_error("Access denied: requires Client role.");
	}
}

void SystemVerifier::requireUniqueUsername(const String& name) {
	if(userFileHandler.findUserByName(name) != -1) {
		throw std::runtime_error("User with that name already exists");
	}
}

void SystemVerifier::requireNotAdmin(unsigned id) {
	User* searchedUser = userFileHandler.getUser(id);

	if(searchedUser->getRole() == UserType::Admin) {
		delete searchedUser;
		throw std::runtime_error("Admin can not be deleted.");
	}
}

void SystemVerifier::requireMinimumMembers(unsigned length) const {
	if(length <= 2) {
		throw std::runtime_error("You need to add at least 2 users in order to create a group");
	}
}

void SystemVerifier::requireUniqueMembers(const String names[], unsigned length) const {
	for(unsigned i = 0; i < length; ++i) {
		for(unsigned j = i + 1; j < length; ++j) {
			if(names[i] == names[j]) {
				throw std::invalid_argument("Duplicate member names found!");
			}
		}
	}
}

void SystemVerifier::requireChatAdmin(unsigned chatId, unsigned adminId) {
	GroupChat chat = groupChatFileHandler.getChat(chatId);
	if(chat.getAdminId() != adminId) {
		throw std::runtime_error("Access denied. Only chat admins can perform this operation.");
	}
}

void SystemVerifier::requireNotChatAdmin(unsigned chatId, unsigned adminId) {
	GroupChat chat = groupChatFileHandler.getChat(chatId);
	if(chat.getAdminId() == adminId) {
		throw std::runtime_error("You can not leave this chat group, because you are the admin of the group. change the admin and then leave the group.");
	}
}

void SystemVerifier::requireUserInChat(unsigned chatId, unsigned userId) {
	int pos = groupChatFileHandler.findMember(chatId, userId);
	if(pos == -1) {
		throw std::runtime_error("User is not part of this chat");
	}
}

void SystemVerifier::requireUserNotInChat(unsigned chatId, unsigned userId) {
	int pos = groupChatFileHandler.findMember(chatId, userId);
	if(pos != -1) {
		throw std::runtime_error("User is part of this chat");
	}
}

void SystemVerifier::requireUserInChatOrAdmin(unsigned chatId, const User* user) {
	bool valid = true;
	try {
		requireUserInChat(chatId, user->getId());
	} catch (...) {
		valid = false;
	}

	if(!valid) {
		try {
			requireAdmin(user);
			valid = true;
		} catch (...) {
			valid = false;
		}
	}

	if(!valid) {
		throw std::runtime_error("Access denied.");
	}
}