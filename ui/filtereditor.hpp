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
	class FilterEditorBase
		{
		public:
			explicit FilterEditorBase(Container& cnt);

			void create();
			void open();
			bool saveAs();
			bool save();
			void compile();
			void operator()(ButtonList<FilterEditorBase>& src,Button& btn);
			void operator()(SourceView& srcv);
			bool askSave();
			bool dirty() const noexcept
				{return m_dirty;}

			const char* filename() const noexcept
				{return m_file_current.c_str();}

		private:
			Box m_box;
				ButtonList<FilterEditorBase> m_toolbar;
				Paned m_vsplit;
					SourceView m_src_view;
					SourceView m_output;
			std::string m_file_current;
			bool m_dirty;

			void save(std::string filename);

			virtual void stateChangeNotify()
				{}
		};

	template<class Callback>
	class FilterEditor final:public FilterEditorBase
		{
		public:
			explicit FilterEditor(Container& cnt,int id):FilterEditorBase(cnt),m_id(id)
				{}

			FilterEditor& callback(Callback& cb)
				{
				r_cb=&cb;
				stateChangeNotify();
				return *this;
				}

			int id() const noexcept
				{return m_id;}

			void stateChangeNotify()
				{
				if(r_cb!=nullptr)
					{(*r_cb)(*this);}	
				}

		private:
			int m_id;
			Callback* r_cb;
		};
	}
#endif
