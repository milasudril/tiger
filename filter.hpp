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
#include "host.hpp"
#include <map>
#include <vector>

namespace Tiger
	{
	class Filter:public Plugin
		{
		public:
			explicit Filter(const char* src,const char* objdir);
			void paramSet(const Parameter& param);
			void paramsList(FILE* output) const;
			void channelsList(FILE* output) const;
			unsigned int channelIndex(const std::string& name) const;
			unsigned int channelCount() const noexcept;
			const float* parameters() const noexcept
				{return m_params.data();}
			void run(const ProcessData& procdata) const
				{m_process(procdata);}

		private:
			decltype(&process) m_process;
			std::vector<float> m_params;
			std::map<std::string,unsigned int> m_param_index;
			std::map<std::string,unsigned int> m_channel_index;
		};
	}

#endif // TIGER_FILTER_HPP
