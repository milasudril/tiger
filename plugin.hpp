//@	{
//@	"targets":[{"name":"plugin.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"plugin.o","rel":"implementation"}]
//@	}

#ifndef TIGER_PLUGIN_H
#define TIGER_PLUGIN_H

#include <string>
#include <cstdint>

namespace Tiger
	{
	class Plugin
		{
		public:
			Plugin(const Plugin&)=delete;
			Plugin& operator=(const Plugin&)=delete;
			Plugin(Plugin&& obj):m_name(std::move(obj.m_name))
				,m_handle(obj.m_handle)
				{obj.m_handle=NULL;}

			Plugin& operator=(Plugin&& obj)
				{
				std::swap(obj.m_name,m_name);
				std::swap(obj.m_handle,m_handle);
				return *this;
				}

			explicit Plugin(std::string&& filename);

			~Plugin();

			template<class Entry>
			Entry entryPoint(const char* name) const
				{
				Entry ret;
				entryPoint(name,reinterpret_cast<intptr_t*>(&ret));
				return ret;
				}

		private:
			std::string m_name;
			void* m_handle;
			void entryPoint(const char* name,intptr_t* result) const;
		};
	}
#endif