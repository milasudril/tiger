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
//@	{"targets":[{"name":"rangeview.o","type":"object","pkgconfig_libs":["gtk+-3.0"]}]}

#include "rangeview.hpp"
#include "range.hpp"
#include "container.hpp"
#include <gtk/gtk.h>
#include <algorithm>

using namespace Tiger;

class RangeView::Impl:private RangeView
	{
	public:
		Impl(Container& cnt,int id);
		~Impl();

		Range range() const noexcept
			{return m_range;}

		void range(const Range& r) noexcept
			{
			m_range=r;
			gtk_widget_queue_draw(m_widget);
			}

		void callback(Callback cb,void* cb_obj)
			{
			m_cb=cb;
			m_cb_obj=cb_obj;
			}
			
		int id() const noexcept
			{return m_id;}

	private:
		int m_id;
		Range m_range;
		Callback m_cb;
		void* m_cb_obj;
		int m_move;
		GdkCursor* m_cursors[4];
		GtkWidget* m_widget;

		static gboolean draw_callback(GtkWidget* widget,cairo_t* cr,gpointer data);
		static gboolean press_callback(GtkWidget* widget,GdkEvent* event,gpointer user_data);
		static gboolean release_callback(GtkWidget* widget,GdkEvent* event,gpointer user_data);
		static gboolean move_callback(GtkWidget* widget,GdkEvent* event,gpointer user_data);
		static gboolean leave_callback(GtkWidget* widget,GdkEvent* event,gpointer user_data);
		static void destroy_callback (GtkWidget* object,gpointer user_data);
	};

RangeView::RangeView(Container& cnt,int id)
	{m_impl=new Impl(cnt,id);}

RangeView::~RangeView()
	{delete m_impl;}

Range RangeView::range() const noexcept
	{return m_impl->range();}

RangeView& RangeView::range(const Range& r) noexcept
	{
	m_impl->range(r);
	return *this;
	}

RangeView& RangeView::callback(Callback cb,void* cb_obj)
	{
	m_impl->callback(cb,cb_obj);
	return *this;
	}

int RangeView::id() const noexcept
	{return m_impl->id();}



RangeView::Impl::Impl(Container& cnt,int id):RangeView(*this),m_id(id),m_cb(nullptr)
	,m_move(0)
	{
	printf("RangeView %p ctor\n",this);
	auto display=gdk_display_get_default();
	m_cursors[0]=gdk_cursor_new_from_name(display,"default");
	m_cursors[1]=gdk_cursor_new_from_name(display,"n-resize");
	m_cursors[2]=gdk_cursor_new_from_name(display,"s-resize");
	m_cursors[3]=gdk_cursor_new_from_name(display,"ns-resize");

	auto widget=gtk_drawing_area_new();
	gtk_widget_set_size_request(widget,24,64);
	g_signal_connect(widget,"draw",G_CALLBACK(draw_callback),this);
	gtk_widget_add_events(widget
		,GDK_POINTER_MOTION_MASK|GDK_BUTTON_PRESS_MASK|GDK_BUTTON_RELEASE_MASK
		|GDK_LEAVE_NOTIFY_MASK);
	g_signal_connect(widget,"button-press-event",G_CALLBACK(press_callback),this);
	g_signal_connect(widget,"button-release-event",G_CALLBACK(release_callback),this);
	g_signal_connect(widget,"motion-notify-event",G_CALLBACK(move_callback),this);
	g_signal_connect(widget,"leave-notify-event",G_CALLBACK(leave_callback),this);
	auto style=gtk_widget_get_style_context(widget);
	gtk_style_context_add_class(style,GTK_STYLE_CLASS_ENTRY);
	m_widget=widget;
	g_object_ref_sink(widget);
	cnt.add(widget);
	}

RangeView::Impl::~Impl()
	{
	m_impl=nullptr;
	m_cb=nullptr;
	printf("RangeView %p dtor\n",this);
	auto style=gtk_widget_get_style_context(m_widget);
	gtk_style_context_remove_class(style,GTK_STYLE_CLASS_ENTRY);
	g_object_unref(m_cursors[3]);
	g_object_unref(m_cursors[2]);
	g_object_unref(m_cursors[1]);
	g_object_unref(m_cursors[0]);
	gtk_widget_destroy(m_widget);
	}

