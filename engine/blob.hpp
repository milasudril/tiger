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
//@	{"targets":[{"name":"blob.hpp","type":"include"}]}

#ifndef TIGER_BLOB_HPP
#define TIGER_BLOB_HPP

#include <cstdint>
extern "C"
{
#if __gnu_linux__

#define TIGER_BLOB(type,identifier,filename) \
asm(".pushsection .data\n" \
	"\t.local " #identifier "_begin\n" \
	"\t.type " #identifier "_begin, @object\n" \
	"\t.align 16\n" \
	#identifier "_begin:\n" \
	"\t.incbin \"" filename "\"\n\n" \
\
	"\t.local " #identifier "_end\n" \
	"\t.type " #identifier "_end, @object\n" \
	"\t.align 1\n" \
	#identifier "_end:\n" \
	"\t.byte 0\n" \
	"\t.popsection\n"); \
\
extern const type identifier##_begin[];\
extern const type identifier##_end[]

#elif _WIN32

#define TIGER_BLOB(type,identifier,filename) \
asm(".data\n" \
	"\t.align 16\n" \
	#identifier "_begin:\n" \
	"\t.incbin \"" filename "\"\n\n" \
\
	"\t.align 1\n" \
	#identifier "_end:\n" \
	"\t.byte 0\n" \
	"\t.text\n"); \
\
extern const type identifier##_begin[];\
extern const type identifier##_end[]

#else
	#error "Cannot include binary files"
#endif
}
#endif
