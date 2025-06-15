/*
	Name: Maksim Hristov
	FN: 4MI0600466
*/

#pragma once
#include "../FileHandler.h"
#include "../FileFactory.h"
#include "../../Utils/Config.h"

class GroupChat;

enum class GroupChatUpdates {
	deleteUser,
	changeAdmin,
	changeMembersCount
};

enum class GroupMemberUpdates {
	deleteChatMembers,
	deleteMember,
	addMember
};

struct MemberDetails {
	unsigned chatId;
	unsigned membersCount;
};

class GroupChatFileHandler {
	GroupChatFileHandler(const GroupChatFileHandler& other) = delete;
	GroupChatFileHandler& operator=(const GroupChatFileHandler& other) = delete;
	GroupChatFileHandler() = delete;
	GroupChatFileHandler(const String& chatFile, const String& membersFile);
	
	void updateChatMatcher(unsigned id, unsigned adminId, GroupChatUpdates type, int value);
	void updateMembersMatcher(unsigned chatId, unsigned searchedId, GroupMemberUpdates type);
	void changeGroupMembers(unsigned chatId, unsigned value);
public:
	FileHandler* chatFileHandler;
	FileHandler* membersFileHandler;
	static GroupChatFileHandler& getInstance(const String& chatFile, const String& membersFile);
	~GroupChatFileHandler();

	void saveChat(const GroupChat& chat, FileHandler& fs);
	GroupChat readChat();
	GroupChat readChat(int& bytes);
	void addMessage(unsigned chatId, unsigned messageId);
	int findChat(unsigned chatId);
	GroupChat getChat(unsigned chatId);
	void deleteChat(unsigned chatId);
	void setGroupAdmin(unsigned chatId, unsigned newAdminId);

	void saveMembers(const GroupChat& chat, FileHandler& fs, const unsigned memberIds[], unsigned length);
	void addMember(unsigned chatId, unsigned memberId);
	void deleteMember(unsigned chatId, unsigned memberId);
	int findMember(unsigned chatId, unsigned memberId);
	void deleteGroupMembers(unsigned chatId);

};