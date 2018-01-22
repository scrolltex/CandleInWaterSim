#pragma once
#include <TGUI/TGUI.hpp>
#include <functional>

namespace gui{
    void CreateGUI(tgui::Gui &gui);
	void UpdateGUI(tgui::Gui &gui);
	
    extern std::function<void()> onApplyCallback;
}
