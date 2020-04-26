/*
	Drawing lib for Moonware, made by Moonware Dev Team.
    Copyright (C) 2020  Cursed Torch#1468

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "Drawing.hpp"

#include "Roblox.hpp"

#include <Windows.h>
#include <iostream>
#include <string>

std::vector<DrawingLine*> linedrawings = {};
std::vector<DrawingCircle*> circledrawings = {};
std::vector<DrawingSquare*> squaredrawings = {};
std::vector<DrawingTriangle*> triangledrawings = {};
std::vector<DrawingText*> textdrawings = {};

DrawingVec2::DrawingVec2(float _x, float _y)
{
	x = _x;
	y = _y;
}

DrawingColor::DrawingColor(float _r, float _g, float _b)
{
	r = _r;
	g = _g;
	b = _b;
}

void vector2new(int ls, int x, int y) {
	Roblox::Functions::rlua_getglobal(ls, "Vector2");
	Roblox::Functions::rlua_getfield(ls, -1, "new");
	Roblox::Functions::rlua_pushnumber(ls, x);
	Roblox::Functions::rlua_pushnumber(ls,y);
	Roblox::Functions::rlua_call(ls, 2, 1);
}

void vector2new(int ls, DrawingVec2 dv2) {
	Roblox::Functions::rlua_getglobal(ls, "Vector2");
	Roblox::Functions::rlua_getfield(ls, -1, "new");
	Roblox::Functions::rlua_pushnumber(ls, dv2.x);
	Roblox::Functions::rlua_pushnumber(ls, dv2.y);
	Roblox::Functions::rlua_call(ls, 2, 1);
}

void color3fromrgb(int ls, int r, int g, int b) {
	Roblox::Functions::rlua_getglobal(ls, "Color3");
	Roblox::Functions::rlua_getfield(ls, -1, "fromRGB");
	Roblox::Functions::rlua_pushnumber(ls, r);
	Roblox::Functions::rlua_pushnumber(ls, g);
	Roblox::Functions::rlua_pushnumber(ls, b);
	Roblox::Functions::rlua_call(ls, 3, 1);
}

void color3fromrgb(int ls, DrawingColor dc) {
	Roblox::Functions::rlua_getglobal(ls, "Color3");
	Roblox::Functions::rlua_getfield(ls, -1, "new");
	Roblox::Functions::rlua_pushnumber(ls, dc.r);
	Roblox::Functions::rlua_pushnumber(ls, dc.g);
	Roblox::Functions::rlua_pushnumber(ls, dc.b);
	Roblox::Functions::rlua_call(ls, 3, 1);
}

DrawingVec2 getdwfromvector2(int ls, int idx) {
	Roblox::Functions::rluaL_checktype(ls, idx, RLUA_TUSERDATA);
	Roblox::Functions::rlua_pushvalue(ls, idx);
	Roblox::Functions::rlua_getfield(ls, -1, "X");
	float x = Roblox::Functions::rluaL_checknumber(ls, -1);
	Roblox::Functions::rlua_getfield(ls, -2, "Y");
	float y = Roblox::Functions::rluaL_checknumber(ls, -1);
	return { x, y };
}

DrawingColor getdcfromcolor3(int ls, int idx) {
	Roblox::Functions::rluaL_checktype(ls, idx, RLUA_TUSERDATA);
	Roblox::Functions::rlua_pushvalue(ls, idx);
	Roblox::Functions::rlua_getfield(ls, -1, "r");
	float r = Roblox::Functions::rluaL_checknumber(ls, -1);
	Roblox::Functions::rlua_getfield(ls, -2, "g");
	float g = Roblox::Functions::rluaL_checknumber(ls, -1);
	Roblox::Functions::rlua_getfield(ls, -3, "b");
	float b = Roblox::Functions::rluaL_checknumber(ls, -1);
	return { r, g, b };
}

/* DRAWING : LINE */
static int drawingline_remove(int ls) {	
	Roblox::Functions::rluaL_checktype(ls, 1, RLUA_TUSERDATA);
	DrawingLine& db = *static_cast<DrawingLine*>(Roblox::Functions::rlua_touserdata(ls, 1));
	//printf("bye bye %p\n", &db);
	for (int idx = 0; idx < linedrawings.size(); idx++) {
		if (linedrawings[idx] == &db) {
			linedrawings.erase(linedrawings.begin() + idx);
		}
	}
	return 0;
}

