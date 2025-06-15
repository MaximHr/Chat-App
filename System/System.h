/*
	Name: Maksim Hristov
	FN: 4MI0600466
*/

#pragma once
#include "../Utils/Config.h"
#include "../Utils/String.h"
#include "../Users/User.h"
#include "../Users/UserFactory.h"
#include "../IdManager/IdContainer.h"
#include "../FileHandlers/UserHandler/UserFileHandler.h"
#include "../FileHandlers/MessageHandler/MessageFileHandler.h"
#include "../FileHandlers/ChatHandlers/GroupChatFileHandler.h"
#include "../FileHandlers/ChatHandlers/IndividualChatFileHandler.h"
#include "SystemVerifier.h"
#include "../Components/Message/Message.h"
#include "../Components/Chats/GroupChat.h"
#include "../Components/Chats/IndividualChat.h"
#include <exception>

class System {
	User* user;
	SystemVerifier verifier;
	IdContainer idContainer;
	GroupChatFileHandler& groupChatFileHandler;
	IndividualChatFileHandler& individualChatFileHandler;
	UserFileHandler& userFileHandler;
	
	System();
	System(const System& other) = delete;
	System& operator=(const System& other) = delete;

public:
	static System& getInstance();
	~System();

	unsigned addUser(UserType type, const String& name, const String& password);
	void deleteUser(unsigned id);
	void login(unsigned id, const String& password);
	void logout();
	void changePassword(const String& password);
	bool doesUserExist(const String& name);
	unsigned createGroup(const String& groupName, String members[], unsigned length);
	unsigned messageIndividual(const String& name);
	void setGroupAdmin(unsigned chatId, unsigned adminId);
	void deleteGroup(unsigned chatId);
	void showGroupStats(unsigned chatId);
	void kickFromGroup(unsigned chatId, unsigned memberId);
	void addToGroup(unsigned chatId, unsigned memberId);
	void leaveGroup(unsigned chatId);
	void viewAllChats();
	void viewChats();
	void sendGroupMessage(unsigned chatId, const String& message);
	void sendIndividualMessage(unsigned chatId, const String& message);
	void printGroupMessages(unsigned chatId);

};