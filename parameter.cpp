//@	 {"targets":[{"name":"parameter.o","type":"object"}]}

#include "parameter.hpp"

using namespace Tiger;

void Parameter::load(const char* keyval_str)
	{
	Parameter ret;
	enum class State:int{KEY,VALUE};
	auto state=State::KEY;
	auto ptr=keyval_str;
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
						ret.m_value=stof(buffer);
						*this=ret;
						return;
					default:
						buffer+=ch_in;
					}
				break;
			}
		++ptr;
		}
	}