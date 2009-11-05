//System
#include <cmath>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

//HgeEngine
#include <hge.h>
#include <hgeSprite.h>
#include <hgeFont.h>
#include <hgeparticle.h>
#include <hgeRect.h>
#include <hgeanim.h>

//HGE Data Declarations
HGE* hge = 0;
hgeFont* Ariel_Font;
HTEXTURE   Tex_BrushedMetal; HTEXTURE   Tex_Scorch; 
hgeSprite* Spr_BrushedMetal; hgeSprite* Spr_Scorch; 

//Custom
#include "data\\Hge_Init.h"          //Includes prototype frame and render functions
#include "data\\System_Tools.h"
#include "data\\Graphic_Tools.h"
#include "data\\Buttons.h"
#include "data\\Mouse_Array.h"
#include "data\\BurnRender.h"

//Class Data Declarations
Mouse_Array* MouseArray;

bool FrameFunc()
{
 hge->Input_GetMousePos(MousePtrX, MousePtrY);
 float delta_time = hge->Timer_GetDelta();
 if(hge->Input_GetKeyState(HGEK_LBUTTON))
  {
   MouseArray->add_position(MousePosX, MousePosY, 1505);
  }
 if(hge->Input_GetKeyState(HGEK_ESCAPE)) return true;
 MouseArray->update(delta_time);
 return false;
}

bool RenderFunc()
{
 hge->Gfx_BeginScene();
 hge->Gfx_Clear(0xFF000000);
 Spr_BrushedMetal->Render(0, 0);
 RenderBurn(Spr_Scorch, MouseArray);
 //Debugging text checks
 if(true)
 {
  for(int i = 0; i < array_size; i++)
   {
    Ariel_Font->printf(5, 5+ 20*i, HGETEXT_LEFT, "%.3f %.3f %.3f %i", MouseArray->Pos_Array[i].x_pos, MouseArray->Pos_Array[i].y_pos, MouseArray->Pos_Array[i].lifetime, MouseArray->Pos_Array[i].in_use);
   }
  Ariel_Font->printf(1024,  5, HGETEXT_RIGHT, "%.3f %.3f", MousePosX, MousePosY);
  Ariel_Font->printf(1024, 25, HGETEXT_RIGHT, "Queue Head: %i, Queue Top: %i", MouseArray->top_queue, MouseArray->cur_position);
 }
 hge->Gfx_EndScene();
 return false;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
  //Startup
  Load_HGEINI();
  hge->System_SetState(HGE_FRAMEFUNC,  FrameFunc);
  hge->System_SetState(HGE_RENDERFUNC, RenderFunc);
  if(hge->System_Initiate())
   {
    //Load system resources
    MouseArray = new Mouse_Array();
    Load_Fonts(Ariel_Font, "fonts\\arial.fnt", 0xFFFFFFFF);
    Load_Graphic(Tex_BrushedMetal, "gfx\\Brushed Metal.jpg", Spr_BrushedMetal,  1024, 768);
    Load_Graphic(Tex_Scorch      , "gfx\\Scorch.png"       , Spr_Scorch      ,    32,  32);
    hge->System_Start();
   }
  //Shutdown
  hge->System_Shutdown();
  hge->Release();
  return 0;
}
