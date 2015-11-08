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

#include <sstream>
#include "arcobs.hpp"
#include "arcmsg.hpp"
#include "arcdrv.hpp"
#include "arcent.hpp"

class spec_big_brother: public big_brother {
private:
	int msg_cnt;
	std::map< std::string, int > msg_req;
	std::map< std::string, int > msg_src;
	std::map< std::string, int > msg_dst;
public:
	spec_big_brother():
		msg_cnt(0) {}

	void update(observable<message>* obs, message* data) {
		if(!drv_sync_msg(data)) {
			++msg_cnt;
			msg_req[data->request()] = msg_req[data->request()] + 1;
			if(data->source() != "") {
				msg_src[data->source()] =
					msg_src[data->source()] + 1;
			}
			if(data->destination() != "") {
				msg_dst[data->destination()] =
					msg_dst[data->destination()] + 1;
			}
		}
	}

	friend std::ostream& operator<<(std::ostream& out, const spec_big_brother& sbb) {
		out << "RESULTS BEGIN:" << std::endl;
		out << "\t msg: " << sbb.msg_cnt << std::endl;
		std::map< std::string, int >::iterator it;
		std::map< std::string, int > data = sbb.msg_req;
		for(it = data.begin(); it != data.end(); ++it) {
			out << "\t req --> " << it->first << " : " << it->second << std::endl;
		}
		data = sbb.msg_src;
		for(it = data.begin(); it != data.end(); ++it) {
			out << "\t src --> " << it->first << " : " << it->second << std::endl;
		}
		data = sbb.msg_dst;
		for(it = data.begin(); it != data.end(); ++it) {
			out << "\t dst --> " << it->first << " : " << it->second << std::endl;
		}
		out << "RESULTS END" << std::endl;
		return out;
	}
};



class qqnono_spontaneous_process: public process {
private:
	std::vector< std::string > tree_neighbours;
	int cnt;

	bool condition;
public:
	qqnono_spontaneous_process(driver* drv):
		process(drv), cnt(0), condition(true) {}
	inline bool spontaneous_condition() { return condition; }
	void spontaneous() {
		message msg("Q");
		drv->send_all(msg);
		condition = false;
	}
	void body() {
		while(cnt < drv->neighbours_size()) {
			message msg = drv->receive();
			if(msg.request() == "Q") {
				++cnt;
			}
			if(msg.request() == "QY") {
				tree_neighbours.push_back(msg.source());
				++cnt;
			}
		}
		std::cout << drv->identifier() << " is root" << std::endl;
		std::cout << "neighbours: ";
		std::vector< std::string >::iterator it;
		for(it = tree_neighbours.begin(); it != tree_neighbours.end(); ++it) {
			std::cout << '\t' << *it << std::endl;
		}
	}
};

class qqnono_process: public process {
private:
	std::string parent;
	std::vector< std::string > tree_neighbours;
	int cnt;
	bool active;
public:
	qqnono_process(driver* drv):
		process(drv), cnt(0), active(false) {}
	void body() {
		while(cnt < drv->neighbours_size()) {
			message msg = drv->receive();
			if(active && msg.request() == "Q") {
				++cnt;
			}
			if(!active && msg.request() == "Q") {
				parent = msg.source();
				tree_neighbours.push_back(msg.source());
				message reply("QY");
				drv->send(msg.source(), reply);
				++cnt;
				if(cnt < drv->neighbours_size()) {
					message query("Q");
					drv->send_all_restricted(msg.source(), query);
					active = true;
				}
			}
			if(active && msg.request() == "QY") {
				tree_neighbours.push_back(msg.source());
				++cnt;
			}
		}
		std::cout << drv->identifier() << "\'s parent " << parent << std::endl;
		std::cout << "neighbours: ";
		std::vector< std::string >::iterator it;
		for(it = tree_neighbours.begin(); it != tree_neighbours.end(); ++it) {
			std::cout << '\t' << *it << std::endl;
		}
	}
};

