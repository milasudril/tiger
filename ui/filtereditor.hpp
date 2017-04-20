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
//@	 "targets":[{"name":"filtereditor.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"filtereditor.o","rel":"implementation"}]
//@	}

#ifndef TIGER_FILTEREDITOR_HPP
#define TIGER_FILTEREDITOR_HPP

#include "box.hpp"
#include "buttonlist.hpp"
#include "paned.hpp"
#include "sourceview.hpp"

#include <string>

namespace Tiger
	{
	class FilterEditor
		{
		public:
			FilterEditor(Container& cnt);

			void create();
			void open();
			bool saveAs();
			bool save();
			void operator()(ButtonList<FilterEditor>& src,Button& btn);
			void operator()(SourceView& srcv);
			bool askSave();
			bool dirty() const noexcept
				{return m_dirty;}

		private:
			Box m_box;
				ButtonList<FilterEditor> m_toolbar;
				Paned m_vsplit;
					SourceView m_src_view;
					SourceView m_output;
			std::string m_file_current;
			bool m_dirty;

			void save(std::string filename);
		};
	}
#endif
