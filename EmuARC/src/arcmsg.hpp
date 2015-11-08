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

#ifndef ARCMSG
#define ARCMSG

#include <vector>
#include <string>
#include <map>
#include <iostream>

class message {
private:
	std::map<std::string, std::string> mf;
public:
	message(const std::string& req);
	message(const std::string& req, const std::string src);
	message(const std::string& req, const std::string src, const std::string dst);
	message(const message& msg);
	inline std::string request() { return mf["__req__"]; }
	inline void set_source(const std::string src) { mf["__src__"] = src; }
	inline std::string source() { if(has_field("__src__")) return mf["__src__"]; else return ""; }
	inline void set_destination(const std::string dst) { mf["__dst__"] = dst; }
	inline std::string destination() { if(has_field("__dst__")) return mf["__dst__"]; else return ""; }
	inline void set_broadcast() { mf["__brd__"] = ""; }
	inline bool broadcast() { return has_field("__brd__"); }
	void set_field(const std::string& field, const std::string& value);
	std::string get_field(const std::string& field);
	bool has_field(const std::string& field);
	std::vector< std::string > fields();
	message& operator=(const message& msg);
	friend std::ostream& operator<<(std::ostream& out, const message& msg);
};

#endif
