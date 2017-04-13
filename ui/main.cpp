//@	{"targets":[{"name":"../tiger-ui","type":"application","pkgconfig_libs":["gtk+-3.0"]}]}

#include <gtk/gtk.h>
#include <algorithm> //std::abs

struct State
	{
	double min;
	double max;
	int move;
	GdkCursor* cursors[4];
	};

static gboolean draw_callback (GtkWidget *widget, cairo_t *cr, gpointer data)
	{
	auto state=reinterpret_cast<State*>(data);
	auto width = gtk_widget_get_allocated_width (widget);
	auto height = gtk_widget_get_allocated_height (widget);

	auto context = gtk_widget_get_style_context (widget);
	gtk_render_background(context,cr,0,0,width,height);

	cairo_rectangle(cr,0,height*(1.0 - state->max),width,height*(state->max - state->min));
	cairo_set_source_rgb(cr, 0.05,0.6,0.15); //What color should be used here?
	cairo_fill (cr);

	cairo_set_source_rgb(cr,0.01,0.3,0.07);	//And here
	auto mid=height*(1.0 - 0.5*(state->min + state->max));
	cairo_move_to(cr,0, mid);
	cairo_line_to(cr,width,mid);
	cairo_stroke(cr);
	
	return FALSE;
	}

static int move_detect(double y,double h,const State& state)
	{
	if(std::abs(y - h*state.max)<4)
		{return 1;}
	if(std::abs(y - h*state.min)<4)
		{return 2;}
	if(std::abs(y-0.5*h*(state.max + state.min))<4)
		{return 3;}
	return 0;
	}

static gboolean press_callback(GtkWidget* widget,GdkEvent* event,gpointer user_data)
	{
	auto state=reinterpret_cast<State*>(user_data);
	auto h=gtk_widget_get_allocated_height(widget);
	auto y=h-event->button.y;
	state->move=move_detect(y,h,*state);
	return FALSE;
	}

static gboolean release_callback(GtkWidget* widget,GdkEvent* event,gpointer user_data)
	{
	auto state=reinterpret_cast<State*>(user_data);
	state->move=0;
	return FALSE;
	}

static gboolean move_callback(GtkWidget* widget,GdkEvent* event,gpointer user_data)
	{
	auto state=reinterpret_cast<State*>(user_data);
	auto h=gtk_widget_get_allocated_height(widget);
	auto state_new=*state;
	auto y=h-event->motion.y;
	switch(state->move)
		{
		case 1:
			state_new.max=std::min(y/h,1.0);
			break;
		case 2:
			state_new.min=std::max(y/h,0.0);
			break;
		case 3:
			{
			auto mid_new=y/h;
			auto l=state_new.max - state_new.min;
			state_new.max=mid_new+0.5*l;
			state_new.min=mid_new-0.5*l;
			if(state_new.max>1.0)
				{
				state_new.max=1.0;
				state_new.min=1.0 - l;
				}
			if(state_new.min<0.0)
				{
				state_new.min=0.0;
				state_new.max=l;
				}
			}
			break;
		default:
			break;
		}
	auto l=state_new.max - state_new.min;
	if(l>0)
		{
		*state=state_new;
		gtk_widget_queue_draw(widget);
		}
	gdk_window_set_cursor(gtk_widget_get_parent_window(widget)
		,state->cursors[state->move==0?move_detect(y,h,*state):state->move]);
	return FALSE;
	}

void cursorsLoad(State& state)
	{
	auto display=gdk_display_get_default();
	state.cursors[0]=gdk_cursor_new_from_name(display,"default");
	state.cursors[1]=gdk_cursor_new_from_name(display,"n-resize");
	state.cursors[2]=gdk_cursor_new_from_name(display,"s-resize");
	state.cursors[3]=gdk_cursor_new_from_name(display,"ns-resize");
	}

int main(int argc, char *argv[])
	{
	gtk_init (&argc, &argv);
	auto window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	g_signal_connect (window, "destroy",G_CALLBACK(gtk_main_quit), NULL);
	auto level=GTK_DRAWING_AREA(gtk_drawing_area_new());
	State state{0.25,0.75,0};
	cursorsLoad(state);
	gtk_widget_set_size_request(GTK_WIDGET(level),32,32);
	g_signal_connect(level,"draw",G_CALLBACK(draw_callback),&state);
	gtk_widget_add_events(GTK_WIDGET(level)
		,GDK_POINTER_MOTION_MASK|GDK_BUTTON_PRESS_MASK|GDK_BUTTON_RELEASE_MASK);
	g_signal_connect(level,"button-press-event",G_CALLBACK(press_callback),&state);
	g_signal_connect(level,"button-release-event",G_CALLBACK(release_callback),&state);
	g_signal_connect(level,"motion-notify-event",G_CALLBACK(move_callback),&state);
	gtk_container_add(GTK_CONTAINER(window),GTK_WIDGET(level));
	gtk_widget_show(GTK_WIDGET(level));
	gtk_widget_show(window);
	gtk_main ();
	return 0;
	}