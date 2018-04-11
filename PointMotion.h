#ifndef POINTMOTION_H
#define POINTMOTION_H

struct Point
   {
   Point(double _x, double _y):x(_x),y(_y){}
   Point(){}
   double x,y;
   };
   
struct Motion
   {
   Point frompt;
   Point topt;
   };

#endif

