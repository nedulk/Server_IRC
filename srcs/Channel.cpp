//
// Created by gangouil on 7/11/24.
//

#include "Channel.hpp"

//void Channel::isValidName(std::string name)
//{
//	if (name.size() > 200)
//		throw (std::runtime_error("Channel name too long"));
//	if (name[0] != '&' && name[0] != '#')
//		throw (std::runtime_error("Channel prefix invalid"));
//	if (name.find_first_of(" ,") != std::string::npos || name.find(7) != std::string::npos)
//		throw (std::runtime_error("Channel name contains space, comma or ^G"));
//}
//
//Channel::Channel(std::string name)
//{
//	isValidName(name);
//	_name = name;
//}
//
//Channel::Channel(std::string name, std::string key)
//{
//	isValidName(name);
//	_name = name;
//	_key = key;
//}
//
//std::map<int, Client *> Channel::getUserList() {
//	return (_userList);
//}