static int drawingline_index(int ls) {
	Roblox::Functions::rluaL_checktype(ls, 1, RLUA_TUSERDATA);
	DrawingLine& dl = *static_cast<DrawingLine*>(Roblox::Functions::rlua_touserdata(ls, 1));
	const char* key = Roblox::Functions::rluaL_checklstring(ls, 2, 0);
	//printf("index start: %p (%s)\n", &dl, key);
	if (0 == std::strcmp(key, "Remove")) {
		Roblox::Functions::rlua_pushcfunction(ls, drawingline_remove);
		return 1;
	}
	else if (0 == std::strcmp(key, "Visible")) {
		Roblox::Functions::rlua_pushboolean(ls, dl.visible);
		return 1;
	}
	else if (0 == std::strcmp(key, "ZIndex")) {
		Roblox::Functions::rlua_pushboolean(ls, dl.zindex);
		return 1;
	}
	else if (0 == std::strcmp(key, "From")) {
		vector2new(ls, dl.from);
		return 1;
	}
	else if (0 == std::strcmp(key, "To")) {
		vector2new(ls, dl.to);
		return 1;
	}
	else if (0 == std::strcmp(key, "Color")) {
		color3fromrgb(ls, dl.color);
		return 1;
	}
	else if (0 == std::strcmp(key, "Thickness")) {
		Roblox::Functions::rlua_pushnumber(ls, dl.thickness);
		return 1;
	}
	else if (0 == std::strcmp(key, "Transparency")) {
		Roblox::Functions::rlua_pushnumber(ls, dl.transparency);
		return 1;
	}
	return 0;
}

static int drawingline_newindex(int ls) {
	Roblox::Functions::rluaL_checktype(ls, 1, RLUA_TUSERDATA);	
	DrawingLine& dl = *static_cast<DrawingLine*>(Roblox::Functions::rlua_touserdata(ls, 1));
	const char* key = Roblox::Functions::rluaL_checklstring(ls, 2, 0);
	//printf("newindex start: %p (%s)\n", &dl, key);
	if (0 == std::strcmp(key, "Visible")) {
		Roblox::Functions::rluaL_checktype(ls, 3, RLUA_TBOOLEAN);
		dl.visible = Roblox::Functions::rlua_toboolean(ls, 3);
		return 0;
	}
	if (0 == std::strcmp(key, "ZIndex")) {
		dl.zindex = Roblox::Functions::rluaL_checknumber(ls, 3);
		return 0;
	}
	else if (0 == std::strcmp(key, "From")) {
		dl.from = getdwfromvector2(ls, 3);
		return 0;
	}
	else if (0 == std::strcmp(key, "To")) {
		dl.to = getdwfromvector2(ls, 3);
		return 0;
	}
	else if (0 == std::strcmp(key, "Color")) {
		dl.color = getdcfromcolor3(ls, 3);
		return 0;
	}
	else if (0 == std::strcmp(key, "Thickness")) {
		dl.thickness = Roblox::Functions::rluaL_checknumber(ls, 3);
		return 0;
	}
	else if (0 == std::strcmp(key, "Transparency")) {
		dl.transparency = Roblox::Functions::rluaL_checknumber(ls, 3);
		return 0;
	}
	return 0;
}

/* DRAWING : CIRCLE */

static int drawingcircle_remove(int ls) {
	Roblox::Functions::rluaL_checktype(ls, 1, RLUA_TUSERDATA);
	DrawingCircle& db = *static_cast<DrawingCircle*>(Roblox::Functions::rlua_touserdata(ls, 1));
	//printf("bye bye %p\n", &db);
	for (int idx = 0; idx < circledrawings.size(); idx++) {
		if (circledrawings[idx] == &db) {
			circledrawings.erase(circledrawings.begin() + idx);
		}
	}
	return 0;
}

