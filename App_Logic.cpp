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
HTEXTURE   Tex_BrushedMetal; HTEXTURE   Tex_Scorch; HTEXTURE   Tex_Particles;
hgeSprite* Spr_BrushedMetal; hgeSprite* Spr_Scorch; hgeSprite* Spr_WeldParticles;

hgeParticleSystem* Par_WeldParticles;

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
   Par_WeldParticles->FireAt(MousePosX + 20, MousePosY + 20); //Slight offset
   MouseArray->add_position(MousePosX, MousePosY, 1505);
  }
 else
  {
   Par_WeldParticles->Stop();                                    
  }
 Par_WeldParticles->Update(delta_time);
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
 Par_WeldParticles->Render();
 //Debugging text checks
 if(false)
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
    //Welding particle system
    Tex_Particles = hge-> Texture_Load("gfx\\particles.png");
    Spr_WeldParticles = new hgeSprite(Tex_Particles, 64, 32, 32, 32);
    Spr_WeldParticles->SetBlendMode(BLEND_COLORMUL | BLEND_ALPHAADD | BLEND_NOZWRITE);
    Spr_WeldParticles->SetHotSpot(16,16);
    Par_WeldParticles = new hgeParticleSystem("gfx\\welding3.psi", Spr_WeldParticles);          
    
    Load_Fonts(Ariel_Font, "fonts\\arial.fnt", 0xFFFFFFFF);
    Load_Graphic(Tex_BrushedMetal, "gfx\\Brushed Metal.png", Spr_BrushedMetal,  1024, 768);
    Load_Graphic(Tex_Scorch      , "gfx\\Scorch.png"       , Spr_Scorch      ,    32,  32);
    hge->System_Start();
   }
  //Shutdown
  hge->System_Shutdown();
  hge->Release();
  return 0;
}
