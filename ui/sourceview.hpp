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
//@	 "targets":[{"name":"sourceview.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"sourceview.o","rel":"implementation"}]
//@	}

#ifndef TIGER_SOURCEVIEW_HPP
#define TIGER_SOURCEVIEW_HPP

#include <utility>

namespace Tiger
	{
	class Container;
	class SourceView
		{
		public:
			explicit SourceView(Container& cnt);
			~SourceView();

			SourceView& operator=(SourceView&& obj) noexcept
				{
				std::swap(obj.m_impl,m_impl);
				return *this;
				}

			SourceView(SourceView&& obj) noexcept:m_impl(obj.m_impl)
				{obj.m_impl=nullptr;}


			SourceView& lineNumbers(bool status);

			SourceView& highlight(const char* filename_pattern);

			const char* content() const;

			SourceView& content(const char* text);

		protected:
			class Impl;
			Impl* m_impl;
			explicit SourceView(Impl& impl):m_impl(&impl){}
		};
	}

#endif
