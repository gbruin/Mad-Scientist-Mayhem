#ifndef MOUSE_ARRAY_H
#define MOUSE_ARRAY_H

const int array_size = 1000;
const float default_life = 15; 

struct Mouse_Pos
{
  float x_pos, y_pos;
  float lifetime;
  bool in_use;
};

class Mouse_Array
{
 public:
  int top_queue, cur_position;
  Mouse_Pos Pos_Array[array_size]; //Saves 1000 positions on the screen to etch in
  Mouse_Array();
  void add_position(float x, float y, float lifetime);
  void update(float dt);
  void purge();
};

Mouse_Array::Mouse_Array()
{
 top_queue = 0;    //Start the stack at 0.
 cur_position = 0; //The top of the stack, add new entries from here.
 for(int i = 0; i < array_size; i++) 
  {
   Pos_Array[i].in_use   = false;
   Pos_Array[i].x_pos    = 0;
   Pos_Array[i].y_pos    = 0; 
   Pos_Array[i].lifetime = 0;
  }
}

void Mouse_Array::add_position(float x, float y, float lifetime)
{
 Pos_Array[cur_position].in_use   = true; //Mark it as used
 Pos_Array[cur_position].x_pos    = x;
 Pos_Array[cur_position].y_pos    = y;
 Pos_Array[cur_position].lifetime = lifetime;
 //Push the topstack up one.
 if(cur_position + 1 == array_size) cur_position = 0;
 else                               cur_position++; 
}

void Mouse_Array::update(float dt)
{
 for(int i = 0; i < array_size; i++) 
 {
  Pos_Array[i].lifetime = Pos_Array[i].lifetime - dt;
  if(Pos_Array[i].lifetime < 0 && Pos_Array[i].in_use) 
   {
    Pos_Array[i].in_use = false; //Kills the memory.
    if(top_queue + 1 == array_size) top_queue = 0;
    else                            top_queue++;    
   }
 }
}

void Mouse_Array::purge()
{
     
}
 
#endif