static int drawingcircle_index(int ls) {
	Roblox::Functions::rluaL_checktype(ls, 1, RLUA_TUSERDATA);
	DrawingCircle& dc = *static_cast<DrawingCircle*>(Roblox::Functions::rlua_touserdata(ls, 1));
	const char* key = Roblox::Functions::rluaL_checklstring(ls, 2, 0);
	//printf("index start: %p (%s)\n", &dc, key);
	if (0 == std::strcmp(key, "Remove")) {
		Roblox::Functions::rlua_pushcfunction(ls, drawingcircle_remove);
		return 1;
	}
	else if (0 == std::strcmp(key, "Visible")) {
		Roblox::Functions::rlua_pushboolean(ls, dc.visible);
		return 1;
	}
	else if (0 == std::strcmp(key, "ZIndex")) {
		Roblox::Functions::rlua_pushboolean(ls, dc.zindex);
		return 1;
	}
	else if (0 == std::strcmp(key, "Position")) {
		vector2new(ls, dc.position);
		return 1;
	}
	else if (0 == std::strcmp(key, "Radius")) {
		Roblox::Functions::rlua_pushnumber(ls, dc.radius);
		return 1;
	}
	else if (0 == std::strcmp(key, "Color")) {
		color3fromrgb(ls, dc.color);
		return 1;
	}
	else if (0 == std::strcmp(key, "Thickness")) {
		Roblox::Functions::rlua_pushnumber(ls, dc.thickness);
		return 1;
	}
	else if (0 == std::strcmp(key, "Filled")) {
		Roblox::Functions::rlua_pushboolean(ls, dc.filled);
		return 1;
	}
	else if (0 == std::strcmp(key, "Transparency")) {
		Roblox::Functions::rlua_pushnumber(ls, dc.transparency);
		return 1;
	}
	else if (0 == std::strcmp(key, "NumSides")) {
		Roblox::Functions::rlua_pushnumber(ls, dc.numsides);
		return 1;
	}
	return 0;
}

static int drawingcircle_newindex(int ls) {
	Roblox::Functions::rluaL_checktype(ls, 1, RLUA_TUSERDATA);
	DrawingCircle& dc = *static_cast<DrawingCircle*>(Roblox::Functions::rlua_touserdata(ls, 1));
	const char* key = Roblox::Functions::rluaL_checklstring(ls, 2, 0);
	//printf("newindex start: %p (%s)\n", &dc, key);
	if (0 == std::strcmp(key, "Visible")) {
		Roblox::Functions::rluaL_checktype(ls, 3, RLUA_TBOOLEAN);
		dc.visible = Roblox::Functions::rlua_toboolean(ls, 3);
	}
	else if (0 == std::strcmp(key, "ZIndex")) {
		dc.zindex = Roblox::Functions::rluaL_checknumber(ls, 3);
	}
	else if (0 == std::strcmp(key, "Position")) {
		dc.position = getdwfromvector2(ls, 3);
	}
	else if (0 == std::strcmp(key, "Radius")) {
		dc.radius = Roblox::Functions::rluaL_checknumber(ls, 3);
	}
	else if (0 == std::strcmp(key, "Color")) {
		dc.color = getdcfromcolor3(ls, 3);
	}
	else if (0 == std::strcmp(key, "Thickness")) {
		dc.thickness = Roblox::Functions::rluaL_checknumber(ls, 3);
	}
	else if (0 == std::strcmp(key, "Filled")) {
		Roblox::Functions::rluaL_checktype(ls, 3, RLUA_TBOOLEAN);
		dc.filled = Roblox::Functions::rlua_toboolean(ls, 3);
	}
	else if (0 == std::strcmp(key, "Transparency")) {
		dc.transparency = Roblox::Functions::rluaL_checknumber(ls, 3);
	}
	else if (0 == std::strcmp(key, "NumSides")) {
		dc.numsides = Roblox::Functions::rluaL_checknumber(ls, 3);
	}
	return 0;
}

/* DRAWING : SQUARE */

