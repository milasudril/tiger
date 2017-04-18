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
//@	 "targets":[{"name":"mimeidentifier.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"mimeidentifier.o","rel":"implementation"}]
//@	}

#ifndef TIGER_MIMEIDENTIFIER_HPP
#define TIGER_MIMEIDENTIFIER_HPP

#include <cstring>

namespace Tiger
	{
	class MimeIdentifier
		{
		public:
			MimeIdentifier(const MimeIdentifier&)=delete;
			MimeIdentifier& operator=(const MimeIdentifier&)=delete;

			MimeIdentifier();
			~MimeIdentifier();

			const char* identify(const char* filename) const noexcept;

		private:
			void* m_handle;
		};

	inline bool begins_with(const char* str,const char* str2)
		{return strncmp(str,str2,strlen(str2))==0;}
	}

#endif
