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
			Entry __attribute__((returns_nonnull)) entryPoint(const char* name) const
				{
				Entry ret;
				entryPoint(name,reinterpret_cast<intptr_t*>(&ret));
				return ret;
				}

			const char* name() const noexcept
				{return m_name.c_str();}

		private:
			std::string m_name;
			void* m_handle;
			void entryPoint(const char* name,intptr_t* result) const;
		};
	}
#endif