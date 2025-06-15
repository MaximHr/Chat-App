/*
	Name: Maksim Hristov
	FN: 4MI0600466
*/

#include "GroupChat.h"

GroupChat::GroupChat(const String& name, unsigned adminId, unsigned id, unsigned membersCount) : 
	membersCount(membersCount), 
	name(name), 
	adminId(adminId), 
	BaseChat(id, membersCount, 0) 
{ }

void GroupChat::incrementMembersCount(int value) {
	membersCount += value;
}

unsigned GroupChat::getAdminId() const {
	return adminId;
}

void GroupChat::setAdminId(unsigned newAdminId) {
	adminId = newAdminId;
}

const String& GroupChat::getName() const {
	return name;
}

ChatType GroupChat::getType() const {
	return ChatType::Group;
}

unsigned GroupChat::getMembersCount() const {
	return membersCount;
}