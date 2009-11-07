#ifndef OBJECTREC_H
#define OBJECTREC_H

const int Tri_waypoints =  7;
const int Tri_gracetime = 10; 
//Array of waypoints
struct object_recog
{
 float x, y, radius, lifetime;
};

object_recog Triangle[Tri_waypoints];

//Edits touched points and returns true if all waypoints are touched
bool update_waypoints(object_recog Object[], int waypoints, float dt, float mouse_x, float mouse_y) 
{
 bool result = true;
 for(int i = 0; i < waypoints; i++) 
  {
   Object[i].lifetime = Object[i].lifetime - dt;
   if(Calc_Distance(Object[i].x, Object[i].y, mouse_x, mouse_y) < Object[i].radius) 
    {
     if(i != 0 && Triangle[i-1].lifetime > 0) Triangle[i].lifetime = Tri_gracetime;
     if(i == 0)                               Triangle[i].lifetime = Tri_gracetime;
    }
   if(Triangle[i].lifetime <= 0) result = false; //If any of the lifetimes are 0, set result to false
  }
 return result; 
}

#endif
