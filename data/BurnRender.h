#ifndef BURNRENDER_H
#define BURNRENDER_H

const int sprite_interval = 4; //Render a new location every 8 pixels until connection is made.

void RenderArc();

/*
void RenderBurn(hgeSprite* Spr_Burn, Mouse_Array* Burn_Positions)
{ 
 for(int i = Burn_Positions->cur_position; i == Burn_Positions->top_queue; i--)
  {   
   //You must figure out what the previous point is and draw burn marks sequentially from that point to the next.
   //Figure out the distance this point and the last point.
   float Radial_Distance, X1, Y1, X2, Y2;
   if(i != 0) 
    {
     cout << "i does not equal zero \n";
     X1 = Burn_Positions->Pos_Array[i].x_pos;
     Y1 = Burn_Positions->Pos_Array[i].y_pos;
     X2 = Burn_Positions->Pos_Array[i-1].x_pos;
     Y2 = Burn_Positions->Pos_Array[i-1].y_pos;
     Radial_Distance = Calc_Distance(X1, Y1, X2, Y2);
    }     
   else //Exception for looping
    {
     cout << "i does equal zero \n";
     X1 = Burn_Positions->Pos_Array[i].x_pos;
     Y1 = Burn_Positions->Pos_Array[i].y_pos;
     X2 = Burn_Positions->Pos_Array[array_size].x_pos;
     Y2 = Burn_Positions->Pos_Array[array_size].y_pos;
     Radial_Distance = Calc_Distance(X1, Y1, X2, Y2);
     i = array_size;
     
    }     
   //Now we know the radial distance from one dot to another, we must now try to render each point in multiples of 8. To do this, we use a unit vector method.
   float Unit_X = (X2 - X1)/Radial_Distance;
   float Unit_Y = (Y2 - Y1)/Radial_Distance;
   int Repeat_Val = static_cast<int>((Radial_Distance/sprite_interval) + 1);
   cout << "Repeat_Val: " << Repeat_Val << endl; 
   //Now we begin drawing, starting from the newest point to our next point, then we update and finish
   for(int j = 0; j < Repeat_Val; j++)
    {
     Spr_Burn->Render(400, 400);
//     if(Burn_Positions->Pos_Array[i].in_use) Spr_Burn->Render(X1, Y1);
//     if(Burn_Positions->Pos_Array[j].in_use) Spr_Burn->Render(X1 + Unit_X*j, Y1 + Unit_Y*j);
    }
  }
}
*/
#define OFFSET -50.0
float X1;
float Y1;
float X2;
float Y2;
float pX1 = 0.0;
float pY1 = 0.0;
float pX2 = 0.0;
float pY2 = 0.0;
float prev_X = 0.0;
float prev_Y = 0.0;
float xposes[NPOS];
float yposes[NPOS];
float fired[NPOS];
bool initialized_positions = false;
int index = 0;
void RenderXLine(hgeSprite* Spr_Burn, float x1, float y1, float x2, float y2)
{
 if(x2 <= x1)
  {
   float x3;
   x3 = x1; x1 = x2; x2 = x3;
   float y3;
   y3 = y1; y1 = y2; y2 = y3;
  }
 for(float cx = x1; cx < x2; cx += 1.0)
  {
   float slope;
   slope = (y2 - y1) / (x2 - x1);
   float cy;
   cy = (cx - x1) * slope + y1;
   Spr_Burn->Render(cx, cy);
  }
}
void RenderYLine(hgeSprite* Spr_Burn, float x1, float y1, float x2, float y2)
{
 if(y2 <= y1)
 {
   float x3;
   x3 = x1; x1 = x2; x2 = x3;
   float y3;
   y3 = y1; y1 = y2; y2 = y3;
 }
 for(float cy = y1; cy < y2; cy += 1.0)
 {
   float slope;
   slope = (x2 - x1) / (y2 - y1);
   float cx;
   cx = (cy - y1) * slope + x1;
   Spr_Burn->Render(cx, cy);
 }
}
void RenderLine(hgeSprite* Spr_Burn, float x1, float y1, float x2, float y2)
{
     if(x1 == OFFSET || y1 == OFFSET)
     {
      Spr_Burn->Render(x2, y2);
      return;
     }
     if(x2 == OFFSET || y2 == OFFSET)
     {
      Spr_Burn->Render(x1, y1);
      return;
     }
     if(abs(x2-x1) > abs(y2-y1)) 
      {
       RenderXLine(Spr_Burn, x1, y1, x2, y2);
      }
     else                        
      {
       RenderYLine(Spr_Burn, x1, y1, x2, y2);
      }
}
void RenderBurn(hgeSprite* Spr_Burn, Mouse_Array* Burn_Positions, bool Click)
{
     if(!initialized_positions)
     {
       initialized_positions = true;
       for(int i = 0; i < NPOS; i++)
       {
        xposes[i] = OFFSET;
        yposes[i] = OFFSET;
        fired[i]  = -1;
       }
     }
  float x;
  float y;
  if(Click) 
  {
   purge_timer = purgetime;
   hge->Input_GetMousePos(&x, &y);
   int prev_index;
   prev_index = index;
   index++;
   if(index == NPOS) //We've looped our memory space
    {
     index = 0;
    }
  xposes[index] = x;
  yposes[index] = y;
  xposes[index + 1 == NPOS ? 0 : index + 1] = OFFSET;
  yposes[index + 1 == NPOS ? 0 : index + 1] = OFFSET;
//  fired[index + 1 == NPOS ? 0 : index + 1]  = -1;
  }
  float x1;
  float y1;
  x1 = OFFSET;
  y1 = OFFSET;
  //Drawing
  for(int i = 0; i < NPOS; i++)
  {
    //float x1 = xposes[prev_index];
    float x2 = xposes[i];
    //float y1 = yposes[prev_index];
    float y2 = yposes[i];
    fired[i] = fired[i] - hge->Timer_GetDelta();
    if(Calc_Distance(x1, y1, x2, y2) < 150) RenderLine(Spr_Burn, x1, y1, x2, y2);
    if(x2 != OFFSET && y2 != OFFSET) 
     {
      Par_MeltParticles[i] ->FireAt(x1 + 20, y1 + 20);
      //Par_LightParticles[i]->FireAt(x1 + 20, y1 + 20);
     }
    //prev_index = i;
    x1 = x2;
    y1 = y2;
  }
  RenderLine(Spr_Burn, x1, y1, xposes[0], yposes[0]);
}


#endif
