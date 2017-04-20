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
//@	{"targets":[{"name":"messageshow.o","type":"object","pkgconfig_libs":["gtk+-3.0"]}]}

#include "messageshow.hpp"
#include "container.hpp"
#include <gtk/gtk.h>

using namespace Tiger;

GtkMessageType messageTypeConvert(unsigned int message_type)
	{
	auto v=message_type&0xf0;
	switch(v)
		{
		case MESSAGE_TYPE_NORMAL:
			return GTK_MESSAGE_INFO;
		case MESSAGE_TYPE_WARNING:
			return GTK_MESSAGE_WARNING;
		case MESSAGE_TYPE_ERROR:
			return GTK_MESSAGE_ERROR;
		default:
			return GTK_MESSAGE_OTHER;
		}
	}

void buttonsAdd(GtkDialog* dialog,unsigned int message_type)
	{
	auto v=message_type&0xf;
	switch(v)
		{
		case MESSAGE_BUTTONS_OK:
			gtk_dialog_add_buttons(dialog,"Ok",MessageAnswer::OK,NULL);
			break;
		case MESSAGE_BUTTONS_RETRYCANCEL:
			gtk_dialog_add_buttons(dialog
				,"Retry",MessageAnswer::RETRY
				,"Cancel",MessageAnswer::CANCEL,NULL);
			break;
		case MESSAGE_BUTTONS_YESNO:
			gtk_dialog_add_buttons(dialog
				,"Yes",MessageAnswer::YES
				,"No",MessageAnswer::NO
				,NULL);
			break;
		case MESSAGE_BUTTONS_YESNOCANCEL:
			gtk_dialog_add_buttons(dialog
				,"Yes",MessageAnswer::YES
				,"No",MessageAnswer::NO
				,"Cancel",MessageAnswer::CANCEL
				,NULL);
			break;
		}
	}


MessageAnswer Tiger::messageShow(const Container& owner,const char* message
	,const char* context_title
	,unsigned int message_type)
	{
	auto parent=GTK_WINDOW( owner.toplevel() );
	auto flags = GTK_DIALOG_DESTROY_WITH_PARENT;

	auto message_type_gtk=messageTypeConvert(message_type);

	auto dialog=gtk_message_dialog_new(parent,flags,message_type_gtk
		,GTK_BUTTONS_NONE,"%s",message);

	buttonsAdd((GtkDialog*)dialog,message_type);
	gtk_window_set_title(GTK_WINDOW(dialog),context_title);

	auto result=gtk_dialog_run (GTK_DIALOG (dialog));
	gtk_widget_destroy (dialog);

	return static_cast<MessageAnswer>(result);
	}
