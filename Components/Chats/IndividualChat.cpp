#include "IndividualChat.h"


IndividualChat::IndividualChat(unsigned id, unsigned arr[2]) : BaseChat(id, 2, 0) {
	usersIds[0] = arr[0];
	usersIds[1] = arr[1];
}

ChatType IndividualChat::getType() const {
	return ChatType::Individual;
}

const unsigned* IndividualChat::getUsersIds() const {
	return usersIds;
}