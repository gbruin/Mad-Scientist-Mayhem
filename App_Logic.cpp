//System
#include <cmath>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

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
HTEXTURE   Tex_Tesla1; hgeSprite* Spr_Tesla1;
HTEXTURE   Tex_Tesla2; hgeSprite* Spr_Tesla2;

HTEXTURE   Tex_BrushedMetal; HTEXTURE   Tex_Scorch; HTEXTURE   Tex_Particles;
hgeSprite* Spr_BrushedMetal; hgeSprite* Spr_Scorch; hgeSprite* Spr_WeldParticles; hgeSprite* Spr_GlowParticles; hgeSprite* Spr_MeltParticles;
HTEXTURE   Tex_Blank; hgeSprite* Spr_Blank;

#define NPOS 750
#define purgetime 10.0
float purge_timer = purgetime; 
bool  Keep_msg    = false;
hgeParticleSystem* Par_WeldParticles;
hgeParticleSystem* Par_GlowParticles;
hgeParticleSystem* Par_MeltParticles[NPOS];
hgeParticleSystem* Par_LightParticles[NPOS];

//Custom
#include "data\\Hge_Init.h"          //Includes prototype frame and render functions
#include "data\\System_Tools.h"
#include "data\\Graphic_Tools.h"
#include "data\\Buttons.h"
#include "data\\_DasHard.h"
#include "data\\Mouse_Array.h"
#include "data\\Point.h"
#include "data\\Tesla_Coil.h"
#include "data\\Lightning.h"
#include "data\\Lightning2.h"
#include "data\\BurnRender.h"
#include "data\\BoltRender.h"
#include "data\\ObjectRec.h"

//loightfucker, motherfucker
#include <iostream>
#include <Windows.h>

void turnLightsOn();
void turnLightsOff();
void turnLightsRed();

int HardDllOpen()
{
	g_dasusbdll = LoadLibrary("DasHard2006.dll");
	if (g_dasusbdll)
		DasUsbCommand  = (DASHARDCOMMAND)::GetProcAddress((HMODULE)g_dasusbdll, "DasUsbCommand");
	if (DasUsbCommand)
		return 1;
	return 0;
}

int HardDllClose()
{
	if (g_dasusbdll!=NULL)
		return	FreeLibrary(g_dasusbdll);
	return 0;
}

int HardDllCommand(int command, int param, unsigned char *bloc)
{
	if (DasUsbCommand)
		return (*DasUsbCommand)(command, param, bloc);
	return 0;
}
/* */

//Class Data Declarations
Mouse_Array* MouseArray;
bool LClick, LUp;
bool delay_burn;
bool On_Off; float Shine_Time;

//Timed booleans
float Timer1 = 0; float Timer2 = 0; float Timer3 = 0;
//Exit Touch Positions
float LeftX = 50; float RightX  = Screen_Width  - 50;
float TopY  = 50; float BottomY = Screen_Height - 50;

