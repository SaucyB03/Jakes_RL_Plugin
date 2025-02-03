#pragma once

#include "GuiBase.h"
#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"
#include "bakkesmod/plugin/PluginSettingsWindow.h"
#include <filesystem>
#include <cmath>

#include "version.h"
constexpr auto plugin_version = stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR) "." stringify(VERSION_PATCH) "." stringify(VERSION_BUILD);

using namespace std;

class Jake_Plugin: public BakkesMod::Plugin::BakkesModPlugin
	,public SettingsWindowBase // Uncomment if you wanna render your own tab in the settings menu
	//,public PluginWindowBase // Uncomment if you want to render your own plugin window
{

	//std::shared_ptr<bool> enabled;

	//Boilerplate
	void onLoad() override;
	void onUnload() override; // Uncomment and implement if you need a unload method
	void jakesBallsFlipping();
	void Hook(CarWrapper car, void* params, std::string eventname);
	void Render(CanvasWrapper canvas);

public:
	void RenderSettings() override; // Uncomment if you wanna render your own tab in the settings menu
	//void RenderWindow() override; // Uncomment if you want to render your own plugin window

private: 
	
	const int DIAL_RES = 1280;
	const float MARGIN_RATIO = 0.27;
	float dial_center = 175.0;
	int screen_res_x = 0;
	int screen_res_y = 0;
	float cur_margin = 94.5;
	float dial_scale = 0.2;
	shared_ptr<bool> enabled = make_shared<bool>(false);
	shared_ptr<float> scale = make_shared<float>(0);
	shared_ptr<bool> enable_dial = make_shared<bool>(true);
	shared_ptr<bool> dodge = make_shared<bool>(true);
	shared_ptr<float> angle = make_shared<float>(0);
	shared_ptr<float> dial_x = make_shared<float>(0);
	shared_ptr<float> dial_y = make_shared<float>(0);
	std::shared_ptr<ImageWrapper> dial_img;
};
