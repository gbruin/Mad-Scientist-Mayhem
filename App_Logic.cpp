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
hgeSprite* Spr_BrushedMetal; hgeSprite* Spr_Scorch; hgeSprite* Spr_WeldParticles; hgeSprite* Spr_GlowParticles; hgeSprite* Spr_MeltParticles;
HTEXTURE   Tex_Blank; hgeSprite* Spr_Blank;

#define NPOS 750
hgeParticleSystem* Par_WeldParticles;
hgeParticleSystem* Par_GlowParticles;
hgeParticleSystem* Par_MeltParticles[NPOS];

//Custom
#include "data\\Hge_Init.h"          //Includes prototype frame and render functions
#include "data\\System_Tools.h"
#include "data\\Graphic_Tools.h"
#include "data\\Buttons.h"
#include "data\\Mouse_Array.h"
#include "data\\BurnRender.h"
#include "data\\Point.h"
#include "data\\Tesla_Coil.h"
#include "data\\Lightning.h"

//Class Data Declarations
Mouse_Array* MouseArray;
bool LClick, LUp;

bool FrameFunc()
{
 hge->Input_GetMousePos(MousePtrX, MousePtrY);
 float delta_time = hge->Timer_GetDelta();
 if(hge->Input_KeyUp(HGEK_Z))  //Purge Function (Works)
  {
   for(int i = 0; i < NPOS; i++)
    {
      xposes[i] = OFFSET;     
      yposes[i] = OFFSET;    
    }
  }
 if(hge->Input_KeyUp(HGEK_LBUTTON)) LUp = true; else LUp = false;
 if(hge->Input_GetKeyState(HGEK_LBUTTON))
  {
   LClick = true;
   Par_WeldParticles->FireAt(MousePosX + 20, MousePosY + 20); //Slight offset
   Par_GlowParticles->FireAt(MousePosX + 20, MousePosY + 20);
   MouseArray->add_position(MousePosX, MousePosY, 1505);
  }
 else
  {
   LClick = false;
   Par_WeldParticles->Stop();                                
   Par_GlowParticles->Stop();   
//   Par_MeltParticles->Stop();  
  }
 Par_WeldParticles->Update(delta_time);
 Par_GlowParticles->Update(delta_time); 
 for(int i = 0; i < NPOS; i++) Par_MeltParticles[i]->Update(delta_time);
 if(hge->Input_GetKeyState(HGEK_ESCAPE)) return true;
 MouseArray->update(delta_time);
 return false;
}

bool RenderFunc()
{
 hge->Gfx_BeginScene();
 hge->Gfx_Clear(0xFF000000);
 Spr_BrushedMetal->Render(0, 0);
 if(LClick) Fade_down(Spr_Blank, 1);
 else       Fade_up  (Spr_Blank, 1, 230);
 RenderBurn(Spr_Scorch, MouseArray, LClick);
 Spr_Blank->Render(0, 0);
 for(int i = 0; i < NPOS; i++) Par_MeltParticles[i]->Render();
 Par_GlowParticles->Render();
 Par_WeldParticles->Render();
// if(LClick) Spr_Blank->Render(0, 0);
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
    
    Spr_GlowParticles = new hgeSprite(Tex_Particles,  0,  0, 32, 32);
    Spr_GlowParticles->SetBlendMode(BLEND_COLORMUL | BLEND_ALPHAADD | BLEND_NOZWRITE);
    Spr_GlowParticles->SetHotSpot(16,16);    
    
    Spr_MeltParticles = new hgeSprite(Tex_Particles,  0,  0, 32, 32);
    Spr_MeltParticles->SetBlendMode(BLEND_COLORMUL | BLEND_ALPHAADD | BLEND_NOZWRITE);
    Spr_MeltParticles->SetHotSpot(16,16);    

    
    Par_WeldParticles = new hgeParticleSystem("gfx\\welding3.psi", Spr_WeldParticles);          
    Par_GlowParticles = new hgeParticleSystem("gfx\\glow.psi"    , Spr_GlowParticles);
    Par_GlowParticles->info.fSizeStart = 30;
    Par_GlowParticles->info.fSizeEnd   = 10;
    Par_GlowParticles->info.nEmission  = 50;
    for(int i = 0; i < NPOS; i++) 
     {
      Par_MeltParticles[i] = new hgeParticleSystem("gfx\\melt1.psi", Spr_MeltParticles);
     }
    
    
    Load_Fonts(Ariel_Font, "fonts\\arial.fnt", 0xFFFFFFFF);
    Load_Graphic(Tex_BrushedMetal, "gfx\\Brushed Metal.png", Spr_BrushedMetal,  1024, 768);
    Load_Graphic(Tex_Scorch      , "gfx\\Scorch.png"       , Spr_Scorch      ,    32,  32);
    Load_Graphic(Tex_Blank       , ""                      , Spr_Blank       ,  1024, 768);
    Spr_Blank->SetColor(0x00000000);
    hge->System_Start();
   }
  //Shutdown
  hge->System_Shutdown();
  hge->Release();
  return 0;
}
