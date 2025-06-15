/*
	Name: Maksim Hristov
	FN: 4MI0600466
*/

#pragma once
#include "../../Utils/String.h"
#include "../../FileHandlers/ChatHandlers/GroupChatFileHandler.h"
#include "./BaseChat.h"

class GroupChat : public BaseChat {
	String name;
	unsigned adminId;
	unsigned membersCount;
	friend GroupChat GroupChatFileHandler::readChat();
public:
	GroupChat() = default;
	GroupChat(const String& name, unsigned adminId, unsigned id, unsigned membersCount);
	
	ChatType getType() const override;
	void incrementMembersCount(int value);
	unsigned getAdminId() const;
	unsigned getMembersCount() const;
	const String& getName() const;
	void setAdminId(unsigned newAdminId);
};