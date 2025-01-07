// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/Font.h>
#include <cnt/Array.h>
#include <td/Color.h>

namespace gui
{
	
class TextAttribs
{
public:
    cnt::Array<Font::Markup, (size_t)gui::Font::Markup::NA> markups;
    gui::Font::Style style;
    td::ColorID colorID;
    
    TextAttribs()
    {
    }
	
	void clean()
	{
        colorID = td::ColorID::SysText;
		style = gui::Font::Style::Normal;
		for(auto& e: markups)
		{
			e = gui::Font::Markup::NA;
		}
	}
};

class TextAttribsRange : public TextAttribs
{
public:
    gui::Range range;
    TextAttribsRange()
    {
    }
};

} //namespace gui
