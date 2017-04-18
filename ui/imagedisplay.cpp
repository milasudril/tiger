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
//@	{"targets":[{"name":"imagedisplay.o","type":"object","pkgconfig_libs":["gtk+-3.0"]}]}

#include "imagedisplay.hpp"
#include "container.hpp"
#include "../engine/image.hpp"
#include <gtk/gtk.h>
#include <algorithm>

using namespace Tiger;

class ImageDisplay::Impl:private ImageDisplay
	{
	public:
		Impl(Container& cnt,int id);
		~Impl();

		void image(Image&& img)=delete;

		void image(const Image* img,int channel) noexcept
			{
			r_img=img;
			m_view_channel=channel;
			gtk_widget_queue_draw(m_widget);
			}

		void callback(Callback cb,void* cb_obj)
			{
			r_cb=cb;
			r_cb_obj=cb_obj;
			}
			
		int id() const noexcept
			{return m_id;}

		void zrange(const Range& r)
			{
			m_z_range=r;
			gtk_widget_queue_draw(m_widget);
			}
		
		Range zrangeOptimal() const noexcept
			{return r_img==nullptr?Range{}:r_img->range(m_view_channel);}

	private:
		int m_id;
		const Image* r_img;
		int m_view_channel;
		Range m_z_range;
		Callback r_cb;
		void* r_cb_obj;
		GdkCursor* m_cursors[4];
		GtkWidget* m_widget;

		static gboolean draw_callback(GtkWidget* widget,cairo_t* cr,gpointer data);
		static gboolean release_callback(GtkWidget* widget,GdkEvent* event,gpointer user_data);
		static gboolean move_callback(GtkWidget* widget,GdkEvent* event,gpointer user_data);
		static gboolean leave_callback(GtkWidget* widget,GdkEvent* event,gpointer user_data);
		static void destroy_callback (GtkWidget* object,gpointer user_data);
	};

ImageDisplay::ImageDisplay(Container& cnt,int id)
	{m_impl=new Impl(cnt,id);}

ImageDisplay::~ImageDisplay()
	{delete m_impl;}

ImageDisplay& ImageDisplay::image(const Image* img,int channel) noexcept
	{
	m_impl->image(img,channel);
	return *this;
	}

ImageDisplay& ImageDisplay::callback(Callback cb,void* cb_obj)
	{
	m_impl->callback(cb,cb_obj);
	return *this;
	}

int ImageDisplay::id() const noexcept
	{return m_impl->id();}

ImageDisplay& ImageDisplay::zrange(const Range& r) noexcept
	{
	m_impl->zrange(r);
	return *this;
	}

Range ImageDisplay::zrangeOptimal() const noexcept
	{
	return m_impl->zrangeOptimal();
	}



ImageDisplay::Impl::Impl(Container& cnt,int id):ImageDisplay(*this),m_id(id)
	,r_img(nullptr)
	,r_cb(nullptr)
	{
	printf("ImageDisplay %p ctor\n",this);
	auto display=gdk_display_get_default();
	m_cursors[0]=gdk_cursor_new_from_name(display,"default");
	m_cursors[1]=gdk_cursor_new_from_name(display,"pointer");

	auto widget=gtk_drawing_area_new();
	gtk_widget_set_size_request(widget,128,128);
	g_signal_connect(widget,"draw",G_CALLBACK(draw_callback),this);
	gtk_widget_add_events(widget
		,GDK_POINTER_MOTION_MASK|GDK_BUTTON_RELEASE_MASK|GDK_BUTTON_PRESS_MASK|GDK_LEAVE_NOTIFY_MASK);
	g_signal_connect(widget,"button-release-event",G_CALLBACK(release_callback),this);
	g_signal_connect(widget,"motion-notify-event",G_CALLBACK(move_callback),this);
	g_signal_connect(widget,"leave-notify-event",G_CALLBACK(leave_callback),this);
	auto style=gtk_widget_get_style_context(widget);
	gtk_style_context_add_class(style,GTK_STYLE_CLASS_ENTRY);
	m_widget=widget;
	g_object_ref_sink(widget);
	cnt.add(widget);
	}

ImageDisplay::Impl::~Impl()
	{
	m_impl=nullptr;
	r_cb=nullptr;
	printf("ImageDisplay %p dtor\n",this);
	auto style=gtk_widget_get_style_context(m_widget);
	gtk_style_context_remove_class(style,GTK_STYLE_CLASS_ENTRY);
	g_object_unref(m_cursors[1]);
	g_object_unref(m_cursors[0]);
	gtk_widget_destroy(m_widget);
	}

