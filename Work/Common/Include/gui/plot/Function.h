// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

// ##################################################################################
// # native Interface Design (natID) ver. 3.2.7 (2024-12-16)
// # Licensed under the Academic Free License, version 3
// # Copyright (C) 2024 Prof. dr Izudin Dzafic 
// # 
// # You may use this code under the terms of the Academic Free License, version 3.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ###################################################################################

#pragma once
#include "gui/Canvas.h"
#include <gui/plot/natPlotLib.h>
#include "gui/Shape.h"
#include "td/String.h"
#include <vector>

#include <deque>

namespace gui
{
namespace plot
{

class NATPLOT_API Function
{
	td::ColorID color = td::ColorID::Black;
	gui::Point *tacke = nullptr, *_tackeUnmodified = nullptr;
	size_t length = 0;
	td::LinePattern pattern = td::LinePattern::Solid;
	double debljina = 1;

	gui::CoordType shiftX = 0;
	gui::CoordType shiftY = 0;

	gui::CoordType scaleX = 1;
	gui::CoordType scaleY = 1;
	
	gui::Point findIntersection(const gui::Point& p1, const gui::Point& p2, const gui::Rect& r);

	bool reDraw = true;
	std::deque<gui::Shape> *lines = nullptr;
	void addToLines(std::vector<gui::Point>& tacke);

public:
	Function() { name = new td::String("line"); };
	Function(gui::CoordType* x, gui::CoordType* y, size_t length, td::ColorID color, const td::String &name = "line", double lineWidth = 1, td::LinePattern pattern = td::LinePattern::Solid);
	Function(gui::Point *points, size_t length, td::ColorID color, const td::String& name = "line", double lineWidth = 1, td::LinePattern pattern = td::LinePattern::Solid);
	Function(const Function&) = delete;
	Function(Function&& f) noexcept;
	Function& operator=(Function&& f) noexcept;

	td::String* name = nullptr;
	void getScale(gui::CoordType& scaleX, gui::CoordType& scaleY) const;
	void getShift(gui::CoordType& shiftX, gui::CoordType& shiftY) const;
	double getLineWidth() const { return debljina; };
	td::LinePattern getPattern() const{ return pattern; };
	td::ColorID getColor() const { return color; }
	size_t getLength() const{ return length; };
	const gui::Point* getPoints() const{ return tacke;}
	void setPoints(gui::CoordType*x, gui::CoordType*y, size_t lenght);
	void setColor(td::ColorID& color) {
		this->color = color;
	}
	void setPattern(td::LinePattern pattern);
	void setLineWidth(double width);
	
	void increaseScaleAndShiftX(const gui::CoordType& scale, const gui::CoordType& shift);
	void increaseScaleAndShiftY(const gui::CoordType& scale, const gui::CoordType& shift);

	void increaseShiftX(const gui::CoordType& shift);
	void increaseShiftY(const gui::CoordType& shift);

	void increaseScaleX(const gui::CoordType& scale);
	void increaseScaleY(const gui::CoordType& scale);

    gui::CoordType realToTransformedX(const gui::CoordType& cord) const;

    gui::CoordType transformedToRealX(const gui::CoordType& cord) const;

    gui::CoordType realToTransformedY(const gui::CoordType& cord) const;

    gui::CoordType TrasformedToRealY(const gui::CoordType& cord) const;

	void draw(const gui::Rect& frame);

    ~Function();
};

} //namepsace plot
} //namespace gui
