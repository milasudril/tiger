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
#include <algorithm>

using namespace Tiger;

Simulation::Simulation(const char* filter,const char* objdir):
	m_filter(filter,objdir),m_frame_current(0)
	{
	m_params.resize(m_filter.parameterCount());
	m_current=Image(128,128,m_filter.channelCount());
	m_next=layoutClone(m_current);
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

Simulation& Simulation::imagesLoad(const std::vector<Image>& images_init)
	{
	auto current=imagesLoad(images_init,m_filter);
	auto next=layoutClone(current);
	m_current=std::move(current);
	m_next=std::move(next);
	m_state=FilterState(m_next.pixels(),m_current.pixels(),m_params.data()
		,m_next.width(),m_next.height());

	return *this;
	}

static void imageSoA2AoS(const Tiger::Image& src,unsigned int ch,Tiger::Image& dest)
	{
	assert(src.height()==dest.height() && src.width()==dest.width() &&
		ch<dest.channelCount() );

	auto w=dest.width();
	auto h=dest.height();
	auto N=w*h;
	auto ch_count=dest.channelCount();
	auto ch_count_in=src.channelCount();
	auto pixels_in=src.pixels();
	auto pixels_out=dest.pixels() + ch;
	while(N)
		{
		*pixels_out=*pixels_in;
		pixels_out+=ch_count;
		pixels_in+=ch_count_in;
		--N;
		}
	}

namespace
	{
	template<class ... Iterator>
	void increment_dummy(Iterator& ... i)
		{}

	template<class Function,class ... Iterator>
	void for_each_combined(size_t N,Function&& fun,Iterator... iter)
		{
		while(N!=0)
			{
			fun(*iter...);
			increment_dummy(++iter...);
			--N;
			}
		}
	}

Image Simulation::imagesLoad(const std::vector<Tiger::Channel>& files,const Filter& f)
	{
	std::vector<Image> images(files.size());
	for_each_combined(files.size(),[](const Channel& ch,Image& img)
		{
		img=Image{Tiger::SrcStdio(ch.filename())};
		if(img.channelCount()!=1)
			{throw Tiger::Error(ch.filename(),": Only grayscale images are supported.");}
		},files.begin(),images.begin());

	fitToLargest(images.data(),images.data() + files.size());
	Image ret(images[0].width(),images[0].height(),f.channelCount());	
	for_each_combined(files.size(),[&f,&ret](const Channel& ch,const Image& img)
		{
		auto ch_index=f.channelIndex(ch.name());
		imageSoA2AoS(img,ch_index,ret);
		},files.begin(),images.begin());

	return std::move(ret);
	}

Image Simulation::imagesLoad(const std::vector<Image>& images
	,const Filter& f)
	{	
	std::vector<Image> img_copy(images.size());
	std::transform(images.begin(),images.end(),img_copy.begin()
		,[](const Image& img)
			{return clone(img);});

	fitToLargest(img_copy.data(),img_copy.data() + img_copy.size());
	Image ret(img_copy[0].width(),img_copy[0].height(),static_cast<uint32_t>(img_copy.size()));	
	int k=0;
	std::for_each(img_copy.begin(),img_copy.end(),[&k,&ret](const Image& img)
		{
		imageSoA2AoS(img,k,ret);
		++k;
		});

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
	Image img_out(d.width(),d.height(),1);
	while(ptr!=ptr_end)
		{
		auto ch=f.channelIndex(ptr->name());
		imageAoS2SoA(d,f.channelCount(),ch,img_out);
		img_out.store(SinkStdio{ptr->filename()});
		++ptr;
		}
	}

void Simulation::imagesStore(const Filter& f,const FilterState& d
	,std::vector<Image>& images)
	{
	images.resize(f.channelCount());
	auto ptr=images.begin();
	auto ptr_end=images.end();
	auto k=0;
	while(ptr!=ptr_end)
		{
		*ptr=Image(d.width(),d.height(),1);
		imageAoS2SoA(d,f.channelCount(),k,*ptr);
		++ptr;
		++k;
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