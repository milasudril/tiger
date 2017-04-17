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
//@  "targets":[{"name":"scrolledwindow.o","type":"object","pkgconfig_libs":["gtk+-3.0"]}]
//@	}

#include "scrolledwindow.hpp"
#include <gtk/gtk.h>

using namespace Tiger;

class ScrolledWindow::Impl:private ScrolledWindow
	{
	public:
		Impl(Container& cnt);
		~Impl();

		void add(GtkWidget* handle) noexcept
			{gtk_container_add(GTK_CONTAINER(m_handle),handle);}

		void show() noexcept 
			{gtk_widget_show_all(GTK_WIDGET(m_handle));}


	private:
		static void destroy_callback (GtkWidget* object,gpointer user_data);
		GtkScrolledWindow* m_handle;
	};

ScrolledWindow::ScrolledWindow(Container& cnt)
	{m_impl=new ScrolledWindow::Impl(cnt);}

ScrolledWindow::~ScrolledWindow()
	{delete m_impl;}

ScrolledWindow& ScrolledWindow::add(void* handle)
	{
	m_impl->add(GTK_WIDGET(handle));
	return *this;
	}

void ScrolledWindow::show()
	{m_impl->show();}

ScrolledWindow::Impl::Impl(Container& cnt):ScrolledWindow(*this)
	{
	printf("ScrolledWindow %p ctor\n",this);
	auto widget=gtk_scrolled_window_new(NULL,NULL);
	cnt.add(widget);
	g_object_ref_sink(widget);
	m_handle=GTK_SCROLLED_WINDOW(widget);
	}

ScrolledWindow::Impl::~Impl()
	{
	m_impl=nullptr;
	gtk_widget_destroy(GTK_WIDGET(m_handle));
	printf("ScrolledWindow %p dtor\n",this);
	}
