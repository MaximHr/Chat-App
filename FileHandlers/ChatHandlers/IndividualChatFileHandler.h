/*
	Name: Maksim Hristov
	FN: 4MI0600466
*/

#pragma once
#include "../FileHandler.h"
#include "../FileFactory.h"
#include "../../Utils/Config.h"
class IndividualChat;

class IndividualChatFileHandler {
	IndividualChatFileHandler(const IndividualChatFileHandler& other) = delete;
	IndividualChatFileHandler& operator=(const IndividualChatFileHandler& other) = delete;
	IndividualChatFileHandler() = delete;
	IndividualChatFileHandler(const String& str);
public:
	FileHandler* fileHandler;
	static IndividualChatFileHandler& getInstance(const String& str);
	~IndividualChatFileHandler();

	void saveChat(const IndividualChat& chat, FileHandler& fs);	
	int findChat(unsigned user1Id, unsigned user2Id);
	IndividualChat readChat();
};