/*
	Name: Maksim Hristov
	FN: 4MI0600466
*/

#include "Config.h"

const unsigned Config::adminId = 0;
const String Config::adminPassword = "0000";

const FileExtension Config::fileExtension = FileExtension::txt;
const String Config::fileNames[Config::numberOfFiles] = {
	"users",
	"ids",
	"messages",
	"temp",
	"individual_chats",
	"group_chats",
	"group_members",
};

const String Config::getFileExtension() {
	String str;
	if(fileExtension == FileExtension::dat) {
		str = ".dat";
	} else if(fileExtension == FileExtension::txt) {
		str = ".txt";
	}
	return str;
}

const String Config::getFile(unsigned i) {
	if(i >= numberOfFiles) {
		throw std::invalid_argument("invalid index");
	}
	String str = fileNames[i];
	str += getFileExtension();
	return str;
}