//@	{"targets":[{"name":"channel.o","type":"object"}]}

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

