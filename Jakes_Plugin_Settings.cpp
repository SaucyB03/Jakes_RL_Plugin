#include "pch.h"
#include "Jake_Plugin.h"

void Jake_Plugin::RenderSettings() {
	ImGui::TextUnformatted("Plugin that tells you the direction of your flips in degrees. An easy way to see exactly how you may have misflipped.");

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

	CVarWrapper scaleCvar = cvarManager->getCvar("jk_scale");

	if (!scaleCvar) { return; }
	float scale = scaleCvar.getFloatValue();
	if (ImGui::SliderFloat("Distance", &scale, 260, screen_res_y / 2)) {
		scaleCvar.setValue(scale);
		cur_margin = scale * MARGIN_RATIO;
		dial_scale = (scale - cur_margin) / DIAL_RES;
		dial_center = scale / 2;		
	}
	if (ImGui::IsItemHovered()) {
		std::string hoverText = "scale is " + std::to_string(scale);
		ImGui::SetTooltip(hoverText.c_str());
	}
}