static int move_detect(double x,double y,double w,double h)
	{
	if(x<0 || x>w)
		{return 0;}
	if(y<0 || y>h)
		{return 0;}
	return 1;
	}

struct alignas(4) Pixel
	{
	uint8_t v0;
	uint8_t v1;
	uint8_t v2;
	uint8_t v3;
	};

static float toSRGB(float x)
	{
	return x<=0.0031308? 12.92f*x : (1.0f + 0.055f)*std::pow( x,1.0f/2.4f) - 0.055f;
	}

static float valueMap(float x,Range r)
	{
	x=static_cast<float>( (x-r.min()) /(r.max() - r.min()) );
	return std::max(0.0f,std::min(toSRGB(x),1.0f));
	}

gboolean ImageDisplay::Impl::draw_callback(GtkWidget *widget, cairo_t *cr, gpointer data)
	{
	auto state=reinterpret_cast<Impl*>(data);
	
	auto width=gtk_widget_get_allocated_width(widget);
	auto height=gtk_widget_get_allocated_height(widget);

	if(state->r_img!=nullptr)
		{
		auto width_in=state->r_img->width();
		auto nch=state->r_img->channelCount();
		auto ch=state->m_view_channel;

		auto ratio_in=double(width_in)/double(state->r_img->height());
		auto ratio_out=double(width)/double(height);

		auto height_out=size_t(ratio_out > ratio_in? height : width/ratio_in);
		auto width_out=size_t(ratio_out > ratio_in? height*ratio_in : width);

		auto factor=static_cast<double>(width_in)/static_cast<double>(width_out);
		const auto N=width_out*height_out;
		std::unique_ptr<Pixel[]> pixels(new Pixel[N]);
		auto ptr=pixels.get();
		auto ptr_end=ptr + N;
		auto ptr_src=state->r_img->pixels();
		unsigned int row=0;
		unsigned int col=0;
		while(ptr!=ptr_end)
			{
		//	TODO: Interpolate
			auto row_src=size_t(row*factor);
			auto col_src=size_t(col*factor);
			auto v_temp=ptr_src[nch*(row_src*width_in + col_src) + ch];
			auto val_src=static_cast<uint8_t>(255.0*valueMap(v_temp,state->m_z_range));

			*ptr={val_src,val_src,val_src,255};

			++col;
			if(col==width_out)
				{
				col=0;
				++row;
				}
			++ptr;
			}

		auto surface=cairo_image_surface_create_for_data(
			(uint8_t*)pixels.get()
			,CAIRO_FORMAT_ARGB32
			,static_cast<int>(width_out),static_cast<int>(height_out)
			,static_cast<int>(width_out*sizeof(Pixel)));
		cairo_set_operator(cr,CAIRO_OPERATOR_OVER);
		cairo_set_source_surface(cr, surface, 0.5*static_cast<double>(width-width_out), 0.0);
		cairo_paint(cr);

		cairo_surface_destroy(surface);
		}	
	
	return FALSE;
	}

gboolean ImageDisplay::Impl::release_callback(GtkWidget* widget,GdkEvent* event,gpointer user_data)
	{
	auto state=reinterpret_cast<Impl*>(user_data);
	if(state->r_cb!=nullptr)
		{state->r_cb(state->r_cb_obj,*state);}
	return FALSE;
	}

gboolean ImageDisplay::Impl::move_callback(GtkWidget* widget,GdkEvent* event,gpointer user_data)
	{
	auto state=reinterpret_cast<Impl*>(user_data);
	if(state->r_cb!=nullptr)
		{
		auto w=gtk_widget_get_allocated_width(widget);
		auto h=gtk_widget_get_allocated_height(widget);
		auto y=h-event->motion.y;
		auto x=event->motion.x;
		auto parent=gtk_widget_get_parent_window(widget);
		gdk_window_set_cursor(parent,state->m_cursors[move_detect(x,y,w,h)]);
		}
	return FALSE;
	}

gboolean ImageDisplay::Impl::leave_callback(GtkWidget* widget,GdkEvent* event,gpointer user_data)
	{
	auto parent=gtk_widget_get_parent_window(widget);
	auto state=reinterpret_cast<Impl*>(user_data);
	gdk_window_set_cursor(parent,state->m_cursors[0]);
	return FALSE;
	}