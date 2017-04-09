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
//@	{"targets":[{"name":"datasource.hpp","type":"include"}]}

/**\file datasource.hpp \brief Definition of an abstract data source
 */

#ifndef TIGER_DATASOURCE_HPP
#define TIGER_DATASOURCE_HPP

#include <cstddef>

namespace Tiger
	{
	/**\brief An abstract data source.
	 *
	 * A DataSource is an object that can be read and that has a name.
	 */
	class DataSource
		{
		public:
			/**\brief Reads data from ths source.
			 *
			 * This method shall read at most `length` bytes from the data
			 * source into `buffer`. The method shall return the number of bytes
			 * actually read. If this value is less than `length`, the caller
			 * may assume an end of file was reached.
			 *
			*/
			virtual size_t read(void* buffer, size_t length)=0;

			/**\brief Retrievs the name of the source.
			 *
			 * This method shall return a name identifying the data source.
			 *
			*/
			virtual const char* name() const noexcept=0;

			virtual ~DataSource()=default;

			/**\brief Function pointer type for a DataSource destruction
			 * function
			 *
			 * A Deleter shall destroy the object pointed to by `data_source`.
			 *
			 * \warning The function must not throw any exceptions.
			*/
			typedef void (*Deleter)(DataSource* data_source);
		};
	}

#endif
