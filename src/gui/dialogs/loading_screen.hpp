/*
	Copyright (C) 2016 - 2022
	Part of the Battle for Wesnoth Project https://www.wesnoth.org/

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY.

	See the COPYING file for more details.
*/

#pragma once

#include "gui/dialogs/modal_dialog.hpp"

#include "events.hpp"
#include "tstring.hpp"

#include <atomic>
#include <chrono>
#include <future>
#include <map>
#include <optional>
#include <vector>

namespace cursor
{
	struct setter;
}

/**
 * Loading screen stage IDs.
 * When adding new entries here, don't forget to update the stage_names
 * map with an appropriate description.
 */
enum class loading_stage
{
	build_terrain,
	create_cache,
	init_display,
	init_fonts,
	init_teams,
	init_theme,
	load_config,
	load_data,
	load_level,
	init_lua,
	init_whiteboard,
	load_unit_types,
	load_units,
	refresh_addons,
	start_game,
	verify_cache,
	connect_to_server,
	login_response,
	waiting,
	redirect,
	next_scenario,
	download_level_data,
	download_lobby_data,
	none,
};

namespace gui2
{
class drawing;
class label;
class window;

namespace dialogs
{
class loading_screen : public modal_dialog, public events::pump_monitor
{
public:
	loading_screen(std::function<void()> f);

	~loading_screen();

	static void display(std::function<void()> f);
	static bool displaying() { return singleton_ != nullptr; }

	static void progress(loading_stage stage = loading_stage::none);

private:
	virtual const std::string& window_id() const override;

	virtual void pre_show(window& window) override;

	virtual void post_show(window& window) override;

	/** Inherited from events::pump_monitor. */
	virtual void process(events::pump_info&) override;

	/** Callback to handle drawing the progress animation. */
	void draw_callback();

	static loading_screen* singleton_;

	std::function<void()> load_func_;
	std::future<void> worker_result_;
	std::unique_ptr<cursor::setter> cursor_setter_;

	label* progress_stage_label_;
	drawing* animation_;

	std::optional<decltype(std::chrono::steady_clock::now())> animation_start_;

	std::atomic<loading_stage> current_stage_;

	using stage_map = std::map<loading_stage, t_string>;
	stage_map visible_stages_;
	stage_map::const_iterator current_visible_stage_;
};

} // namespace dialogs
} // namespace gui2
