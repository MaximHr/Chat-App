/*
	Name: Maksim Hristov
	FN: 4MI0600466
*/

#pragma once
#include "../FileHandler.h"
#include "../FileFactory.h"
#include "../../Utils/Config.h"

class GroupChat;

class GroupChatFileHandler {
	GroupChatFileHandler(const GroupChatFileHandler& other) = delete;
	GroupChatFileHandler& operator=(const GroupChatFileHandler& other) = delete;
	GroupChatFileHandler() = delete;
	GroupChatFileHandler(const String& chatFile, const String& membersFile);
	
	void updateChatMatcher(unsigned id, unsigned adminId, bool shouldDeleteUser);
public:
	FileHandler* chatFileHandler;
	FileHandler* membersFileHandler;
	static GroupChatFileHandler& getInstance(const String& chatFile, const String& membersFile);
	~GroupChatFileHandler();

	void addMembers(const GroupChat& chat, const unsigned memberIds[], unsigned length);
	void addMember(const GroupChat& chat, unsigned memberId);
	void saveChat(const GroupChat& chat, FileHandler& fs);
	GroupChat readChat();
	GroupChat readChat(int& bytes);
	void addMessage(unsigned chatId, unsigned messageId);
	int findChat(unsigned chatId);
	GroupChat getChat(unsigned chatId);
	void deleteChat(unsigned chatId);
	void setGroupAdmin(unsigned chatId, unsigned newAdminId);
	void deleteMember(unsigned chatId, unsigned memberId);
	void findMember(unsigned chatId, unsigned memberId);
	void deleteGroupMembers(unsigned chatId);

};