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

			void changed(TextEntry& src);

			void changed(RangeView& src);

			void clicked(Button& src);

			ImageView& image(Image&& img,int channel)=delete;

			ImageView& image(const Image* img,int channel)
				{
				if(img!=nullptr)
					{
					assert(channel>=0 && channel<static_cast<int>(img->channelCount()));
					m_img_display.image(img,channel);
					m_box.sensitive(1);
					}				else
					{
					m_img_display.image(img,channel);
					m_box.sensitive(0);
					}
				return *this;
				}

			ImageView& channel(int channel)
				{
				m_img_display.channel(channel);
				return *this;
				}

			template<class Callback>
			ImageView& displayCallback(Callback& cb)
				{
				m_img_display.callback(cb);
				return *this;
				}

			ImageView& range(const Range& rng) noexcept;

			Range range() const noexcept;

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
