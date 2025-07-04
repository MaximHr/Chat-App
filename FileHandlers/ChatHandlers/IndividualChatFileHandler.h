/*
	Name: Maksim Hristov
	FN: 4MI0600466
*/

#pragma once
#include "../FileHandler.h"
#include "../FileFactory.h"
#include "../../Utils/Config.h"
#include "../MessageHandler/MessageFileHandler.h"
#include <iostream>
class IndividualChat;

class IndividualChatFileHandler {
	IndividualChatFileHandler(const IndividualChatFileHandler& other) = delete;
	IndividualChatFileHandler& operator=(const IndividualChatFileHandler& other) = delete;
	IndividualChatFileHandler() = delete;
	IndividualChatFileHandler(const String& str);

	int findChatMatcher(unsigned user1Id, unsigned user2Id, bool shouldGetId);

public:
	MessageFileHandler messageFileHandler;
	FileHandler* fileHandler;
	static IndividualChatFileHandler& getInstance(const String& str);
	~IndividualChatFileHandler();

	void saveChat(const IndividualChat& chat, FileHandler& fs);	
	int findChat(unsigned user1Id, unsigned user2Id);
	int getChatId(unsigned user1Id, unsigned user2Id);
	IndividualChat readChat();
	void printChats(bool shouldViewAllChats, unsigned userId);
	
};