static int drawingsquare_remove(int ls) {
	Roblox::Functions::rluaL_checktype(ls, 1, RLUA_TUSERDATA);
	DrawingSquare& db = *static_cast<DrawingSquare*>(Roblox::Functions::rlua_touserdata(ls, 1));
	//printf("bye bye %p\n", &db);
	for (int idx = 0; idx < squaredrawings.size(); idx++) {
		if (squaredrawings[idx] == &db) {
			squaredrawings.erase(squaredrawings.begin() + idx);
		}
	}
	return 0;
}

static int drawingsquare_index(int ls) {
	Roblox::Functions::rluaL_checktype(ls, 1, RLUA_TUSERDATA);
	DrawingSquare& dc = *static_cast<DrawingSquare*>(Roblox::Functions::rlua_touserdata(ls, 1));
	const char* key = Roblox::Functions::rluaL_checklstring(ls, 2, 0);
	//printf("index start: %p (%s)\n", &dc, key);
	if (0 == std::strcmp(key, "Remove")) {
		Roblox::Functions::rlua_pushcfunction(ls, drawingsquare_remove);
		return 1;
	}
	else if (0 == std::strcmp(key, "Visible")) {
		Roblox::Functions::rlua_pushboolean(ls, dc.visible);
		return 1;
	}
	else if (0 == std::strcmp(key, "ZIndex")) {
		Roblox::Functions::rlua_pushboolean(ls, dc.zindex);
		return 1;
	}
	else if (0 == std::strcmp(key, "Position")) {
		vector2new(ls, dc.position);
		return 1;
	}
	else if (0 == std::strcmp(key, "Size")) {
		vector2new(ls, dc.size);
		return 1;
	}
	else if (0 == std::strcmp(key, "Color")) {
		color3fromrgb(ls, dc.color);
		return 1;
	}
	else if (0 == std::strcmp(key, "Thickness")) {
		Roblox::Functions::rlua_pushnumber(ls, dc.thickness);
		return 1;
	}
	else if (0 == std::strcmp(key, "Filled")) {
		Roblox::Functions::rlua_pushboolean(ls, dc.filled);
		return 1;
	}
	else if (0 == std::strcmp(key, "Transparency")) {
		Roblox::Functions::rlua_pushnumber(ls, dc.transparency);
		return 1;
	}
	return 0;
}

static int drawingsquare_newindex(int ls) {
	Roblox::Functions::rluaL_checktype(ls, 1, RLUA_TUSERDATA);
	DrawingSquare& dc = *static_cast<DrawingSquare*>(Roblox::Functions::rlua_touserdata(ls, 1));
	const char* key = Roblox::Functions::rluaL_checklstring(ls, 2, 0);
	//printf("newindex start: %p (%s)\n", &dc, key);
	if (0 == std::strcmp(key, "Visible")) {
		Roblox::Functions::rluaL_checktype(ls, 3, RLUA_TBOOLEAN);
		dc.visible = Roblox::Functions::rlua_toboolean(ls, 3);
	}
	else if (0 == std::strcmp(key, "ZIndex")) {
		dc.zindex = Roblox::Functions::rluaL_checknumber(ls, 3);
	}
	else if (0 == std::strcmp(key, "Position")) {
		dc.position = getdwfromvector2(ls, 3);
	}
	else if (0 == std::strcmp(key, "Size")) {
		dc.size = getdwfromvector2(ls, 3);
	}
	else if (0 == std::strcmp(key, "Color")) {
		dc.color = getdcfromcolor3(ls, 3);
	}
	else if (0 == std::strcmp(key, "Thickness")) {
		dc.thickness = Roblox::Functions::rluaL_checknumber(ls, 3);
	}
	else if (0 == std::strcmp(key, "Filled")) {
		Roblox::Functions::rluaL_checktype(ls, 3, RLUA_TBOOLEAN);
		dc.filled = Roblox::Functions::rlua_toboolean(ls, 3);
	}
	else if (0 == std::strcmp(key, "Transparency")) {
		dc.transparency = Roblox::Functions::rluaL_checknumber(ls, 3);
	}
	return 0;
}

