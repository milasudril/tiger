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
//@	{"targets":[{"name":"error.hpp","type":"include"}]}

#ifndef TIGER_ERROR_HPP
#define TIGER_ERROR_HPP

namespace Tiger
	{
	class Error
		{
		public:
			template<class T,class...U>
			explicit constexpr Error(const T& first,const U&...next) noexcept:m_buffer{}
				{write(m_buffer,m_buffer+capacity(),first,next...);}

			const char* message() const noexcept
				{return m_buffer;}

			static constexpr size_t capacity() noexcept
				{return s_capacity;}

		private:
			static constexpr void write(char* buffer,const char* buffer_end)
				{*buffer='\0';}

			template<class T,class...U>
			static constexpr void write(char* buffer,const char* buffer_end,const T& first,const U&...next)
				{
				if(buffer!=buffer_end)
					{
					buffer=concat(buffer,buffer_end,first);
					write(buffer,buffer_end,next...);
					}
				else
					{*buffer='\0';}
				}

			static constexpr char* concat(char* buffer,const char* buffer_end,const char* value)
				{
				do
					{
					auto val=*value;
					if(val=='\0')
						{return buffer;}
					*buffer=val;
					++value;
					++buffer;
					}
				while(buffer!=buffer_end);
				return buffer;
				}

			static constexpr char* concat(char* buffer,const char* buffer_end,char value)
				{
				*buffer=value;
				return buffer+1;
				}

			static constexpr size_t s_capacity=1023;
			char m_buffer[s_capacity + 1];
		};
	}

#endif // TIGER_ERROR_HPP
