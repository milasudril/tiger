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
//@	{"targets":[{"name":"separator.o","type":"object","pkgconfig_libs":["gtk+-3.0"]}]}

#include "separator.hpp"
#include "container.hpp"
#include <gtk/gtk.h>

using namespace Tiger;

class Separator::Impl:private Separator
	{
	public:
		Impl(Container& cnt,bool vertical);
		~Impl();

	private:
		GtkSeparator* m_handle;
	};

Separator::Separator(Container& cnt,bool vertical) noexcept
	{m_impl=new Impl(cnt,vertical);}

Separator::~Separator()
	{delete m_impl;}

Separator::Impl::Impl(Container& cnt,bool vertical):Separator(*this)
	{
	printf("Separator %p ctor\n",this);

	auto widget=gtk_separator_new(vertical?GTK_ORIENTATION_VERTICAL
		:GTK_ORIENTATION_HORIZONTAL);
	
	m_handle=GTK_SEPARATOR(widget);
	g_object_ref_sink(m_handle);
	cnt.add(widget);
	}

Separator::Impl::~Impl()
	{
	m_impl=nullptr;
	gtk_widget_destroy(GTK_WIDGET(m_handle));
	printf("Separator %p dtor\n",this);
	}
