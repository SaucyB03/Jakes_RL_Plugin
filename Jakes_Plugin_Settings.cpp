#include "pch.h"
#include "Jake_Plugin.h"

void Jake_Plugin::RenderSettings() {
	ImGui::TextUnformatted("Plugin that tells you the direction of your flips in degrees. An easy way to see exactly how you may have misflipped.");


	// Enabling checkboxes
	CVarWrapper enableCvar = cvarManager->getCvar("jk_enable");
	if (!enableCvar) { return; }
	bool enabled = enableCvar.getBoolValue();
	if (ImGui::Checkbox("Enable Flip plugin", &enabled)) {
		enableCvar.setValue(enabled);
	}
	if (ImGui::IsItemHovered()) {
		ImGui::SetTooltip("Toggle Flip Directional Plugin");
	}

	CVarWrapper enableDialCvar = cvarManager->getCvar("jk_enableDial");
	if (!enableDialCvar) { return; }
	bool enable_dial = enableDialCvar.getBoolValue();
	if (ImGui::Checkbox("Enable Dial GUI", &enable_dial)) {
		enableDialCvar.setValue(enable_dial);
	}
	if (ImGui::IsItemHovered()) {
		ImGui::SetTooltip("Toggle Dial Flip Indicator");
	}

	//Scale controls:
	CVarWrapper scaleCvar = cvarManager->getCvar("jk_scale");

	if (!scaleCvar) { return; }
	float scale = scaleCvar.getFloatValue();
	if (ImGui::SliderFloat("Scale", &scale, 260, screen_res_y / 2)) {
		scaleCvar.setValue(scale);
		cur_margin = scale * MARGIN_RATIO;
		dial_scale = (scale - cur_margin) / DIAL_RES;
		dial_center = scale / 2;		
	}
	if (ImGui::IsItemHovered()) {
		std::string hoverText = "scale is " + std::to_string(scale);
		ImGui::SetTooltip(hoverText.c_str());
	}


	// Position Controls:
	CVarWrapper posXCvar = cvarManager->getCvar("jk_pos_x");

	if (!posXCvar) { return; }
	float posX = posXCvar.getFloatValue();
	if (ImGui::SliderFloat("X Position", &posX, 0, screen_res_x-scale)) {
		posXCvar.setValue(posX);
	}
	if (ImGui::IsItemHovered()) {
		std::string hoverText = "X position is " + std::to_string(posX);
		ImGui::SetTooltip(hoverText.c_str());
	}

	
	CVarWrapper posYCvar = cvarManager->getCvar("jk_pos_y");

	if (!posYCvar) { return; }
	float posY = posYCvar.getFloatValue();
	if (ImGui::SliderFloat("Y Position", &posY, 0, screen_res_y - scale)) {
		posYCvar.setValue(posY);
	}
	if (ImGui::IsItemHovered()) {
		std::string hoverText = "Y position is " + std::to_string(posY);
		ImGui::SetTooltip(hoverText.c_str());
	}

	// Preset #1:
	CVarWrapper presOneCvar = cvarManager->getCvar("jk_preset_one");
	if (!presOneCvar) { return; }
	bool presOne = presOneCvar.getBoolValue();

	// Preset #2:
	CVarWrapper presTwoCvar = cvarManager->getCvar("jk_preset_two");
	if (!presTwoCvar) { return; }
	bool presTwo = presTwoCvar.getBoolValue();

	//Preset #1 Customizer
	if (ImGui::Checkbox("Use Color Preset #1", &presOne)) {
		presOneCvar.setValue(presOne);
		if (presOne && presTwo) {
			presTwoCvar.setValue(false);
		}
	}
	if (ImGui::IsItemHovered()) {
		ImGui::SetTooltip("Set color theme to preset #1");
	}

	CVarWrapper backColorVar1 = cvarManager->getCvar("jk_back_color_one");
	if (!backColorVar1) { return; }
	// converts from 0-255 color to 0.0-1.0 color
	LinearColor backColor1 = backColorVar1.getColorValue() / 255;
	if (ImGui::ColorEdit4("Background Color Preset #1", &backColor1.R, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar)) {
		backColorVar1.setValue(backColor1 * 255);
	}
	CVarWrapper textColorVar1 = cvarManager->getCvar("jk_text_color_one");
	if (!textColorVar1) { return; }
	// converts from 0-255 color to 0.0-1.0 color
	LinearColor textColor1 = textColorVar1.getColorValue() / 255;
	if (ImGui::ColorEdit4("Text Color P1", &textColor1.R, ImGuiColorEditFlags_NoInputs)) {
		textColorVar1.setValue(textColor1 * 255);
	}
	CVarWrapper armColorVar1 = cvarManager->getCvar("jk_arm_color_one");
	if (!armColorVar1) { return; }
	// converts from 0-255 color to 0.0-1.0 color
	LinearColor armColor1 = armColorVar1.getColorValue() / 255;
	if (ImGui::ColorEdit4("Dial Arm Color P1", &armColor1.R, ImGuiColorEditFlags_NoInputs)) {
		armColorVar1.setValue(armColor1 * 255);
	}

	// Update vars for comparison
	presOne = presOneCvar.getBoolValue();
	presTwo = presTwoCvar.getBoolValue();

	//Preset #2 Customizer
	if (ImGui::Checkbox("Use Color Preset #2", &presTwo)) {
		presTwoCvar.setValue(presTwo);
		if (presOne && presTwo) {
			presOneCvar.setValue(false);
		}
	}
	if (ImGui::IsItemHovered()) {
		ImGui::SetTooltip("Set color theme to preset #2");
	}

	CVarWrapper backColorVar2 = cvarManager->getCvar("jk_back_color_two");
	if (!backColorVar2) { return; }
	// converts from 0-255 color to 0.0-1.0 color
	LinearColor backColor2 = backColorVar2.getColorValue() / 255;
	if (ImGui::ColorEdit4("Background Color P2", &backColor2.R, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar)) {
		backColorVar2.setValue(backColor2 * 255);
	}
	CVarWrapper textColorVar2 = cvarManager->getCvar("jk_text_color_two");
	if (!textColorVar2) { return; }
	// converts from 0-255 color to 0.0-1.0 color
	LinearColor textColor2 = textColorVar2.getColorValue() / 255;
	if (ImGui::ColorEdit4("Text Color P2", &textColor2.R, ImGuiColorEditFlags_NoInputs)) {
		textColorVar2.setValue(textColor2 * 255);
	}
	CVarWrapper armColorVar2 = cvarManager->getCvar("jk_arm_color_two");
	if (!armColorVar2) { return; }
	// converts from 0-255 color to 0.0-1.0 color
	LinearColor armColor2 = armColorVar2.getColorValue() / 255;
	if (ImGui::ColorEdit4("Dial Arm Color P2", &armColor2.R, ImGuiColorEditFlags_NoInputs)) {
		armColorVar2.setValue(armColor2 * 255);
	}
}