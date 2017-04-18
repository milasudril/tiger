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

#include "range.hpp"
#include <cstdint>
#include <memory>

namespace Tiger
	{
	class DataSource;
	class DataSink;
	class Image
		{
		public:
			typedef float SampleType;

			Image():m_width(0),m_height(0),m_channel_count(0){}

			explicit Image(DataSource&& source):Image(source){}

			explicit Image(DataSource& source);

			explicit Image(uint32_t width,uint32_t height,uint32_t n_channels);

			void store(DataSink&& sink) const
				{store(sink);}

			void store(DataSink& sink) const;

			bool valid() const noexcept
				{return static_cast<bool>(m_data);}

			SampleType* pixels() noexcept
				{
				rangeInvalidate();
				return m_data.get();
				}

			const SampleType* pixels() const noexcept
				{return m_data.get();}

			uint32_t width() const noexcept
				{return m_width;}

			uint32_t height() const noexcept
				{return m_height;}

			uint32_t channelCount() const noexcept
				{return m_channel_count;}

			const Range& range(uint32_t ch) const noexcept
				{
				assert(ch<m_channel_count);
				if(!m_range[ch].valid())
					{rangeCompute();}
				return m_range[ch];
				}

		private:
			std::unique_ptr<SampleType[]> m_data;
			uint32_t m_width;
			uint32_t m_height;
			uint32_t m_channel_count;
			mutable std::unique_ptr<Range[]> m_range;
			void rangeCompute() const noexcept;
			void rangeInvalidate() noexcept;
		};

	inline Image layoutClone(const Image& img)
		{return Image(img.width(),img.height(),img.channelCount());}

	inline bool layoutSame(const Image& img_a,const Image& img_b)
		{
		return img_a.width()==img_b.width() && img_a.height()==img_b.height()
			&& img_a.channelCount()==img_b.channelCount();
		}

	void fitToLargest(Image* img_begin,Image* img_end);

	Image resize(const Image& src,uint32_t width,uint32_t height);
	}

#endif
