/****************************************************************************************
 *
 *  EmuARC, Emulatore per Algoritmi su Reti di Calcolatori
 *  Copyright (C)  2009  Michele Caini
 *
 *
 *  This file is part of EmuARC.
 *
 *  EmuARC is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 *
 *  To contact me:   michele.caini@gmail.com
 *
 ***************************************************************************************/

#include "arcmsg.hpp"

message::message(const std::string& req) {
	mf["__req__"] = req;
}

message::message(const std::string& req, const std::string src) {
	mf["__req__"] = req;
	mf["__src__"] = src;
}

message::message(const std::string& req, const std::string src, const std::string dst) {
	mf["__req__"] = req;
	mf["__src__"] = src;
	mf["__dst__"] = dst;
}

message::message(const message& msg) {
	mf = msg.mf;
}

void message::set_field(const std::string& field, const std::string& value) {
	if(field == "__req__") return;
	mf[field] = value;
}

std::string message::get_field(const std::string& field) {
	if(has_field(field))
		return mf[field];
	else return "";
}

bool message::has_field(const std::string& field) {
	return (mf.find(field) != mf.end());
}

std::vector< std::string > message::fields() {
	std::vector< std::string > list;
	for(std::map< std::string, std::string >::iterator it = mf.begin(); it != mf.end(); ++it)
		list.push_back((*it).first);
	return list;
}

message& message::operator=(const message& msg) {
	if(this != &msg) {
		mf = msg.mf;
	}
	return (*this);
}

std::ostream& operator<<(std::ostream& out, const message& msg) {
	out << "MESSAGE" << std::endl;
	std::map< std::string, std::string >::iterator it;
	std::map<std::string, std::string> imf = msg.mf;
	for(it = imf.begin(); it != imf.end(); ++it)
		out << '\t' << it->first << " : " << it->second << std::endl;
	return out;
}
