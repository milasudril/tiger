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
