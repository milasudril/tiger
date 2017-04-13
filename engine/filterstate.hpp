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
//@	{"targets":[{"name":"filterstate.hpp","type":"include"}]}

#ifndef TIGER_FILTERSTATE_HPP
#define TIGER_FILTERSTATE_HPP

#include <utility>
#include <cstring>
#include <type_traits>

namespace Tiger
	{
	class FilterState
		{
		public:
			FilterState() noexcept
				{
				static_assert(std::is_standard_layout<FilterState>::value,"");
				memset(this,0,sizeof(*this));
				}

			FilterState(float* buffer_next,float* buffer_current,const float* src
				,const float* params,int w,int h) noexcept:
				 m_next(buffer_next),m_current(buffer_current),m_src(src),m_params(params)
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
			void* user_pointer;
		};
	}
#endif
