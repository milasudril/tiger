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
//@  "targets":[{"name":"imagedisplay.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"imagedisplay.o","rel":"implementation"}]
//@	}

#ifndef TIGER_IMAGEDISPLAY_HPP
#define TIGER_IMAGEDISPLAY_HPP

#include <utility>

namespace Tiger
	{
	class Container;
	class Image;
	class Range;
	class ImageDisplay
		{
		public:
			explicit ImageDisplay(Container& container,int id);
			~ImageDisplay();

			ImageDisplay& operator=(ImageDisplay&& obj) noexcept
				{
				std::swap(obj.m_impl,m_impl);
				return *this;
				}

			ImageDisplay(ImageDisplay&& obj) noexcept:m_impl(obj.m_impl)
				{obj.m_impl=nullptr;}
			
			template<class ImageDisplayCallback>
			ImageDisplay& callback(ImageDisplayCallback& cb)
				{
				auto cb_wrapper=[](void* rvc,ImageDisplay& self)
					{
					auto x=reinterpret_cast<ImageDisplayCallback*>(rvc);
					(*x)(self);
					};
				return callback(cb_wrapper,&cb); 
				}

			ImageDisplay& image(Image&& img)=delete;

			ImageDisplay& image(const Image& img,int channel) noexcept;

			Range zrangeOptimal() const noexcept;

			ImageDisplay& zrange(const Range& r) noexcept;
			
			int id() const noexcept;

		protected:
			typedef void (*Callback)(void* cb_obj,ImageDisplay& self);
			ImageDisplay& callback(Callback cb,void* cb_obj);

			class Impl;
			Impl* m_impl;

			explicit ImageDisplay(Impl& impl):m_impl(&impl){}
		};
	}

#endif // TIGER_IMAGEDISPLAY_HPP
