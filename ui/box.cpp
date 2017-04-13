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
//@  "targets":[{"name":"box.o","type":"object","pkgconfig_libs":["gtk+-3.0"]}]
//@	}

#include "box.hpp"
#include <gtk/gtk.h>

using namespace Tiger;

class Box::Impl
	{
	public:
		Impl(Container& cnt,bool vertical);
		~Impl();

		void add(GtkWidget* handle) noexcept
			{gtk_box_pack_start(m_handle,handle,m_mode.expand,m_mode.fill,m_mode.padding);}

		void show() noexcept 
			{gtk_widget_show_all(GTK_WIDGET(m_handle));}

		void homogenous(bool status) noexcept
			{gtk_box_set_homogeneous(m_handle,status);}

		void insertMode(const InsertMode& mode) noexcept
			{m_mode=mode;}

	private:
		static void destroy_callback (GtkWidget* object,gpointer user_data);
		GtkBox* m_handle;
		InsertMode m_mode;
	};

Box::Box(Container& cnt,bool vertical)
	{
	m_impl=new Box::Impl(cnt,vertical);
	}

Box::~Box()
	{}

Box& Box::add(void* handle)
	{
	m_impl->add(GTK_WIDGET(handle));
	return *this;
	}

void Box::show()
	{m_impl->show();}

Box& Box::homogenous(bool status) noexcept
	{
	m_impl->homogenous(status);
	return *this;
	}

Box& Box::insertMode(const InsertMode& mode) noexcept
	{
	m_impl->insertMode(mode);
	return *this;
	}

Box::Impl::Impl(Container& cnt,bool vertical):m_mode{0,0,0}
	{
	printf("Box %p ctor\n",this);
	auto widget=gtk_box_new(vertical?GTK_ORIENTATION_VERTICAL:GTK_ORIENTATION_HORIZONTAL,4);
	g_signal_connect(widget,"destroy",G_CALLBACK(destroy_callback),this);
	cnt.add(widget);
	m_handle=GTK_BOX(widget);
	}

Box::Impl::~Impl()
	{
	printf("Box %p dtor\n",this);
	}

void Box::Impl::destroy_callback (GtkWidget* object,gpointer user_data)
	{
	auto self=reinterpret_cast<Impl*>(user_data);
	delete self;
	}
