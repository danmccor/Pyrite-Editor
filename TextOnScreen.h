#pragma once
#include "textNode.h"
#include "windowFramework.h"
#include "pandaSystem.h"

class TextOnScreen
{
public:
	TextNode text;

	TextOnScreen(WindowFramework* window);
	void SetText(std::string newText);


};

