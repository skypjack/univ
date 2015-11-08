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

#ifndef ARCDRV
#define ARCDRV

#include <map>
#include <queue>
#include <list>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include "arcmsg.hpp"
#include "arcobs.hpp"

class channel: public observable<message> {
private:
	boost::mutex ch_mutex;
public:
	virtual ~channel() {}
	virtual void send(message msg);
};

class driver: public observer<message> {
private:
	std::string id;
	std::list< channel* > chs;
	std::map< std::string, channel* > entities;
	std::queue< message > msgs;

	boost::condition msgs_available;
	boost::mutex msgs_queue_mutex;
public:
	driver(std::string iid): id(iid) {}

	void sync();

	std::string identifier();
	void add_input_channel(channel* ch);
	void add_output_channel(channel* ch);
	void add_bidirectional_channel(channel* ch);

	std::list< std::string > neighbours();
	int neighbours_size();

	void send(const std::string& entity, message msg);
	void send_all(message msg);
	void send_all_restricted(const std::string& entity, message msg);
	message receive();
	bool has_message();

	void update(observable<message>* obs);
	void update(observable<message>* obs, message* data);
};

#endif
