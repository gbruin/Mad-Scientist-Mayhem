#ifndef HGE_INIT_H
#define HGE_INIT_H
//-----------------------------------------------------------------------------------------------------------------------
int Screen_Width = 1024;
int Screen_Height = 768;
float Screen_Ratio = Screen_Width/Screen_Height;

void Load_HGEINI()
{
  hge = hgeCreate(HGE_VERSION);
  hge->System_SetState(HGE_LOGFILE, "LogFile.txt");
  hge->System_SetState(HGE_TITLE, "Project MadHouse - Alpha");
  hge->System_SetState(HGE_WINDOWED, false);
  hge->System_SetState(HGE_SCREENWIDTH, Screen_Width);
  hge->System_SetState(HGE_SCREENHEIGHT, Screen_Height);
  hge->System_SetState(HGE_SCREENBPP, 32);
  hge->System_SetState(HGE_SHOWSPLASH, false);
  hge->System_SetState(HGE_HIDEMOUSE, true);
  hge->System_SetState(HGE_FPS, 60);
}

//RESOURCE FUNCTIONS-----------------------------------------------------------------------------------------------------
bool Load_Graphic(HTEXTURE &Graphic, char* File, hgeSprite *&Sprite, int Width, int Height, int X_Start = 0, int Y_Start = 0, int Hot_X = 0, int Hot_Y = 0, int DWORD = 0xFFFFFFFF, bool preloaded = false)
{
  if(!preloaded) Graphic = hge->Texture_Load(File); //Makes sure that the texture does not "reload" if it is already loaded in memory. Can be a big optimization if sprite sheets are being used.
  if(!&Graphic || X_Start < 0 || Y_Start < 0 || Width < 0 || Height < 0) return false; //Error Checking
  Sprite  = new hgeSprite(Graphic, X_Start, Y_Start, Width, Height);  
  Sprite->SetHotSpot(Hot_X, Hot_Y);
  Sprite->SetColor(DWORD);
  return true;
}

bool Load_Animation(HTEXTURE &Graphic, char* File, hgeAnimation *&Animation, int Frames, int FPS, float X_Start, int Y_Start, int Width, int Height, int Hot_X = 0, int Hot_Y = 0, int DWORD = 0xFFFFFFFF, bool preloaded = false)
{
 if(!preloaded) Graphic = hge->Texture_Load(File);
 if(!&Graphic || X_Start < 0 || Y_Start < 0 || Width < 0 || Height < 0) return false; //Error Checking
 Animation = new hgeAnimation(Graphic, Frames, FPS, X_Start, Y_Start, Width, Height);
 Animation->SetHotSpot(Hot_X, Hot_Y);
 Animation->SetColor(DWORD);
 return true;
}

bool Load_Fonts(hgeFont*& Font, char* File, int DWORD, float Scale = 1)
{
  Font = new hgeFont(File);
  Font->SetScale(Scale);
  Font->SetColor(DWORD);
}
#endif
