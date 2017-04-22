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
//@	 "targets":
//@		[{
//@		"name":"mimeidentifier.o","type":"object"
//@		,"dependencies":[{"ref":"magic","rel":"external"}]
//@		}]
//@	}

#include "mimeidentifier.hpp"
#include <magic.h>

using namespace Tiger;

MimeIdentifier::MimeIdentifier():m_handle(magic_open(MAGIC_ERROR|MAGIC_MIME))
	{magic_load(reinterpret_cast<magic_t>(m_handle),nullptr);}

MimeIdentifier::~MimeIdentifier()
	{magic_close(reinterpret_cast<magic_t>(m_handle));}

const char* MimeIdentifier::identify(const char* filename) const noexcept
	{
	return magic_file(reinterpret_cast<magic_t>(m_handle),filename);
	}
