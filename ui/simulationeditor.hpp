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
	class SimulationEditor
		{
		private:
			class ParamDataDescriptor;

		public:
			explicit SimulationEditor(Container& cnt,int id);
			SimulationEditor& simulation(Simulation& sim);
			void operator()(ButtonList<SimulationEditor>& list,Button& btn);
			void operator()(MapView<ParamDataDescriptor>& src,float& obj,const char* valstr);
			void operator()(ImageDisplay& src);

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
			int m_id;
			Simulation* r_sim;
			Box m_top;
				Box m_left;
					Label m_init_label;
					Box m_init_panels;
						ButtonList<SimulationEditor> m_init_list;
						ImageView m_img_view;
				Separator m_sep;
				Box m_right;
					Label m_param_label;
					MapView<ParamDataDescriptor> m_params;
			std::vector<Image> m_img_staged;
			std::vector<std::string> m_img_names;
			std::vector<Range> m_img_ranges;
			int m_ch_current;
		};
	}

#endif // TIGER_SIMULATIONEDITOR_HPP
