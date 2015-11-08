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

#include "arcdrv.hpp"

void channel::send(message msg) {
	boost::mutex::scoped_lock ch_lock(ch_mutex);
	notify(&msg);
}


void driver::sync() {
	message msg("__SYNC__");
	send_all(msg);
}

std::string driver::identifier() {
	return id;
}

void driver::add_input_channel(channel* ch) {
	ch->attach(this);
}

void driver::add_output_channel(channel* ch) {
	std::list< channel* >::iterator it;
	for(it = chs.begin(); it != chs.end() && (*it) != ch; ++it);
	if(it == chs.end())
		chs.push_back(ch);
}

void driver::add_bidirectional_channel(channel* ch) {
	add_input_channel(ch);
	add_output_channel(ch);
}

std::list< std::string > driver::neighbours() {
	std::list< std::string > list;
	std::map< std::string, channel* >::iterator it;
	for(it = entities.begin(); it != entities.end(); ++it) {
		list.push_back(it->first);
	}
	return list;
}

int driver::neighbours_size() {
	return entities.size();
}

void driver::send(const std::string& entity, message msg) {
	std::map< std::string, channel* >::iterator it = entities.find(entity);
	if(it != entities.end()) {
		msg.set_source(identifier());
		msg.set_destination(entity);
		(it->second)->send(msg);
	}
}

void driver::send_all(message msg) {
	msg.set_source(identifier());
	msg.set_broadcast();
	std::list< channel* >::iterator it;
	for(it = chs.begin(); it != chs.end(); ++it) {
		(*it)->send(msg);
	}
}

void driver::send_all_restricted(const std::string& entity, message msg) {
	msg.set_source(identifier());
	std::map< std::string, channel* >::iterator it;
	for(it = entities.begin(); it != entities.end(); ++it) {
		if(it->first != entity) {
			msg.set_destination(it->first);
			(it->second)->send(msg);
		}
	}
}

message driver::receive() {
	boost::mutex::scoped_lock queue_lock(msgs_queue_mutex);
	if(msgs.empty())
		msgs_available.wait(queue_lock);
	message msg = msgs.front();
	msgs.pop();
	return msg;
}

bool driver::has_message() {
	boost::mutex::scoped_lock queue_lock(msgs_queue_mutex);
	return (!msgs.empty());
}

void driver::update(observable<message>* obs) {}

void driver::update(observable<message>* obs, message* data) {
	if((data->request() == "__SYNC__") && (data->source() != identifier())) {
		std::list< channel* >::iterator it;
		for(it = chs.begin(); it != chs.end() && (*it) != obs; ++it);
		if(it != chs.end())
			entities[data->source()] = (*it);
	} else if((data->broadcast() && (data->source() != identifier())) ||
				(!data->broadcast() && (data->destination() == identifier()))) {
			boost::mutex::scoped_lock queue_lock(msgs_queue_mutex);
			message msg(*data);
			msgs.push(msg);
			msgs_available.notify_all();
	}
}
