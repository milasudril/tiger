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
//@	{
//@	"targets":
//@		[{
//@		 "name":"filter.o","type":"object"
//@		}]
//@	}

#include "filter.hpp"
#include "error.hpp"
#include "sinkstdio.hpp"
#include "mimeidentifier.hpp"
#include "filtercompile.hpp"
#include <cstring>
#include <stack>
#include <array>


using namespace Tiger;

static const char* filename(const char* str)
	{
	auto ptr=str;
	auto ptr_end=ptr + strlen(str);
	while(ptr!=ptr_end)
		{
		--ptr_end;
		auto ch_in=*ptr_end;
		if(ch_in=='/')
			{return ptr_end+1;}
		}
	return ptr_end;
	}

static std::string objectGenerate(const char* src,const char* objdir)
	{
	MimeIdentifier m;
	auto mime=m.identify(src);
	if(mime==nullptr)
		{throw Error(src," has unknown MIME type");}
	if(begins_with(mime,"text/x-c") || begins_with(mime,"text/plain"))
		{
		std::string ret(objdir);
		if(ret.size()!=0){ret+='/';}
		ret+=filename(src);
		ret+=".so";
		filterCompile(src,ret.c_str(),SinkStdio(nullptr));
		return ret;
		}
	if(begins_with(mime,"application/x-sharedlib"))
		{return std::string(src);}
	throw Error("Tiger cannot use a filter of type ",mime);
	}

static void lookup_fill(const char* const* names
	,std::map<std::string,unsigned int>& index_map)
	{
	if(names==nullptr)
		{return;}
	auto N=0;
	while((*names)!=nullptr)
		{
		auto ip=index_map.insert({*names,N});
		if(!ip.second)
			{throw Error(*names," is already used within this context");}
		++names;
		++N;
		}
	}

Filter::Filter(const char* src,const char* objdir):Plugin(objectGenerate(src,objdir))
	{
		{
		auto fn=entryPoint<decltype(&::parameters)>("_Z10parametersv");
		lookup_fill(fn(),m_param_index);
		m_param_names=fn();
		}

		{
		auto fn=entryPoint<decltype(&channels)>("_Z8channelsv");
		lookup_fill(fn(),m_channel_index);
		m_channel_names=fn();
		}

	m_process=entryPoint<decltype(m_process)>("_ZN5Tiger9__processERKNS_11FilterStateEy");
	}
unsigned int Filter::channelIndex(const std::string& ch) const
	{
	auto i=m_channel_index.find(ch);
	if(i==m_channel_index.end())
		{throw Error(name()," does use a channel with name ",ch.c_str());}
	return i->second;
	}

unsigned int Filter::channelCount() const noexcept
	{return static_cast<unsigned int>( m_channel_index.size() );}

unsigned int Filter::parameterIndex(const std::string& param) const
	{
	auto i=m_param_index.find(param);
	if(i==m_param_index.end())
		{throw Error(name()," does not take a parameter with name ",param.c_str());}
	return i->second;
	}

unsigned int Tiger::Filter::parameterCount() const noexcept
	{return static_cast<unsigned int>(m_param_index.size());}
