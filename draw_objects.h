#ifndef UNTITLED23_DRAW_OBJECTS_H
#define UNTITLED23_DRAW_OBJECTS_H

#include "point.h"
#include "gameobjects.h"
#include "framebuffer.h"
#include "gamehandler.h"
#include "color_funcs.h"

const int half_scH = screenH / 2, half_scW = screenW / 2;

//Pt<int> get_coords_on_screen(Pt<double> p, double h);

//template<typename Type>
//Type get_y(Pt<Type> p1, Pt<Type> p2, Type x);

//void draw_wall(Wall wall, Player pl, FrameBuffer &fb);

void draw_frame(GameHandler &gh, FrameBuffer &fb);

#endif //UNTITLED23_DRAW_OBJECTS_H