bool FrameFunc()
{
 delay_burn = false;
 hge->Input_GetMousePos(MousePtrX, MousePtrY);
 float delta_time = hge->Timer_GetDelta();
 
 purge_timer = purge_timer - delta_time;
 
 if(hge->Input_KeyUp(HGEK_LBUTTON)) LUp = true; else LUp = false;
 if(hge->Input_GetKeyState(HGEK_LBUTTON))
  {
   if(update_waypoints(Triangle, Tri_waypoints, delta_time, MousePosX, MousePosY)) //If triangle is drawn, returns true;
    {
     if(Shine_Time == -1) Shine_Time = 10; //Time to cycle = 10 seconds
     if(On_Off)turnLightsOn(); else turnLightsRed();
     Shine_Time = Shine_Time - delta_time;
     On_Off = !On_Off;
     if(Shine_Time <  0) Shine_Time = -1;  //Reset
    } 
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
  }
 Par_WeldParticles->Update(delta_time);
 Par_GlowParticles->Update(delta_time); 
 for(int i = 0; i < NPOS; i++) 
  {
   Par_MeltParticles[i] ->Update(delta_time);
   Par_LightParticles[i]->Update(delta_time);
  }
 
 //Message Toggle
 //Corner4 Collision Check
 if(Calc_Distance(MousePosX, MousePosY, LeftX,  BottomY) < 50 && hge->Input_KeyUp(HGEK_LBUTTON)) 
  {Keep_msg = !Keep_msg;  delay_burn = true;}
 
 //Purge Condition: Touching all 4 corners of the screen
 Timer1 = Timer1 - delta_time; Timer2 = Timer2 - delta_time; Timer3 = Timer3 - delta_time; //Updates
 //Collision Detection
 //Corner1 Collision Check
 if(Calc_Distance(MousePosX, MousePosY, LeftX,  TopY) < 50 && LClick)                   {Timer1 = 10; delay_burn = true;} //Collision made
 //Corner2 Collision Check
 if(Calc_Distance(MousePosX, MousePosY, RightX, TopY) < 50 && LClick && Timer1 > 0)     {Timer2 = 10; delay_burn = true;} //Collision made    
 //Corner3 Collision Check
 if(Calc_Distance(MousePosX, MousePosY, RightX, BottomY) < 50 && LClick && Timer2 > 0)  {Timer3 = 10; delay_burn = true;} //Collision made    
 
 if((Timer1 > 0 && Timer2 > 0 && Timer3 > 0) || purge_timer < 0) //Purge Check.
 {
  Timer1 = 0; Timer2 = 0; Timer3 = 0;
  if(!Keep_msg) 
   {
    for(int i = 0; i < NPOS; i++)
     {
       xposes[i] = OFFSET;
       yposes[i] = OFFSET;
     } 
   }
 }    
 if(hge->Input_GetKeyState(HGEK_ESCAPE)) return true;
 MouseArray->update(delta_time);
 return false;
}

bool RenderFunc()
{
 hge->Gfx_BeginScene();
 hge->Gfx_Clear(0xFF000000);
 
 //Background Work
 #ifdef madworld
 Spr_BrushedMetal->Render(0, 0);
 if(LClick) Fade_down(Spr_Blank, 1);
 else       Fade_up  (Spr_Blank, 1, 230);
 if(!delay_burn) RenderBurn(Spr_Scorch, MouseArray, LClick);
 Spr_Blank->Render(0, 0);
 for(int i = 0; i < NPOS; i++) 
  {
   Par_LightParticles[i]->Render();
   Par_MeltParticles[i] ->Render();
  }
 Par_GlowParticles->Render();
 Par_WeldParticles->Render();
 if(Keep_msg) {Spr_Scorch->RenderEx(LeftX - 50, BottomY - 50, 0, 3.0);}
 #endif

 #ifdef tesla
  Spr_Tesla1->Render(100, Screen_Height);
  Spr_Tesla2->Render(Screen_Width - 100, Screen_Height); 
  RenderBolts();
 #endif

 //Debugging text checks 
 if(false)
 {
  for(int i = 0; i < 7; i++)
   {
    //Ariel_Font->printf(5, 5+ 20*i, HGETEXT_LEFT, "%.3f %.3f %.3f %i", MouseArray->Pos_Array[i].x_pos, MouseArray->Pos_Array[i].y_pos, MouseArray->Pos_Array[i].lifetime, MouseArray->Pos_Array[i].in_use);
    Ariel_Font->printf(5, 5+ 20*i, HGETEXT_LEFT, "%.3f", Triangle[i].lifetime);
   }
  Ariel_Font->printf(1024,  5, HGETEXT_RIGHT, "%.3f %.3f", MousePosX + 20, MousePosY + 20);
  //Ariel_Font->printf(1024, 25, HGETEXT_RIGHT, "Queue Head: %i, Queue Top: %i", MouseArray->top_queue, MouseArray->cur_position);
 }
 hge->Gfx_EndScene();
 return false;
}

    bool disable_dmx = false;