/* DRAWING : TRIANGLE */

static int drawingtriangle_remove(int ls) {
	Roblox::Functions::rluaL_checktype(ls, 1, RLUA_TUSERDATA);
	DrawingTriangle& db = *static_cast<DrawingTriangle*>(Roblox::Functions::rlua_touserdata(ls, 1));
	//printf("bye bye %p\n", &db);
	for (int idx = 0; idx < triangledrawings.size(); idx++) {
		if (triangledrawings[idx] == &db) {
			triangledrawings.erase(triangledrawings.begin() + idx);
		}
	}
	return 0;
}

static int drawingtriangle_index(int ls) {
	Roblox::Functions::rluaL_checktype(ls, 1, RLUA_TUSERDATA);
	DrawingTriangle& dc = *static_cast<DrawingTriangle*>(Roblox::Functions::rlua_touserdata(ls, 1));
	const char* key = Roblox::Functions::rluaL_checklstring(ls, 2, 0);
	//printf("index start: %p (%s)\n", &dc, key);
	if (0 == std::strcmp(key, "Remove")) {
		Roblox::Functions::rlua_pushcfunction(ls, drawingtriangle_remove);
		return 1;
	}
	else if (0 == std::strcmp(key, "Visible")) {
		Roblox::Functions::rlua_pushboolean(ls, dc.visible);
		return 1;
	}
	else if (0 == std::strcmp(key, "ZIndex")) {
		Roblox::Functions::rlua_pushboolean(ls, dc.zindex);
		return 1;
	}
	else if (0 == std::strcmp(key, "PointA")) {
		vector2new(ls, dc.pointa);
		return 1;
	}
	else if (0 == std::strcmp(key, "PointB")) {
		vector2new(ls, dc.pointb);
		return 1;
	}
	else if (0 == std::strcmp(key, "PointC")) {
		vector2new(ls, dc.pointc);
		return 1;
	}
	else if (0 == std::strcmp(key, "Color")) {
		color3fromrgb(ls, dc.color);
		return 1;
	}
	else if (0 == std::strcmp(key, "Thickness")) {
		Roblox::Functions::rlua_pushnumber(ls, dc.thickness);
		return 1;
	}
	else if (0 == std::strcmp(key, "Filled")) {
		Roblox::Functions::rlua_pushboolean(ls, dc.filled);
		return 1;
	}
	else if (0 == std::strcmp(key, "Transparency")) {
		Roblox::Functions::rlua_pushnumber(ls, dc.transparency);
		return 1;
	}
	return 0;
}

static int drawingtriangle_newindex(int ls) {
	Roblox::Functions::rluaL_checktype(ls, 1, RLUA_TUSERDATA);
	DrawingTriangle& dc = *static_cast<DrawingTriangle*>(Roblox::Functions::rlua_touserdata(ls, 1));
	const char* key = Roblox::Functions::rluaL_checklstring(ls, 2, 0);
	//printf("newindex start: %p (%s)\n", &dc, key);
	if (0 == std::strcmp(key, "Visible")) {
		Roblox::Functions::rluaL_checktype(ls, 3, RLUA_TBOOLEAN);
		dc.visible = Roblox::Functions::rlua_toboolean(ls, 3);
	}
	else if (0 == std::strcmp(key, "ZIndex")) {
		dc.zindex = Roblox::Functions::rluaL_checknumber(ls, 3);
	}
	else if (0 == std::strcmp(key, "PointA")) {
		dc.pointa = getdwfromvector2(ls, 3);
	}
	else if (0 == std::strcmp(key, "PointB")) {
		dc.pointb = getdwfromvector2(ls, 3);
	}
	else if (0 == std::strcmp(key, "PointC")) {
		dc.pointc = getdwfromvector2(ls, 3);
	}
	else if (0 == std::strcmp(key, "Color")) {
		dc.color = getdcfromcolor3(ls, 3);
	}
	else if (0 == std::strcmp(key, "Thickness")) {
		dc.thickness = Roblox::Functions::rluaL_checknumber(ls, 3);
	}
	else if (0 == std::strcmp(key, "Filled")) {
		Roblox::Functions::rluaL_checktype(ls, 3, RLUA_TBOOLEAN);
		dc.filled = Roblox::Functions::rlua_toboolean(ls, 3);
	}
	else if (0 == std::strcmp(key, "Transparency")) {
		dc.transparency = Roblox::Functions::rluaL_checknumber(ls, 3);
	}
	return 0;
}

