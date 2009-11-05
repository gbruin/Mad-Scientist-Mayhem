#ifndef BUTTONS_H
#define BUTTONS_H

struct Button
{
 hgeRect* Box_Size;
 hgeSprite* On_Sprite;
 hgeSprite* Off_Sprite;
 hgeFont* Button_Text; //Future feature to add text support in buttons
};

//Creates a button the same size as the size of the sprite. 
void CreateButton(Button& AButton, hgeSprite* On_spr, hgeSprite* Off_spr, float Pos_X, float Pos_Y, float Scale, float HScale = -1)
{
 AButton.On_Sprite  = On_spr;
 AButton.Off_Sprite = Off_spr;
 float Width;
 if(HScale == -1) Width = AButton.On_Sprite->GetWidth()*Scale;
 else             Width = AButton.On_Sprite->GetWidth()*HScale;
 float Height = AButton.On_Sprite->GetHeight()*Scale; 
 AButton.Box_Size = new hgeRect(Pos_X, Pos_Y, Pos_X + Width, Pos_Y + Height);
}

bool Check_Press(Button AButton, float MouseX, float MouseY, bool Click)
{
 if(AButton.Box_Size->TestPoint(MouseX, MouseY) && Click) return true;
 else                                                     return false;
}

//Renders the button depending on the position of the mouse, either On, Off, or off with color overlay.
bool RenderButton(Button AButton, float MouseX, float MouseY, bool Click, DWORD MouseOverColor = 0x00000000, float Scale = 1.0, float HScale = -1)
{
 if(HScale == -1) HScale = Scale;
 
 if(AButton.Box_Size->TestPoint(MouseX, MouseY)) //The mouse is on top of the button
  {
   //Button off sprite
   if(Click)
    {
     AButton.Off_Sprite->RenderEx(AButton.Box_Size->x1, AButton.Box_Size->y1, 0, Scale, HScale);
     return true;      
    }                                      
   //Button on sprite w/ color overlay
   else
    {
     int Cur_Color = AButton.On_Sprite->GetColor();
     AButton.On_Sprite->SetColor(MouseOverColor);
     AButton.On_Sprite->RenderEx(AButton.Box_Size->x1, AButton.Box_Size->y1, 0, Scale, HScale);
     AButton.On_Sprite->SetColor(Cur_Color);
     return false;                         
    }
  }
 //Button on sprite
 else
  {
   AButton.On_Sprite->RenderEx(AButton.Box_Size->x1, AButton.Box_Size->y1, 0, Scale, HScale);            
   return false;                        
  }
}

#endif
