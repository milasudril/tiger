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
//@  "targets":[{"name":"tabview.o","type":"object","pkgconfig_libs":["gtk+-3.0"]}]
//@	}

#include "tabview.hpp"
#include <gtk/gtk.h>
#include <string>

using namespace Tiger;

class TabView::Impl:private TabView
	{
	public:
		Impl(Container& cnt);
		~Impl();

		void _add(GtkWidget* handle) noexcept
			{
			auto label=gtk_label_new(m_tab_title.c_str());
			gtk_notebook_append_page(m_handle,handle,label);
			}

		void _show() noexcept 
			{gtk_widget_show_all(GTK_WIDGET(m_handle));}

		void _sensitive(bool val)
			{gtk_widget_set_sensitive(GTK_WIDGET(m_handle),val);}

		void* _toplevel() const
			{return gtk_widget_get_toplevel(GTK_WIDGET(m_handle));}

		void tabTitle(const char* t)
			{m_tab_title=std::string(t);}

	private:
		static void destroy_callback (GtkWidget* object,gpointer user_data);
		GtkNotebook* m_handle;
		std::string m_tab_title;
	};

TabView::TabView(Container& cnt)
	{
	printf("TabView %p ctor\n",this);
	m_impl=new TabView::Impl(cnt);
	}

TabView::~TabView()
	{
	delete m_impl;
	printf("TabView %p dtor\n",this);
	}

TabView& TabView::add(void* handle)
	{
	m_impl->_add(GTK_WIDGET(handle));
	return *this;
	}

TabView& TabView::show()
	{
	m_impl->_show();
	return *this;
	}

TabView& TabView::sensitive(bool val)
	{
	m_impl->_sensitive(val);
	return *this;
	}

void* TabView::toplevel() const
	{return m_impl->_toplevel();}

TabView& TabView::tabTitle(const char* t)
	{
	m_impl->tabTitle(t);
	return *this;
	}


TabView::Impl::Impl(Container& cnt):TabView(*this)
	{
	printf("TabView::Impl %p ctor\n",this);
	auto widget=gtk_notebook_new();
	cnt.add(widget);
	g_object_ref_sink(widget);
	m_handle=GTK_NOTEBOOK(widget);
	}

TabView::Impl::~Impl()
	{
	m_impl=nullptr;
	gtk_widget_destroy(GTK_WIDGET(m_handle));
	printf("TabView::Impl %p dtor\n",this);
	}
