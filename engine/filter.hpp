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
//@	 "targets":[{"name":"filter.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"filter.o","rel":"implementation"}]
//@	}

#ifndef TIGER_FILTER_HPP
#define TIGER_FILTER_HPP

#include "plugin.hpp"
#include "parameter.hpp"
#include "filterstate.hpp"
#include "pluginmain.hpp"
#include <map>
#include <vector>

namespace Tiger
	{
	class Filter:public Plugin
		{
		public:
			explicit Filter(const char* src,const char* objdir);

			unsigned int channelIndex(const std::string& name) const;
			unsigned int channelCount() const noexcept;
			unsigned int parameterIndex(const std::string& name) const;
			unsigned int parameterCount() const noexcept;

			const char* channelName(unsigned int index) const noexcept
				{return m_channel_names[index];}

			const char* const& parameterName(unsigned int index) const noexcept	
				{return m_param_names[index];}

			auto processEntry() const noexcept
				{return m_process;}

		private:
			decltype(&__process) m_process;
			const char* const* m_channel_names;
			const char* const* m_param_names;
			std::map<std::string,unsigned int> m_param_index;
			std::map<std::string,unsigned int> m_channel_index;
			
		};
	}

#endif // TIGER_FILTER_HPP
