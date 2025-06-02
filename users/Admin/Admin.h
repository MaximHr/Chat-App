/*
	Name: Maksim Hristov
	FN: 4MI0600466
*/

#pragma once
#include "../User.h"
#include "../../Utils/Config.h"
#include <iostream>

// in this case its unnecessary to have two classes - Admin and Client, because User can have property role and handle everything.
// However I have decided to implement those classes to showcase my knowedge and because in the future admin and client might have different properties or methods

class Admin: public User {
public:
	Admin() = default;
	Admin(const String& name, const String& password, unsigned idCounter);
	UserType getRole() const override;
	User* clone() const override;
};