static int move_detect(double x,double y,double w,double h,const Range& r)
	{
	if(x<0 || x>w)
		{return 0;}
	if(y<0 || y>h)
		{return 0;}

	if(std::abs(y - h*r.max())<4)
		{return 1;}
	if(std::abs(y - h*r.min())<4)
		{return 2;}
	if(std::abs(y - h*r.mid())<4)
		{return 3;}
	return 0;
	}

gboolean RangeView::Impl::draw_callback(GtkWidget *widget, cairo_t *cr, gpointer data)
	{
	auto state=reinterpret_cast<Impl*>(data);
	auto width = gtk_widget_get_allocated_width (widget);
	auto height = gtk_widget_get_allocated_height (widget);

	auto context = gtk_widget_get_style_context (widget);
	gtk_render_background(context,cr,0,0,width,height);

	cairo_rectangle(cr,0,height*(1.0 - state->m_range.max())
			,width,height*state->m_range.length());
	cairo_set_source_rgb(cr, 0.05,0.6,0.15); //What color should be used here?
	cairo_fill (cr);

	cairo_set_source_rgb(cr,0.01,0.3,0.07);	//And here
	auto mid=height*(1.0 - state->m_range.mid());
	cairo_move_to(cr,0, mid);
	cairo_line_to(cr,width,mid);
	cairo_stroke(cr);

	gtk_render_frame(context,cr,0,0,width,height);
	
	return FALSE;
	}

gboolean RangeView::Impl::press_callback(GtkWidget* widget,GdkEvent* event,gpointer user_data)
	{
	auto state=reinterpret_cast<Impl*>(user_data);
	auto h=gtk_widget_get_allocated_height(widget);
	auto w=gtk_widget_get_allocated_width(widget);
	auto y=h-event->button.y;
	auto x=event->button.x;
	state->m_move=move_detect(x,y,w,h,state->m_range);
	return FALSE;
	}

gboolean RangeView::Impl::release_callback(GtkWidget* widget,GdkEvent* event,gpointer user_data)
	{
	auto state=reinterpret_cast<Impl*>(user_data);
	state->m_move=0;
	return FALSE;
	}

gboolean RangeView::Impl::move_callback(GtkWidget* widget,GdkEvent* event,gpointer user_data)
	{
	auto state=reinterpret_cast<Impl*>(user_data);
	auto w=gtk_widget_get_allocated_width(widget);
	auto h=gtk_widget_get_allocated_height(widget);
	auto range_new=state->m_range;
	auto y=h-event->motion.y;
	auto x=event->motion.x;
	switch(state->m_move)
		{
		case 1:
			{
			auto val_new=std::min(y/h,1.0);
			range_new.max(std::max(val_new,range_new.min()));
			}
			break;
		case 2:
			{
			auto val_new=std::max(y/h,0.0);
			range_new.min(std::min(val_new,range_new.max()));
			}
			break;
		case 3:
			{
			auto mid_new=y/h;
			auto l=range_new.length();
			range_new=Range(mid_new-0.5*l,mid_new+0.5*l);
			if(range_new.max()>1.0)
				{range_new=Range(1.0 - l,1.0);}
			if(range_new.min()<0.0)
				{range_new=Range(0.0,l);}
			}
			break;
		default:
			break;
		}
	auto l=range_new.length();
	assert(l>=0);
	state->m_range=range_new;
	gtk_widget_queue_draw(widget);
	auto parent=gtk_widget_get_parent_window(widget);
	if(state->m_move==0)
		{
		gdk_window_set_cursor(parent
			,state->m_cursors[move_detect(x,y,w,h,state->m_range)]);
		}
	else
		{
		gdk_window_set_cursor(parent,state->m_cursors[state->m_move]);
		if(state->m_cb!=nullptr)
			{state->m_cb(state->m_cb_obj,*state);}
		}
	return FALSE;
	}

gboolean RangeView::Impl::leave_callback(GtkWidget* widget,GdkEvent* event,gpointer user_data)
	{
	auto parent=gtk_widget_get_parent_window(widget);
	auto state=reinterpret_cast<Impl*>(user_data);
	gdk_window_set_cursor(parent,state->m_cursors[0]);
	return FALSE;
	}