//@	{
//@	"targets":[{"name":"imageview.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"imageview.o","rel":"implementation"}]
//@	}

#ifndef TIGER_IMAGEVIEW_HPP
#define TIGER_IMAGEVIEW_HPP

#include "box.hpp"
#include "textentry.hpp"
#include "rangeview.hpp"
#include "separator.hpp"

namespace Tiger
	{
	class ImageView
		{
		public:
			explicit ImageView(Container& cnt,int id);

			int id() const noexcept
				{return m_id;}

			void operator()(TextEntry& src);

			void operator()(RangeView& src);

		private:
			int m_id;
			Box m_box;
				TextEntry m_entry_max;
				Box m_rv_box;
					Separator m_sep_left;
					RangeView m_rv;
					Separator m_sep_right;
				TextEntry m_entry_min;
		};
	}

#endif // TIGER_IMAGEVIEW_HPP