/* DRAWING : TEXT */

static int drawingtext_remove(int ls) {
	Roblox::Functions::rluaL_checktype(ls, 1, RLUA_TUSERDATA);
	DrawingText& db = *static_cast<DrawingText*>(Roblox::Functions::rlua_touserdata(ls, 1));
	//printf("bye bye %p\n", &db);
	for (int idx = 0; idx < textdrawings.size(); idx++) {
		if (textdrawings[idx] == &db) {
			textdrawings.erase(textdrawings.begin() + idx);
		}
	}
	return 0;
}

static int drawingtext_index(int ls) {
	Roblox::Functions::rluaL_checktype(ls, 1, RLUA_TUSERDATA);
	DrawingText& dc = *static_cast<DrawingText*>(Roblox::Functions::rlua_touserdata(ls, 1));
	const char* key = Roblox::Functions::rluaL_checklstring(ls, 2, 0);
	//printf("index start: %p (%s)\n", &dc, key);
	if (0 == std::strcmp(key, "Remove")) {
		Roblox::Functions::rlua_pushcfunction(ls, drawingtext_remove);
		return 1;
	}
	else if (0 == std::strcmp(key, "Visible")) {
		Roblox::Functions::rlua_pushboolean(ls, dc.visible);
		return 1;
	}
	else if (0 == std::strcmp(key, "ZIndex")) {
		Roblox::Functions::rlua_pushboolean(ls, dc.zindex);
		return 1;
	}
	else if (0 == std::strcmp(key, "Text")) {
		Roblox::Functions::rlua_pushlstring(ls, dc.text, dc.textlen);
		return 1;
	}
	else if (0 == std::strcmp(key, "Position")) {
		vector2new(ls, dc.position);
		return 1;
	}
	else if (0 == std::strcmp(key, "Color")) {
		color3fromrgb(ls, dc.color);
		return 1;
	}
	else if (0 == std::strcmp(key, "Center")) {
		Roblox::Functions::rlua_pushboolean(ls, dc.center);
		return 1;
	}
	else if (0 == std::strcmp(key, "Outline")) {
		Roblox::Functions::rlua_pushboolean(ls, dc.outline);
		return 1;
	}
	else if (0 == std::strcmp(key, "Transparency")) {
		Roblox::Functions::rlua_pushnumber(ls, dc.transparency);
		return 1;
	}
	else if (0 == std::strcmp(key, "Size")) {
		Roblox::Functions::rlua_pushnumber(ls, dc.size);
		return 1;
	}
	else if (0 == std::strcmp(key, "TextBounds")) {
		vector2new(ls, dc.textbounds);
		return 1;
	}
	return 0;
}

