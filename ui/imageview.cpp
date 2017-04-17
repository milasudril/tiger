//@	{"targets":[{"name":"imageview.o","type":"object"}]}

#include "imageview.hpp"
#include "range.hpp"
#include <algorithm>
#include <cmath>

using namespace Tiger;

static double sinh2(double x)
	{
	return 0.5*(std::exp2(x) - std::exp2(-x));
	}

static double asinh2(double x)
	{
	return log2(x + sqrt(x*x + 1.0));
	}

static double line(double x)
	{
	return 23.0*x - 23.0*(1.0 - x);
	}

static double line_inv(double x)
	{
	return (x + 23.0)/46.0;
	}

static Tiger::Range map(Tiger::Range r)
	{
	r=Range(line(r.min()),line(r.max()));
	return Range(sinh2(r.min()),sinh2(r.max()));
	}

static double unmap(double x)
	{
	x=asinh2(x);
	return line_inv(x);
	}

ImageView::ImageView(Container& cnt,int id):m_id(id)
,m_box(cnt,1)
	,m_entry_max(m_box,0)
	,m_rv_box(m_box.insertMode(Tiger::Box::InsertMode{0,Tiger::Box::FILL|Tiger::Box::EXPAND}),0)
		,m_sep_left(m_rv_box.insertMode(Tiger::Box::InsertMode{0,Tiger::Box::FILL|Tiger::Box::EXPAND}),1)
		,m_rv(m_rv_box,0)
		,m_sep_right(m_rv_box,1)
	,m_entry_min(m_box.insertMode(Tiger::Box::InsertMode{0,0}),1)
	{
	(*this)(m_rv);
	m_entry_max.alignment(1.0f).small(1).width(8).callback(*this);
	m_entry_min.alignment(1.0f).small(1).width(8).callback(*this);
	m_rv.callback(*this);
	}

void ImageView::operator()(RangeView& rv)
	{
	auto r=map(rv.range());
	char buffer[12];
	sprintf(buffer,"%.2e",r.min());
	m_entry_min.content(buffer);
	sprintf(buffer,"%.2e",r.max());
	m_entry_max.content(buffer);
	}

void ImageView::operator()(TextEntry& entry)
	{
	switch(entry.id())
		{
		case 0:
			{
			auto v_min=m_rv.range().min();
			auto v=std::max(unmap(atof(entry.content())),v_min);
			m_rv.range(Tiger::Range(v_min,v));
			break;
			}
		case 1:
			{
			auto v_max=m_rv.range().max();
			auto v=std::min(unmap(atof(entry.content())),v_max);
			m_rv.range(Tiger::Range(v,v_max));
			break;
			}
		default:
			break;
		}
	}
