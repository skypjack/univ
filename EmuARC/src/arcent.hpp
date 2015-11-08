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

#ifndef ARCENT
#define ARCENT

#include <map>
#include <list>
#include <string>
#include <utility>
#include "arcmsg.hpp"
#include "arcdrv.hpp"

class big_brother: public observer<message> {
public:
	inline bool drv_sync_msg(message* msg) { return (msg->request() == "__SYNC__"); }

	virtual void update(observable<message>* obs) {}
	virtual void update(observable<message>* obs, message* data) {
		std::cout << (*data) << std::endl;
	}
};


class process {
protected:
	driver* drv;
public:
	process(driver* idrv): drv(idrv) {}
	virtual ~process() {}
	inline virtual bool spontaneous_condition() { return false; }
	virtual void spontaneous() {}
	virtual void body() {}
	inline void set_driver(driver* ndrv) { drv = ndrv; }
	void operator()() {
		if(drv) {
			while(spontaneous_condition()) {
				spontaneous();
			}
			body();
		}
	}
};

#endif