void qqnono_emu() {
	/*
	 * Stella di David:
	 *    - indice 0 alto-sinistra
	 *    - numerazione continua con orientamento orario
	 *    - urlatore in posizione 11
	 */
	int nodes = 12;
	int edges = 3*(nodes/2);

	spec_big_brother bb;

	driver* drvs[nodes];
	process* sp[nodes];
	for(int i = 0; i < (nodes - 1); ++i) {
		std::stringstream ss;
		ss << i;
		std::string name = "qqnono-" + ss.str();
		drvs[i] = new driver(name);
		sp[i] = new qqnono_process(drvs[i]);
	}
	drvs[nodes - 1] = new driver("qqnono-spontaneous");
	sp[nodes - 1] =
		new qqnono_spontaneous_process(drvs[nodes - 1]);

	channel* chs[edges];
	for(int i = 0; i < (nodes/2); ++i) {
		chs[3*i] = new channel();
		chs[3*i]->attach(&bb);
		drvs[2*i]->add_bidirectional_channel(chs[3*i]);
		drvs[(2*i - 1 + nodes)%nodes]->add_bidirectional_channel(chs[3*i]);
		//
		chs[3*i + 1] = new channel();
		chs[3*i + 1]->attach(&bb);
		drvs[2*i]->add_bidirectional_channel(chs[3*i + 1]);
		drvs[(2*i + 1)%nodes]->add_bidirectional_channel(chs[3*i + 1]);
		//
		chs[3*i + 2] = new channel();
		chs[3*i + 2]->attach(&bb);
		drvs[(2*i - 1 + nodes)%nodes]->add_bidirectional_channel(chs[3*i + 2]);
		drvs[(2*i + 1)%nodes]->add_bidirectional_channel(chs[3*i + 2]);
	}

	for(int i = 0; i < nodes; ++i)
		drvs[i]->sync();

	boost::thread* ths[nodes];
	for(int i = 0; i < nodes - 1; ++i)
		ths[i] = new boost::thread(*(static_cast<qqnono_process*>(sp[i])));
	ths[nodes - 1] = new boost::thread(*(static_cast<qqnono_spontaneous_process*>(sp[nodes - 1])));
	for(int i = 0; i < nodes; ++i)
			ths[i]->join();

	std::cout << bb << std::endl;

	for(int i = 0; i < edges; ++i)
		delete chs[i];
	for(int i = 0; i < nodes; ++i) {
		delete ths[i];
		delete drvs[i];
		delete sp[i];
	}
}



class shout_spontaneous_process: public process {
private:
	bool condition;
public:
	shout_spontaneous_process(driver* drv):
		process(drv), condition(true) {}
	inline bool spontaneous_condition() { return condition; }
	void spontaneous() {
		condition = false;
		message msg("INFO");
		drv->send_all(msg);
	}
	void body() {
		std::cout << drv->identifier() << " done !!" << std::endl;
	}
};

class shout_process: public process {
private:
	bool done;
public:
	shout_process(driver* drv):
		process(drv), done(false) {}
	void body() {
		while(!done) {
			message msg = drv->receive();
			if(msg.request() == "INFO") {
				drv->send_all_restricted(msg.source(), msg);
				std::cout << drv->identifier() << " done !!" << std::endl;
				done = true;
			}
		}
	}
};

void shout_emu() {
	/*
	 * Stella di David:
	 *    - indice 0 alto-sinistra
	 *    - numerazione continua con orientamento orario
	 *    - urlatore in posizione 11
	 */
	int nodes = 12;
	int edges = 3*(nodes/2);

	spec_big_brother bb;

	driver* drvs[nodes];
	process* sp[nodes];

	for(int i = 0; i < (nodes - 1); ++i) {
		std::stringstream ss;
		ss << i;
		std::string name = "shout-" + ss.str();
		drvs[i] = new driver(name);
		sp[i] = new shout_process(drvs[i]);
	}
	drvs[nodes - 1] = new driver("shout-spontaneous");
	sp[nodes - 1] =
		new shout_spontaneous_process(drvs[nodes - 1]);

	channel* chs[edges];
	for(int i = 0; i < (nodes/2); ++i) {
		chs[3*i] = new channel();
		chs[3*i]->attach(&bb);
		drvs[2*i]->add_bidirectional_channel(chs[3*i]);
		drvs[(2*i - 1 + nodes)%nodes]->add_bidirectional_channel(chs[3*i]);
		//
		chs[3*i + 1] = new channel();
		chs[3*i + 1]->attach(&bb);
		drvs[2*i]->add_bidirectional_channel(chs[3*i + 1]);
		drvs[(2*i + 1)%nodes]->add_bidirectional_channel(chs[3*i + 1]);
		//
		chs[3*i + 2] = new channel();
		chs[3*i + 2]->attach(&bb);
		drvs[(2*i - 1 + nodes)%nodes]->add_bidirectional_channel(chs[3*i + 2]);
		drvs[(2*i + 1)%nodes]->add_bidirectional_channel(chs[3*i + 2]);
	}

	for(int i = 0; i < nodes; ++i)
		drvs[i]->sync();

	boost::thread* ths[nodes];
	for(int i = 0; i < nodes - 1; ++i)
		ths[i] = new boost::thread(*(static_cast<shout_process*>(sp[i])));
	ths[nodes - 1] = new boost::thread(*(static_cast<shout_spontaneous_process*>(sp[nodes - 1])));
	for(int i = 0; i < nodes; ++i)
		ths[i]->join();

	std::cout << bb << std::endl;

	for(int i = 0; i < edges; ++i)
		delete chs[i];
	for(int i = 0; i < nodes; ++i) {
		delete ths[i];
		delete drvs[i];
		delete sp[i];
	}
}



int main(int argc, char** argv) {
	char c;
	std::cout << "SHOUT EMU" << std::endl;
	shout_emu();
	std::cin >> c;
	std::cout << std::endl << "QQNO EMU" << std::endl;
	qqnono_emu();
	return 0;
}
