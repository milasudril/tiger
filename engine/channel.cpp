//@	{"targets":[{"name":"channel.o","type":"object"}]}
/*
Tiger
Copyright (C) 2017  Torbjörn Rathsman

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/
#include "channel.hpp"

using namespace Tiger;

void Channel::load(const char* keyval_string)
	{
	Channel ret;
	enum class State:int{KEY,VALUE};
	auto state=State::KEY;
	auto ptr=keyval_string;
	std::string buffer;
	while(1)
		{
		auto ch_in=*ptr;
		switch(state)
			{
			case State::KEY:
				switch(ch_in)
					{
					case ':':
						state=State::VALUE;
						break;
					case '\0':
						*this=ret;
						return;
					default:
						ret.m_name+=ch_in;
					}
				break;
			case State::VALUE:
				switch(ch_in)
					{
					case '\0':
						*this=ret;
						return;
					default:
						ret.m_filename+=ch_in;
					}
				break;
			}
		++ptr;
		}
	}

