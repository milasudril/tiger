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
//@	 "dependencies_extra":[{"ref":"image.o","rel":"implementation"}]
//@	,"targets":[{"name":"image.hpp","type":"include"}]
//@	}

#ifndef TIGER_IMAGE_HPP
#define TIGER_IMAGE_HPP

#include <cstdint>
#include <memory>

namespace Tiger
	{
	class DataSource;
	class Image
		{
		public:
			typedef float SampleType;

			explicit Image(DataSource&& source):Image(source){}

			explicit Image(DataSource& source);

			explicit Image(uint32_t width,uint32_t height,uint32_t n_channels
				,uint32_t id);

			SampleType* pixels() noexcept
				{return m_data.get();}

			const SampleType* pixels() const noexcept
				{return m_data.get();}

			uint32_t width() const noexcept
				{return m_width;}

			uint32_t height() const noexcept
				{return m_height;}

			uint32_t channelCount() const noexcept
				{return m_channel_count;}

		private:
			std::unique_ptr<SampleType[]> m_data;
			uint32_t m_width;
			uint32_t m_height;
			uint32_t m_channel_count;
		};
	}

#endif
