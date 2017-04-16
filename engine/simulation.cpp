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
//@	{"targets":[{"name":"simulation.o","type":"object"}]}

#include "simulation.hpp"
#include "channel.hpp"
#include "srcstdio.hpp"
#include "sinkstdio.hpp"
#include <cassert>

using namespace Tiger;

Simulation::Simulation(const char* filter,const char* objdir):
	m_filter(filter,objdir),m_frame_current(0)
	{
	m_params.resize(m_filter.parameterCount());
	}

Simulation& Simulation::run(ProcessMonitor monitor,void* processcallback) noexcept
	{
	auto k=m_frame_current;
	auto filter_entry=m_filter.processEntry();
	while(monitor(processcallback,*this,k))
		{
		filter_entry(m_state,k);
		m_state.swap();
		++k;
		}
	m_frame_current=k;
	return *this;
	}

Simulation& Simulation::imagesLoad(const std::vector<Channel>& files_init)
	{
	auto current=imagesLoad(files_init,m_filter);
	auto next=layoutClone(current);
	m_current=std::move(current);
	m_next=std::move(next);
	m_state=FilterState(m_next.pixels(),m_current.pixels(),m_params.data()
		,m_next.width(),m_next.height());

	return *this;
	}

static void imageSoA2AoS(const Tiger::Image& src,unsigned int ch,Tiger::Image& dest)
	{
	assert(src.height()==dest.height() && src.width()==dest.width() 
		&& src.channelCount()==1 && ch<dest.channelCount() );

	auto w=dest.width();
	auto h=dest.height();
	auto N=w*h;
	auto ch_count=dest.channelCount();
	auto pixels_in=src.pixels();
	auto pixels_out=dest.pixels() + ch;
	while(N)
		{
		*pixels_out=*pixels_in;
		pixels_out+=ch_count;
		++pixels_in;
		--N;
		}
	}

Image Simulation::imagesLoad(const std::vector<Tiger::Channel>& files,const Tiger::Filter& f)
	{
	auto ptr=files.begin();
	auto ptr_end=files.end();
	Tiger::Image ret;
	while(ptr!=ptr_end)
		{
		Tiger::Image src{Tiger::SrcStdio(ptr->filename())};
		if(src.channelCount()!=1)
			{throw Tiger::Error(ptr->filename(),": Only grayscale images are supported.");}
		if(!ret.valid())
			{ret=Tiger::Image(src.width(),src.height(),f.channelCount());}
		if( ret.width()!=src.width() || ret.height()!=src.height() )
			{
			throw Tiger::Error(ptr->filename()
				,": Image has a different size comared to other loaded images.");
			}

		auto ch=f.channelIndex(ptr->name());
		imageSoA2AoS(src,ch,ret);
		++ptr;
		}
	return std::move(ret);
	}

static void imageAoS2SoA(const FilterState& src,unsigned int ch_count
	,unsigned int ch,Tiger::Image& dest)
	{
	assert(src.height()==static_cast<int>(dest.height())
		&& src.width()==static_cast<int>(dest.width())
		&& dest.channelCount()==1 && ch<ch_count );;

	auto w=dest.width();
	auto h=dest.height();
	auto N=w*h;
	auto pixels_in=src.buffer_current() + ch;
	auto pixels_out=dest.pixels();
	while(N)
		{
		*pixels_out=*pixels_in;
		pixels_in+=ch_count;
		++pixels_out;
		--N;
		}
	}

void Simulation::imagesStore(const Tiger::Filter& f,const Tiger::FilterState& d
	,const std::vector<Tiger::Channel>& files)
	{
	auto ptr=files.begin();
	auto ptr_end=files.end();
	Tiger::Image img_out(d.width(),d.height(),1);
	while(ptr!=ptr_end)
		{
		auto ch=f.channelIndex(ptr->name());
		imageAoS2SoA(d,f.channelCount(),ch,img_out);
		img_out.store(Tiger::SinkStdio{ptr->filename()});
		++ptr;
		}
	}

Simulation& Simulation::paramsLoad(const std::vector<Parameter>& params)
	{
	auto ptr=params.begin();
	auto ptr_end=params.end();
	while(ptr!=ptr_end)
		{
		auto index=m_filter.parameterIndex(ptr->name());
		m_params[index]=ptr->value();
		++ptr;
		}
	return *this;
	}

const Simulation& Simulation::paramsList(ParamsListCallback cb,void* paramproc) const
	{
	auto N=m_filter.parameterCount();
	for(decltype(N) k=0;k<N;++k)
		{cb(paramproc,*this,m_filter.parameterName(k),m_params[k]);}
	return *this;
	}

Simulation& Simulation::paramsList(ParamsListByRefCallback cb,void* paramproc)
	{
	auto N=m_filter.parameterCount();
	for(decltype(N) k=0;k<N;++k)
		{cb(paramproc,*this,m_filter.parameterName(k),m_params[k]);}
	return *this;
	}

const Simulation& Simulation::channelsList(ChannelsListCallback cb,void* chproc) const
	{
	auto N=m_filter.channelCount();
	for(decltype(N) k=0;k<N;++k)
		{cb(chproc,*this,m_filter.channelName(k));}
	return *this;
	}