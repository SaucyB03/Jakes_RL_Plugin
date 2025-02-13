#include "pch.h"
#include "Jake_Plugin.h"
#include <cmath>


BAKKESMOD_PLUGIN(Jake_Plugin, "Jake's Plugin", plugin_version, PLUGINTYPE_FREEPLAY)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

void Jake_Plugin::onLoad()
{
	_globalCvarManager = cvarManager;

	LOG("Jake's Plugin cuz hes bad");

	//Get screen res
	std::string res = gameWrapper->GetSettings().GetVideoSettings().Resolution;
	size_t x_pos = res.find('x');
	screen_res_x = std::stoi(res.substr(0,x_pos));
	screen_res_y = std::stoi(res.substr(x_pos+1));

	//Define Cvars
	cvarManager->registerCvar("jk_enable", "0", "Enable jake plugin", true, true, 0, true, 1, false).bindTo(enabled);
	cvarManager->registerCvar("jk_scale", "350.0", "Scale of window", true, true, 250, true, screen_res_y/2, false).bindTo(scale);
	cvarManager->registerCvar("jk_enableDial", "0", "Enable the dial", true, true, 0, true, 1, false).bindTo(enable_dial);
	cvarManager->registerCvar("jk_dodge", "0", "Currently Flipping", true, true, 0, true, 1, false).bindTo(dodge);
	cvarManager->registerCvar("jk_angle", "0.0", "Flip Angle", true, true, 0, true, 360, false).bindTo(angle);
	cvarManager->registerCvar("jk_dial_x", "0", "X component for dial line", true, true, -1, true, 1, true).bindTo(dial_x);
	cvarManager->registerCvar("jk_dial_y", "0", "Y component for dial line", true, true, -1, true, 1, true).bindTo(dial_y);
	cvarManager->registerCvar("jk_pos_x", "0.0", "Position of Window", true, true, 0, true, screen_res_x, true).bindTo(pos_x);
	cvarManager->registerCvar("jk_pos_y", "0.0", "Position of Window", true, true, 0, true, screen_res_y, true).bindTo(pos_y);

	gameWrapper->RegisterDrawable([this](CanvasWrapper canvas) {
		Render(canvas);
		});

	gameWrapper->HookEventWithCaller<CarWrapper>("Function TAGame.Car_TA.SetVehicleInput",
		[this](CarWrapper car, void* params, std::string eventname) {
			Hook(car, params, eventname);
		});

	//load image
	std:filesystem::path path = gameWrapper->GetDataFolder() / "Jake_Plugin" / "dial.png";
	LOG(path.string());
	dial_img = std::make_shared<ImageWrapper>(path, true, true);
	dial_img->LoadForCanvas();

	// Initalize scale and margins
	cur_margin = *scale * MARGIN_RATIO;
	dial_scale = (*scale - cur_margin) / DIAL_RES;
	dial_center = *scale / 2;
}


void Jake_Plugin::onUnload() 
{
	LOG("Unloading cuz jake got better");
}

void Jake_Plugin::Hook(CarWrapper car, void* params, std::string eventname) {
		
	
	
	if (!*enabled)
		return;


	if (car.IsNull())
		return;

	if (!gameWrapper->IsInFreeplay() && !gameWrapper->IsInCustomTraining()) {
		cvarManager->getCvar("jk_enable").setValue(0);
		cvarManager->getCvar("jk_enableDial").setValue(0);
		return;
	}

	ControllerInput* input = static_cast<ControllerInput*>(params);
	if (car.IsDodging() && !*dodge) {
		//Update that we are now dodging
		cvarManager->getCvar("jk_dodge").setValue(1);

		Vector unitVec = car.GetDodgeComponent().GetDodgeDirection();

		float cur_angle = 0;

		//Calculate Angle:
		if(unitVec.X == 0.0){
			if (unitVec.Y > 0) {
				cur_angle = 90;
			}
			else {
				cur_angle = 270;
			}
		}
		else {
			cur_angle = atan(unitVec.Y / unitVec.X) * 180 / 3.1415;
			if (unitVec.X < 0) {
				cur_angle = 180 + cur_angle;
			}
			else if (unitVec.Y < 0 && unitVec.X > 0) {
				cur_angle = 360 + cur_angle;
			}

		}

		//Update the angle CVar
		cvarManager->getCvar("jk_angle").setValue(cur_angle);

		//Update X,Y Components in CVar for dial
		cvarManager->getCvar("jk_dial_x").setValue(unitVec.Y);
		cvarManager->getCvar("jk_dial_y").setValue(unitVec.X);
	}
	else if (!car.IsDodging()) {
		cvarManager->getCvar("jk_dodge").setValue(0);
	}
}

void Jake_Plugin::Render(CanvasWrapper canvas) {
	if (!*enabled) {
		return;
	}

	float back_scale_y = *scale;
	if (!*enable_dial) {
		back_scale_y = 47;
	}
		
	// Background tile
	LinearColor background;
	background.R = 102;
	background.G = 184;
	background.B = 218;
	background.A = 125;
	canvas.SetColor(background);
	canvas.SetPosition(Vector2F{ *pos_x, *pos_y });
	canvas.FillBox(Vector2F{*scale, back_scale_y});

	// Render Text for flip angle
	LinearColor text_color;
	text_color.R = 0;
	text_color.G = 255;
	text_color.B = 0;
	text_color.A = 255;
	canvas.SetColor(text_color);
	canvas.SetPosition(Vector2F{ *pos_x + (*scale-200)/2, *pos_y });
	canvas.DrawString("Flip Angle: " + std::format("{:.2f}", *angle), 2.0, 2.0, false);

	if (!*enable_dial) {
		return;
	}
	//Render Dial image
	if (dial_img->IsLoadedForCanvas()) {
		canvas.SetPosition(Vector2F{*pos_x + cur_margin/2,*pos_y + cur_margin/2});
		canvas.DrawTexture(dial_img.get(), dial_scale);
		// there are multiple functions in the canvaswrapper that accept ImageWrapper*
	}

	//Render center dot on dial
	LinearColor center_color;
	center_color.R = 0;
	center_color.G = 0;
	center_color.B = 0;
	center_color.A = 255;
	float dot_width = *scale/35.0;
	canvas.SetColor(center_color);
	canvas.SetPosition(Vector2F{ *pos_x + dial_center - (dot_width/2), *pos_y + dial_center - (dot_width / 2) });
	canvas.FillBox(Vector2F{dot_width, dot_width});

	//Render indicator line
	LinearColor line_color;
	line_color.R = 0;
	line_color.G = 255;
	line_color.B = 0;
	line_color.A = 255;
	canvas.SetColor(line_color);
	Vector2F dial_xy{ *pos_x + dial_center + (*dial_x * dial_scale * DIAL_RES)/2, *pos_y + dial_center + (*dial_y * -dial_scale * DIAL_RES)/2 };
	canvas.DrawLine(Vector2F{ *pos_x + dial_center, *pos_y + dial_center }, dial_xy, 3.0);

}