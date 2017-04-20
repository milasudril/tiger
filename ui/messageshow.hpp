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
//@	 "targets":[{"name":"messageshow.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"messageshow.o","rel":"implementation"}]
//@	}

#ifndef TIGER_MESSAGESHOW_HPP
#define TIGER_MESSAGESHOW_HPP
#include <cstdint>

namespace Tiger
	{
	class Container;

	static constexpr uint32_t MESSAGE_TYPE_NORMAL=0x40;
	static constexpr uint32_t MESSAGE_TYPE_WARNING=0x30;
	static constexpr uint32_t MESSAGE_TYPE_ERROR=0x10;

	static constexpr uint32_t MESSAGE_BUTTONS_OK=0x0;
	static constexpr uint32_t MESSAGE_BUTTONS_RETRYCANCEL=0x5;
	static constexpr uint32_t MESSAGE_BUTTONS_YESNO=0x4;
	static constexpr uint32_t MESSAGE_BUTTONS_YESNOCANCEL=0x3;

	enum class MessageAnswer:uint32_t
		{
		 CANCEL=2
		,OK=1
		,NO=7
		,RETRY=4
		,YES=6
		};

	MessageAnswer messageShow(const Container& owner,const char* message
		,const char* context_title,uint32_t message_type);
	}

#endif