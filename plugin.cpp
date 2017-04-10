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
//@		"dependencies":[{"ref":"dl","rel":"external"}]
//@		,"name":"plugin.o","type":"object"
//@		}]
//@	}
#include "plugin.hpp"
#include "error.hpp"
#include <dlfcn.h>

using namespace Tiger;

Plugin::Plugin(std::string&& filename):m_name(std::move(filename))
	{
	m_handle=dlopen(m_name.c_str(),RTLD_LOCAL|RTLD_NOW|RTLD_DEEPBIND);
	if(m_handle==NULL)
		{
		throw Error("It was not possible to load the plugin "
			,m_name.c_str(),". ",dlerror());
		}
	}

Plugin::~Plugin()
	{
	if(m_handle!=NULL)
		{dlclose(m_handle);}
	}

void Plugin::entryPoint(const char* name,intptr_t* p_loc) const
	{
	auto sym=dlsym(m_handle,name);
	if(sym==NULL)
		{
		throw Error("It was not possible to find the entry point "
			,name," in ",m_name.c_str(),". ",dlerror());
		}
	*p_loc=reinterpret_cast<intptr_t>(sym);
	}
