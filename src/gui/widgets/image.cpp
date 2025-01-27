/*
	Copyright (C) 2008 - 2022
	by Mark de Wever <koraq@xs4all.nl>
	Part of the Battle for Wesnoth Project https://www.wesnoth.org/

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY.

	See the COPYING file for more details.
*/

#define GETTEXT_DOMAIN "wesnoth-lib"

#include "gui/widgets/image.hpp"

#include "picture.hpp" // We want the file in src/

#include "gui/core/widget_definition.hpp"
#include "gui/core/window_builder.hpp"
#include "gui/core/log.hpp"
#include "gui/core/register_widget.hpp"
#include "gui/widgets/settings.hpp"

#include <functional>

#define LOG_SCOPE_HEADER get_control_type() + " [" + id() + "] " + __func__
#define LOG_HEADER LOG_SCOPE_HEADER + ':'

namespace gui2
{

// ------------ WIDGET -----------{

REGISTER_WIDGET(image)

image::image(const implementation::builder_image& builder)
	: styled_widget(builder, type())
{
}

point image::calculate_best_size() const
{
	surface image(::image::get_image(::image::locator(get_label())));

	if(!image) {
		DBG_GUI_L << LOG_HEADER << " empty image return default.\n";
		return get_config_default_size();
	}

	const point minimum = get_config_default_size();
	const point maximum = get_config_maximum_size();

	point result {image->w, image->h};

	if(minimum.x > 0 && result.x < minimum.x) {
		DBG_GUI_L << LOG_HEADER << " increase width to minimum.\n";
		result.x = minimum.x;
	} else if(maximum.x > 0 && result.x > maximum.x) {
		DBG_GUI_L << LOG_HEADER << " decrease width to maximum.\n";
		result.x = maximum.x;
	}

	if(minimum.y > 0 && result.y < minimum.y) {
		DBG_GUI_L << LOG_HEADER << " increase height to minimum.\n";
		result.y = minimum.y;
	} else if(maximum.y > 0 && result.y > maximum.y) {
		DBG_GUI_L << LOG_HEADER << " decrease height to maximum.\n";
		result.y = maximum.y;
	}

	DBG_GUI_L << LOG_HEADER << " result " << result << ".\n";
	return result;
}

void image::set_active(const bool /*active*/)
{
	/* DO NOTHING */
}

bool image::get_active() const
{
	return true;
}

unsigned image::get_state() const
{
	return ENABLED;
}

bool image::disable_click_dismiss() const
{
	return false;
}

// }---------- DEFINITION ---------{

image_definition::image_definition(const config& cfg)
	: styled_widget_definition(cfg)
{
	DBG_GUI_P << "Parsing image " << id << '\n';

	load_resolutions<resolution>(cfg);
}

image_definition::resolution::resolution(const config& cfg)
	: resolution_definition(cfg)
{
	// Note the order should be the same as the enum state_t in image.hpp.
	state.emplace_back(cfg.child("state_enabled"));
}

// }---------- BUILDER -----------{

namespace implementation
{

builder_image::builder_image(const config& cfg) : builder_styled_widget(cfg)
{
}

widget* builder_image::build() const
{
	image* widget = new image(*this);

	DBG_GUI_G << "Window builder: placed image '" << id << "' with definition '"
			  << definition << "'.\n";

	return widget;
}

} // namespace implementation

// }------------ END --------------

} // namespace gui2
