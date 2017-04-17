//@	{"targets":[{"name":"imageview.o","type":"object"}]}

#include "imageview.hpp"
#include "range.hpp"
#include <algorithm>

using namespace Tiger;

ImageView::ImageView(Container& cnt,int id):m_id(id)
,m_box(cnt,1)
	,m_entry_max(m_box,0)
	,m_rv_box(m_box.insertMode(Tiger::Box::InsertMode{0,Tiger::Box::FILL|Tiger::Box::EXPAND}),0)
		,m_sep_left(m_rv_box.insertMode(Tiger::Box::InsertMode{0,Tiger::Box::FILL|Tiger::Box::EXPAND}),1)
		,m_rv(m_rv_box,0)
		,m_sep_right(m_rv_box,1)
	,m_entry_min(m_box.insertMode(Tiger::Box::InsertMode{0,0}),0)
	{
	m_entry_max.alignment(1.0f).small(1).width(8).callback(*this);
	m_entry_min.alignment(1.0f).small(1).width(8).callback(*this);
	m_rv.callback(*this);
	}

void ImageView::operator()(RangeView& rv)
	{
	auto r=rv.range();
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
			auto v=std::max(atof(entry.content()),v_min);
			m_rv.range(Tiger::Range(v_min,v));
			break;
			}
		case 1:
			{
			auto v_max=m_rv.range().max();
			auto v=std::min(atof(entry.content()),v_max);
			m_rv.range(Tiger::Range(v,v_max));
			break;
			}
		default:
			break;
		}
	}
