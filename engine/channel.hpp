//@	{
//@	 "targets":[{"name":"channel.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"channel.o","rel":"implementation"}]
//@	}
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
#ifndef TIGER_CHANNEL_HPP
#define TIGER_CHANNEL_HPP

#include <string>

namespace Tiger
	{
	class Channel
		{
		public:
			Channel(){}

			explicit Channel(const char* keyval_string)
				{load(keyval_string);}

			void load(const char* keyval_string);

			const char* name() const noexcept
				{return m_name.c_str();}

			const char* filename() const noexcept
				{return m_filename.c_str();}


		private:
			std::string m_name;
			std::string m_filename;
		};

	}

#endif // TIGER_CHANNEL_HPP
