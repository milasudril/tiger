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
//@	 "targets":[{"name":"parameter.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"parameter.o","rel":"implementation"}]
//@	}

#ifndef TIGER_PARAMETER_HPP
#define TIGER_PARAMETER_HPP

#include <string>

namespace Tiger
	{
	class Parameter
		{
		public:
			Parameter():m_name(""),m_value(0){}

			explicit Parameter(const char* keyval_string)
				{load(keyval_string);}

			void load(const char* keyval_string);

			const char* name() const noexcept
				{return m_name.c_str();}

			float value() const noexcept	
				{return m_value;}

		private:
			std::string m_name;
			float m_value;
		};
	}
	
#endif // TIGER_PARAMETER_HPP
