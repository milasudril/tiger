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
//@	{"targets":[{"name":"datasink.hpp","type":"include"}]}

/**\file datasink.hpp \brief Definition of an abstract data sink
 */
#ifndef TIGER_DATASINK_HPP
#define TIGER_DATASINK_HPP

#include <cstddef>

namespace Tiger
	{
	/**\brief An abstract data sink.
	 *
	 * A DataSink is an object that can be write and that has a name.
	 */
	class DataSink
		{
		public:
			/**\brief Writes data to this source.
			 *
			 * This method shall write at most `length` bytes to the data
			 * sink from `buffer`. The method shall return the number of bytes
			 * actually written. If this value is less than `length`, the caller
			 * may assume that an error occured.
			 *
			*/
			virtual size_t write(const void* buffer, size_t length)=0;

			/**\brief Retrievs the name of the source.
			 *
			 * This method shall return a name identifying the data source.
			 *
			*/
			virtual const char* name() const noexcept=0;
			
			virtual void flush()=0;

			virtual ~DataSink()=default;

			/**\brief Function pointer type for a DataSink destruction
			 * function
			 *
			 * A Deleter shall destroy the object pointed to by `data_source`.
			 *
			 * \warning The function must not throw any exceptions.
			*/
			typedef void (*Deleter)(DataSink* data_source);
		};
	}

#endif