static int drawingtext_newindex(int ls) {
	Roblox::Functions::rluaL_checktype(ls, 1, RLUA_TUSERDATA);
	DrawingText& dc = *static_cast<DrawingText*>(Roblox::Functions::rlua_touserdata(ls, 1));
	const char* key = Roblox::Functions::rluaL_checklstring(ls, 2, 0);
	//printf("newindex start: %p (%s)\n", &dc, key);
	if (0 == std::strcmp(key, "Visible")) {
		Roblox::Functions::rluaL_checktype(ls, 3, RLUA_TBOOLEAN);
		dc.visible = Roblox::Functions::rlua_toboolean(ls, 3);
	}
	else if (0 == std::strcmp(key, "ZIndex")) {
		dc.zindex = Roblox::Functions::rluaL_checknumber(ls, 3);
	}
	else if (0 == std::strcmp(key, "Text")) {
		unsigned int len;
		const char* text = Roblox::Functions::rluaL_checklstring(ls, 3, &len);
		dc.text = text;
		dc.textend = text + len;
		dc.textlen = len;
	}
	else if (0 == std::strcmp(key, "Position")) {
		dc.position = getdwfromvector2(ls, 3);
	}
	else if (0 == std::strcmp(key, "Color")) {
		dc.color = getdcfromcolor3(ls, 3);
	}
	else if (0 == std::strcmp(key, "Center")) {
		Roblox::Functions::rluaL_checktype(ls, 3, RLUA_TBOOLEAN);
		dc.center = Roblox::Functions::rlua_toboolean(ls, 3);
	}
	else if (0 == std::strcmp(key, "Outline")) {
		Roblox::Functions::rluaL_checktype(ls, 3, RLUA_TBOOLEAN);
		dc.outline = Roblox::Functions::rlua_toboolean(ls, 3);
	}
	else if (0 == std::strcmp(key, "Transparency")) {
		dc.transparency = Roblox::Functions::rluaL_checknumber(ls, 3);
	}
	else if (0 == std::strcmp(key, "Size")) {
		dc.size = Roblox::Functions::rluaL_checknumber(ls, 3);
	}
	return 0;
}

void initmetatables(int ls) {
	/* DRAWING : LINE */
	Roblox::Functions::rluaL_newmetatable(ls, "drawingline");

	Roblox::Functions::rlua_pushcfunction(ls, drawingline_index);
	Roblox::Functions::rlua_setfield(ls, -2, "__index");

	Roblox::Functions::rlua_pushcfunction(ls, drawingline_newindex);
	Roblox::Functions::rlua_setfield(ls, -2, "__newindex");

	Roblox::Functions::rlua_pushcfunction(ls, drawingline_remove);
	Roblox::Functions::rlua_setfield(ls, -2, "__gc");

	Roblox::Functions::rlua_pop(ls, 1);

	/* DRAWING : CIRCLE */
	Roblox::Functions::rluaL_newmetatable(ls, "drawingcircle");

	Roblox::Functions::rlua_pushcfunction(ls, drawingcircle_index);
	Roblox::Functions::rlua_setfield(ls, -2, "__index");

	Roblox::Functions::rlua_pushcfunction(ls, drawingcircle_newindex);
	Roblox::Functions::rlua_setfield(ls, -2, "__newindex");

	Roblox::Functions::rlua_pushcfunction(ls, drawingcircle_remove);
	Roblox::Functions::rlua_setfield(ls, -2, "__gc");

	Roblox::Functions::rlua_pop(ls, 1);

	/* DRAWING : SQUARE */
	Roblox::Functions::rluaL_newmetatable(ls, "drawingsquare");

	Roblox::Functions::rlua_pushcfunction(ls, drawingsquare_index);
	Roblox::Functions::rlua_setfield(ls, -2, "__index");

	Roblox::Functions::rlua_pushcfunction(ls, drawingsquare_newindex);
	Roblox::Functions::rlua_setfield(ls, -2, "__newindex");

	Roblox::Functions::rlua_pushcfunction(ls, drawingsquare_remove);
	Roblox::Functions::rlua_setfield(ls, -2, "__gc");

	Roblox::Functions::rlua_pop(ls, 1);

	/* DRAWING : TRIANGLE */
	Roblox::Functions::rluaL_newmetatable(ls, "drawingtriangle");

	Roblox::Functions::rlua_pushcfunction(ls, drawingtriangle_index);
	Roblox::Functions::rlua_setfield(ls, -2, "__index");

	Roblox::Functions::rlua_pushcfunction(ls, drawingtriangle_newindex);
	Roblox::Functions::rlua_setfield(ls, -2, "__newindex");

	Roblox::Functions::rlua_pushcfunction(ls, drawingtriangle_remove);
	Roblox::Functions::rlua_setfield(ls, -2, "__gc");

	Roblox::Functions::rlua_pop(ls, 1);

	/* DRAWING : TEXT */
	Roblox::Functions::rluaL_newmetatable(ls, "drawingtext");

	Roblox::Functions::rlua_pushcfunction(ls, drawingtext_index);
	Roblox::Functions::rlua_setfield(ls, -2, "__index");

	Roblox::Functions::rlua_pushcfunction(ls, drawingtext_newindex);
	Roblox::Functions::rlua_setfield(ls, -2, "__newindex");

	Roblox::Functions::rlua_pushcfunction(ls, drawingtext_remove);
	Roblox::Functions::rlua_setfield(ls, -2, "__gc");

	Roblox::Functions::rlua_pop(ls, 1);
}

