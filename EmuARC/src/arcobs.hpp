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

#ifndef ARCOBS
#define ARCOBS

#include <vector>

template <class T>
class observable;


template <class T>
class observer {
public:
	virtual ~observer() {};
	virtual void update(observable<T>* obs) = 0;
	virtual void update(observable<T>* obs, T* data) = 0;
};


template <class T>
class observable {
private:
	std::vector< observer<T>* > obss;
protected:
	void notify();
	void notify(T* data);
public:
	virtual ~observable() {}
	void attach(observer<T>* obs);
	void detach(observer<T>* obs);
};


template <class T>
void observable<T>::attach(observer<T>* obs) {
	typename std::vector< observer<T>* >::iterator it =
		find(obss.begin(), obss.end(), obs);
	if(it == obss.end())
		obss.push_back(obs);
}

template <class T>
void observable<T>::detach(observer<T>* obs) {
	typename std::vector< observer<T>* >::iterator it =
		find(obss.begin(), obss.end(), obs);
	if(it != obss.end())
		obss.erase(it);
}

template <class T>
void observable<T>::notify() {
	typename std::vector< observer<T>* >::iterator it;
	for (it = obss.begin(); it != obss.end(); ++it)
		(*it)->update(static_cast< observable<T>* >(this));
}

template <class T>
void observable<T>::notify(T* data) {
	typename std::vector< observer<T>* >::iterator it;
	for (it = obss.begin(); it != obss.end(); ++it)
		(*it)->update(static_cast< observable<T>* >(this), data);
}

#endif
