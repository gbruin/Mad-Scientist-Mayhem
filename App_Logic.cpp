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
HTEXTURE   Tex_BrushedMetal;
hgeSprite* Spr_BrushedMetal;

//Custom
#include "data\\Hge_Init.h"          //Includes prototype frame and render functions
#include "data\\System_Tools.h"
#include "data\\Graphic_Tools.h"
#include "data\\Buttons.h"
#include "data\\Mouse_Array.h"

//Class Data Declarations
Mouse_Array* MouseArray;

bool FrameFunc()
{
 hge->Input_GetMousePos(MousePtrX, MousePtrY);
 if(hge->Input_GetKeyState(HGEK_LBUTTON))
  {
   cout << "Function enter \n";
   MouseArray->add_position(MousePosX, MousePosY, 15);
  }
 if(hge->Input_GetKeyState(HGEK_ESCAPE)) return true;
 return false;
}

bool RenderFunc()
{
 hge->Gfx_BeginScene();
 hge->Gfx_Clear(0xFF000000);
 Spr_BrushedMetal->Render(0, 0);
 //Debugging text checks
 if(true)
 {
  for(int i = 0; i < array_size; i++)
   {
    Ariel_Font->printf(5, 5+ 20*i, HGETEXT_LEFT, "%.3f %.3f %.3f %i", MouseArray->Pos_Array[i].x_pos, MouseArray->Pos_Array[i].y_pos, MouseArray->Pos_Array[i].lifetime, MouseArray->Pos_Array[i].in_use);
   }
  Ariel_Font->printf(1024, 5, HGETEXT_RIGHT, "%.3f %.3f Total Position Adds: %i", MousePosX, MousePosY);
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
    hge->System_Start();
   }
  //Shutdown
  hge->System_Shutdown();
  hge->Release();
  return 0;
}
