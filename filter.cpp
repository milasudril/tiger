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
//@		,"pkgconfig_libs":["uuid"]
//@		,"dependencies":[{"ref":"magic","rel":"external"}]
//@		}]
//@	}

#include "filter.hpp"
#include "error.hpp"
#include "blob.hpp"
#include "sinkstdio.hpp"
#include <magic.h>
#include <uuid.h>
#include <unistd.h>
#include <sys/stat.h>
#include <cstring>
#include <stack>
#include <array>


using namespace Tiger;

inline bool begins_with(const char* str,const char* str2)
	{return strncmp(str,str2,strlen(str2))==0;}

TIGER_BLOB(char,filterstate,"filterstate.hpp");
TIGER_BLOB(char,filterstateclient,"filterstateclient.hpp");
TIGER_BLOB(char,pluginmain,"pluginmain.hpp");

static std::array<char,37> uuid_generate() noexcept
	{
	std::array<char,37> ret;
	uuid_t id;
	::uuid_generate(id);
	uuid_unparse_upper(id,ret.begin());
	return ret;
	}

namespace
	{
	class MagicHandle
		{
		public:
			MagicHandle(const MagicHandle&)=delete;
			MagicHandle& operator=(const MagicHandle&)=delete;

			MagicHandle():m_handle(magic_open(MAGIC_ERROR|MAGIC_MIME))
				{magic_load(m_handle,nullptr);}
			~MagicHandle()
				{magic_close(m_handle);}

			const char* identify(const char* filename) noexcept
				{return magic_file(m_handle,filename);}

		private:
			magic_t m_handle;
		};

	class DirectoryGuard
		{
		public:
			DirectoryGuard& operator=(const DirectoryGuard&)=delete;
			DirectoryGuard(const DirectoryGuard&)=delete;

			DirectoryGuard(const std::string& dir):m_dir(dir)
				{mkdir(m_dir.c_str(),S_IRWXU);}

			~DirectoryGuard()
				{	
				while(!m_items.empty())
					{
					auto x=m_items.top();
					m_items.pop();
					remove(x.c_str());
					}
				rmdir(m_dir.c_str());
				}

			DirectoryGuard& itemAdd(const char* name)
				{
				m_items.push(m_dir + '/' + name);
				return *this;
				}

		private:
			std::stack<std::string> m_items;
			std::string m_dir;
		};
	}

static void compile(const char* src,const char* dest)
	{
	auto tmp=std::string("/dev/shm/");
	tmp+=uuid_generate().data();
	DirectoryGuard g(tmp);
	printRange(filterstate_begin,filterstate_end,SinkStdio((tmp+"/filterstate.hpp").c_str()));
	g.itemAdd("filterstate.hpp");
	printRange(filterstateclient_begin,filterstateclient_end,SinkStdio((tmp+"/filterstateclient.hpp").c_str()));
	g.itemAdd("filterstateclient.hpp");
	printRange(pluginmain_begin,pluginmain_end,SinkStdio((tmp+"/pluginmain.hpp").c_str()));
	
//TODO: Use libmaike (somewhat overkill here) or fork/exec pair
	std::string cmdbuff("g++ -std=c++14 -O3 --fast-math -march=native "
		"-fno-stack-protector -Wconversion -Wall -iquote'");
	cmdbuff+=tmp;
	cmdbuff+="' -include pluginmain.hpp -include filterstateclient.hpp "
		"-fpic -shared -o '";
	cmdbuff+=dest;
	cmdbuff+="' '";
	cmdbuff+=src;
	cmdbuff+="'";

	if(system(cmdbuff.c_str())!=0)
		{throw Error("It was not possible to compile filter ",src);}
	}

static std::string objectGenerate(const char* src,const char* objdir)
	{
	MagicHandle m;
	auto mime=m.identify(src);
	if(mime==nullptr)
		{throw Error(src," has unknown MIME type");}
	if(begins_with(mime,"text/x-c") || begins_with(mime,"text/plain"))
		{
		std::string ret(objdir);
		ret+='/';
		ret+=src;
		ret+=".so";
		compile(src,ret.c_str());
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
	if(i==m_channel_index.end())
		{throw Error(name()," does not take a parameter with name ",param.c_str());}
	return i->second;
	}

unsigned int Tiger::Filter::parameterCount() const noexcept
	{return static_cast<unsigned int>(m_param_index.size());}
