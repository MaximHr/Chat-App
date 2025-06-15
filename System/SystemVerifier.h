/*
	Name: Maksim Hristov
	FN: 4MI0600466
*/

#pragma once
#include "../Users/User.h"
#include "../Components/Chats/GroupChat.h"
#include "../FileHandlers/UserHandler/UserFileHandler.h"

class SystemVerifier {
private:
	UserFileHandler& userFileHandler;
	GroupChatFileHandler& groupChatFileHandler;
public:
	SystemVerifier();

	void requireLogged(const User* user) const;
	void requireLoggedOut(const User* user) const;
	void requireAdmin(const User* user) const;
	void requireClient(const User* user) const;
	void requireUniqueUsername(const String& name);
	void requireNotAdmin(unsigned id);
	void requireMinimumMembers(unsigned length) const;
	void requireUniqueMembers(const String names[], unsigned length) const;
	void requireChatAdmin(unsigned chatId, unsigned adminId);
	void requireUserInChat(unsigned chatId, unsigned userId);
	void requireUserInChatOrAdmin(unsigned chatId, const User* user);
};