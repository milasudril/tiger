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
//@	 "targets":[{"name":"simulationeditor.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"simulationeditor.o","rel":"implementation"}]
//@	}

#ifndef TIGER_SIMULATIONEDITOR_HPP
#define TIGER_SIMULATIONEDITOR_HPP

#include "box.hpp"
#include "mapview.hpp"
#include "label.hpp"
#include "buttonlist.hpp"
#include "imageview.hpp"
#include <cassert>

namespace Tiger
	{
	class Simulation;
	class Window;
	class SimulationEditorBase
		{
		private:
			class ParamDataDescriptor;

		public:
			typedef SimulationEditorBase Self;

			explicit SimulationEditorBase(Container& cnt);
			SimulationEditorBase& simulation(Simulation& sim);
			void clicked(ButtonList<Self>& list,Button& btn);
			void itemChanged(MapView<ParamDataDescriptor>& src,float& obj,const char* valstr);
			void clicked(ImageDisplay& src);
			const std::vector<Image>& imagesStaged() const noexcept
				{return m_img_staged;}

		private:
			class ParamDataDescriptor
				{
				public:
					typedef const char* key_type;
					typedef float mapped_type;

					static std::string keyString(const char* str) noexcept
						{
						assert(str!=nullptr);
						return std::string(str);
						}

					static std::string valueString(float x) noexcept
						{
						std::string ret;
						ret.reserve(16);
						sprintf(const_cast<char*>(ret.data()),"%.9g",x);
						return std::move(ret);
						}

					static const char* key() noexcept
						{return "Name";}

					static const char* value() noexcept
						{return "Value";}
				};

			static constexpr ParamDataDescriptor s_desc{};
			Box m_top;
				ButtonList<Self> m_toolbar;
				Box m_lower;
					Box m_left;
						Label m_init_label;
						Box m_init_panels;
							ButtonList<Self> m_init_list;
							ImageView m_img_view;
					Separator m_sep;
					Box m_right;
						Label m_param_label;
						MapView<ParamDataDescriptor> m_params;
			std::vector<Image> m_img_staged;
			std::vector<std::string> m_img_names;
			std::vector<Range> m_img_ranges;
			int m_ch_current;

			virtual void submit()
				{}
		};

	template<class Callback>
	class SimulationEditor:public SimulationEditorBase
		{
		public:
			explicit SimulationEditor(Container& cnt,int id):SimulationEditorBase(cnt)
				,m_id(id),r_cb(nullptr)
				{}

			SimulationEditor& callback(Callback& cb)
				{
				r_cb=&cb;
				return *this;
				}

			int id() const noexcept
				{return m_id;}

			void submit()
				{
				if(r_cb!=nullptr)
					{r_cb->submit(*this);}
				}

		private:
			int m_id;
			Callback* r_cb;
		};
	}

#endif // TIGER_SIMULATIONEDITOR_HPP
