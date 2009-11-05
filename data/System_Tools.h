#ifndef SYSTEM_TOOLS_H
#define SYSTEM_TOOLS_H

//Mouse Positioning
float MousePosX = 0, *MousePtrX = &MousePosX, MousePosY = 0, *MousePtrY = &MousePosY; float MousePrevX; float MousePrevY;

void Collect_Time_Data(float& Delta_Time, float& Elapsed_Time, int& Current_FPS)
{
 Delta_Time    = hge->Timer_GetDelta();
 Elapsed_Time  = hge->Timer_GetTime();
 Current_FPS   = hge->Timer_GetFPS();
}

void Report_Status(hgeFont* Font, float Pos_X, float Pos_Y, float Delta_Time, int Current_FPS, float Elapsed_Time, float Scale = 1.0, DWORD Color = 0xFFFFFFFF)
{
 float Cur_Scale = Font->GetScale();
 DWORD Cur_Color = Font->GetColor();
 Font->SetScale(Scale);
 Font->SetColor(Color);
 Font->printf(Pos_X, Pos_Y     , HGETEXT_LEFT, "Delta Time  : %.3f", Delta_Time);
 Font->printf(Pos_X, Pos_Y + 30, HGETEXT_LEFT, "Current FPS : %i", Current_FPS);
 Font->printf(Pos_X, Pos_Y + 60, HGETEXT_LEFT, "Elapsed Time: %.3f", Elapsed_Time);        
 Font->SetScale(Cur_Scale);
 Font->SetColor(Cur_Color);
}

bool Pause_Function(float& Time_Left, float Delta_Time)
{
 if(Time_Left > 0) Time_Left = Time_Left - Delta_Time;
 if(Time_Left > 0) return false;
 else return true;
}
#endif