extern void DisplayStack(int ls);
extern std::string random_string(size_t length);

static int drawing_new(int ls) {
	const char* type = Roblox::Functions::rluaL_checklstring(ls, 1, 0);
	//printf("start: %s\n", type);
	void* udata;
	if (std::string(type) == "Line") {
		udata = Roblox::Functions::rlua_newuserdata(ls, sizeof(DrawingLine));
		Roblox::Functions::rluaL_setmetatable(ls, "drawingline");
		new (udata) DrawingLine;
		linedrawings.push_back((DrawingLine*)udata);
	}
	else if (std::string(type) == "Circle") {
		udata = Roblox::Functions::rlua_newuserdata(ls, sizeof(DrawingCircle));
		Roblox::Functions::rluaL_setmetatable(ls, "drawingcircle");
		new (udata) DrawingCircle;
		circledrawings.push_back((DrawingCircle*)udata);
	}
	else if (std::string(type) == "Square") {
		udata = Roblox::Functions::rlua_newuserdata(ls, sizeof(DrawingSquare));
		Roblox::Functions::rluaL_setmetatable(ls, "drawingsquare");
		new (udata) DrawingSquare;
		squaredrawings.push_back((DrawingSquare*)udata);
	}
	else if (std::string(type) == "Triangle") {
		udata = Roblox::Functions::rlua_newuserdata(ls, sizeof(DrawingTriangle));
		Roblox::Functions::rluaL_setmetatable(ls, "drawingtriangle");
		new (udata) DrawingTriangle;
		triangledrawings.push_back((DrawingTriangle*)udata);
	}
	else if (std::string(type) == "Text") {
		udata = Roblox::Functions::rlua_newuserdata(ls, sizeof(DrawingText));
		Roblox::Functions::rluaL_setmetatable(ls, "drawingtext");
		new (udata) DrawingText;
		textdrawings.push_back((DrawingText*)udata);
	}
	else {
		//printf("oops: %s\n", type);
		Roblox::Functions::rluaL_error(ls, "Unsupported Drawing class.");
		return 0;
	}
	//printf("end: %s\n", type);
	return 1;
}

static int drawing_clear(int ls) {
	linedrawings.clear();
	circledrawings.clear();
	squaredrawings.clear();
	triangledrawings.clear();
	textdrawings.clear();
	return 0;
}

void newfunc(int ls, const char* name, rbx_CFunction func) {
	Roblox::Functions::rlua_pushstring(ls, name);
	Roblox::Functions::rlua_pushcfunction(ls, func);
	Roblox::Functions::rlua_settable(ls, -3);
}

void Drawing::Register(int ls)
{
	initmetatables(ls);

	Roblox::Functions::rlua_createtable(ls, 0, 0);
	newfunc(ls, "new", drawing_new);
	newfunc(ls, "clear", drawing_clear);
	Roblox::Functions::rlua_setglobal(ls, "Drawing");
	Roblox::Functions::rlua_pop(ls, 1);
	Roblox::Functions::rlua_settop(Roblox::GetLuaState(), 0);
}

std::vector<DrawingLine*> Drawing::GetLineDrawings()
{
	return linedrawings;
}

std::vector<DrawingCircle*> Drawing::GetCircleDrawings()
{
	return circledrawings;
}

std::vector<DrawingSquare*> Drawing::GetSquareDrawings()
{
	return squaredrawings;
}

std::vector<DrawingTriangle*> Drawing::GetTriangleDrawings()
{
	return triangledrawings;
}

std::vector<DrawingText*> Drawing::GetTextDrawings()
{
	return textdrawings;
}

DrawingBase::~DrawingBase()
{
}
