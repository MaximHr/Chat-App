/*
	Name: Maksim Hristov
	FN: 4MI0600466
*/

#pragma once
#include "./BaseChat.h"

class IndividualChat : public BaseChat {
	unsigned usersIds[2];
public:
	IndividualChat() = default;
	IndividualChat(unsigned id, unsigned arr[2]);
	ChatType getType() const override;
	const unsigned* getUsersIds() const;
};