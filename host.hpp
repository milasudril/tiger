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
//@	{"targets":[{"name":"host.hpp","type":"include"}]}

#ifndef TIGER_HOST_HPP
#define TIGER_HOST_HPP

#include <utility>

namespace Tiger
	{
	class ProcessData
		{
		public:
			ProcessData(float* buffer_a,float* buffer_b,const float* src
				,const float* params,int w,int h) noexcept:
				 m_next(buffer_a),m_current(buffer_b),m_src(src),m_params(params)
				,m_width(w),m_height(h)
				{}

			const float* buffer_current() const noexcept
				{return m_current;}

			int width() const noexcept
				{return m_width;}

			int height() const noexcept
				{return m_height;}

			void swap() noexcept
				{std::swap(m_next,m_current);}
				
		protected:
			float* m_next;
			float* m_current;
			const float* m_src;
			const float* m_params;
			int m_width;
			int m_height;
		};
	}

const char* const* parameters() noexcept;
const char* const* channels() noexcept;

void process(const Tiger::ProcessData& data);

#endif
