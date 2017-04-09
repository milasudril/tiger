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
//@	{"targets":[{"name":"cpuinfo.hpp","type":"include"}]}
#ifndef GLINDE_CPUINFO_HPP
#define GLINDE_CPUINFO_HPP

/** \file cpuinfo.hpp \brief Routines for getting properties of the CPU
 */

#include <cstdint>

namespace Tiger
	{
	/**\brief Routines for getting properties of the CPU
	*/
	namespace CPUInfo
		{
		static constexpr uint16_t word_test=0x00ff;

		/**\brief Returns the endianness of the CPU.
		 *
		 * This function tests the endianness of the CPU
		 *
		 * \return A non-zero value when the CPU is big endian, and zero
		 * otherwise.
		 *
		*/
		inline bool constexpr bigEndianIs()
			{
			return ( *(reinterpret_cast<const uint8_t*>(&word_test))==0x0);
			}
		}
	}

#endif
