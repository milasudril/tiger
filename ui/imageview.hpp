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
#include "imagedisplay.hpp"
#include "button.hpp"
#ifndef NDEBUG
#include "../engine/image.hpp"
#include <cassert>
#endif

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

			void operator()(Button& src);

			ImageView& image(Image&& img,int channel)=delete;

			ImageView& image(const Image& img,int channel)
				{
				assert(channel>=0 && channel<static_cast<int>(img.channelCount()));
				m_img_display.image(img,channel);
				m_box.sensitive(1);
				return *this;
				}

		private:
			int m_id;
			Box m_box;
				ImageDisplay m_img_display;
				Box m_range_box;
					Button m_btn_auto;
					TextEntry m_entry_max;
					Box m_rv_box;
						Separator m_sep_left;
						RangeView m_rv;
						Separator m_sep_right;
					TextEntry m_entry_min;
		};
	}

#endif // TIGER_IMAGEVIEW_HPP