void turnLightsOn()
{
    if(disable_dmx) return;
	unsigned char dmx[513];
	dmx[512] = 0;
	for(int i = 0; i < 512; i++)
	{
		dmx[i] = 250; 
	}
	HardDllCommand(DHC_DMXOUT, 512, dmx);
}
void turnLightsOff()
{
    if(disable_dmx) return;
	unsigned char dmx[513];
	dmx[512] = 0;
	for(int i = 0; i < 512; i++)
	{
		dmx[i] = 0; 
	}
	HardDllCommand(DHC_DMXOUT, 512, dmx);
}
void turnLightsRed()
{
    if(disable_dmx) return;
	unsigned char dmx[513];
	dmx[512] = 0;
	for(int i = 0; i < 512; i++)
	{
		dmx[i] = 0;
	}
	dmx[0] = 127;
	dmx[9] = 255;
	HardDllCommand(DHC_DMXOUT, 512, dmx);
}
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
  //Startup
  Load_HGEINI();
  hge->System_SetState(HGE_FRAMEFUNC,  FrameFunc);
  hge->System_SetState(HGE_RENDERFUNC, RenderFunc);
  if(hge->System_Initiate())
  {
    for (int i = 0; i < numbolts; i++)
 {  bolt[0][i].height = 0;
    bolt[1][i].height = .64418;
    bolt[2][i].height = .771149f;
    bolt[3][i].height = .794658f;
}

    //Load data
    //Waypoints for each location on the triangle
    Triangle[0].x = 520; Triangle[0].y = 200; Triangle[0].radius = 50; Triangle[0].lifetime = 0;
    Triangle[1].x = 400; Triangle[1].y = 400; Triangle[1].radius = 50; Triangle[1].lifetime = 0;
    Triangle[2].x = 300; Triangle[2].y = 560; Triangle[2].radius = 50; Triangle[2].lifetime = 0;
    Triangle[3].x = 520; Triangle[3].y = 560; Triangle[3].radius = 50; Triangle[3].lifetime = 0;
    Triangle[4].x = 770; Triangle[4].y = 560; Triangle[4].radius = 50; Triangle[4].lifetime = 0;
    Triangle[5].x = 660; Triangle[5].y = 400; Triangle[5].radius = 50; Triangle[5].lifetime = 0;
    Triangle[6].x = 520; Triangle[6].y = 200; Triangle[6].radius = 50; Triangle[6].lifetime = 0;

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
      Par_MeltParticles[i]  = new hgeParticleSystem("gfx\\melt1.psi", Spr_MeltParticles);
      Par_LightParticles[i] = new hgeParticleSystem("gfx\\glow.psi" , Spr_GlowParticles);
      Par_LightParticles[i]->info.fLifetime = Par_MeltParticles[i]->info.fLifetime;
      Par_LightParticles[i]->info.nEmission = 3;
      Par_LightParticles[i]->info.fSizeStart = 5;
      Par_LightParticles[i]->info.fSizeEnd   = 3;
     }
    
    
    Load_Fonts(Ariel_Font, "fonts\\arial.fnt", 0xFFFFFFFF);
    Load_Graphic(Tex_Tesla1, "gfx\\Tesla1.png", Spr_Tesla1, 296, 570);
    Load_Graphic(Tex_Tesla2, "gfx\\Tesla2.png", Spr_Tesla2, 296, 570);
    Load_Graphic(Tex_BrushedMetal, "gfx\\Brushed Metal.png", Spr_BrushedMetal, 1024, 768);
    Load_Graphic(Tex_Scorch,       "gfx\\scorch.png", Spr_Scorch, 32, 32);    
    Spr_Tesla1->SetHotSpot(0, 570);
    Spr_Tesla2->SetHotSpot(296, 570);
    Load_Graphic(Tex_Blank, "" , Spr_Blank,  1024, 768);
    Spr_Blank->SetColor(0x00000000);
    
    //Light control initialization
    /**/
  	HardDllOpen();
    if(HardDllCommand(DHC_INIT, NULL, NULL) < 0)
	{
		//cout << "Fuck.  Cannot init.";
		disable_dmx = true;
	}
	if(disable_dmx || HardDllCommand(DHC_OPEN, NULL, NULL) != DHE_OK)
	{
		//cout << "Fuck.  Cannot open.";
		disable_dmx;
	}
	
    /* */
    hge->System_Start();
   }
  //Shutdown
  hge->System_Shutdown();
  hge->Release();
  /**/
	if(disable_dmx || HardDllCommand(DHC_CLOSE, NULL, NULL) != DHE_OK)
	{
 disable_dmx = true;
		//cout << "Fuck, cannot close.";
	}
	if(!disable_dmx)
	{
	HardDllCommand(DHC_EXIT, NULL, NULL);
	HardDllClose();
}
	/* */
  return 0;
}
