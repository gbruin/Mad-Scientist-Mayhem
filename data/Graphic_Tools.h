#ifndef GRAPHIC_TOOLS_H
#define GRAPHIC_TOOLS_H

void Fade_down(hgeSprite* Fade_Sprite, unsigned int Fade_Down)   //Fades towards Transparency
{
 unsigned int Cur_Color = Fade_Sprite->GetColor();                         //Gets the current Color
 unsigned int Cur_Alpha = GETA(Cur_Color);                                 //Extract the Alpha term in the DWORD
 //Do a check to make sure it doesn't loop over
 if(Cur_Alpha > Fade_Down) Cur_Alpha = Cur_Alpha - Fade_Down;
 else                       Cur_Alpha = 0;
 Cur_Color = SETA(Cur_Color, Cur_Alpha);                                   //Replace the Alpha term with the new Alpha term
 Fade_Sprite->SetColor(Cur_Color);                                         //Set the new color of the Sprite
}

void Fade_up(hgeSprite* Fade_Sprite, unsigned int Fade_Up)       //Fades towards Obsurity
{
 unsigned int Cur_Color = Fade_Sprite->GetColor();                         //Gets the current Color
 unsigned int Cur_Alpha = GETA(Cur_Color);                                 //Extract the Alpha term in the DWORD
 //Do a check to make sure it doesn't loop over
 if(255 - Cur_Alpha > Fade_Up) Cur_Alpha = Cur_Alpha + Fade_Up;
 else                          Cur_Alpha = 255;
 Cur_Color = SETA(Cur_Color, Cur_Alpha);                                   //Replace the Alpha term with the new Alpha term
 Fade_Sprite->SetColor(Cur_Color);                                         //Set the new color of the Sprite
}

void Special_Text(hgeFont* Font, float Pos_X, float Pos_Y, int Letter_Style, char* Text, DWORD To_Color, float To_Scale = 1.0)
{
 DWORD Color = Font->GetColor();
 float Scale = Font->GetScale();
 Font->SetColor(To_Color);
 Font->SetScale(To_Scale);
 Font->printf(Pos_X, Pos_Y, Letter_Style, Text);     
 Font->SetColor(Color);
 Font->SetScale(To_Scale);
}

void Render_Player()
{
     
}

//Renders a field of textures, all the same.
void Render_Texture_Field(hgeSprite* Spr_Field, float Start_X, float Start_Y, float End_X, float End_Y, float CameraX, float CameraY)
{
 float Traverse_X = Spr_Field->GetWidth();
 float Traverse_Y = Spr_Field->GetHeight();
 for(int i = 0; Start_Y + i*Traverse_X < End_Y; i++)
  {
   for(int j = 0; Start_X + j*Traverse_Y < End_X; j++)
    {
     Spr_Field->Render(Start_X + j*Traverse_Y, Start_Y + i*Traverse_X);
// float Player_Pos_Y = Y_Position - Camera_Y + Screen_Height/2;      
    }
  }
}

//Renders a number of selected textures randomly. Used to generate a procedural based ground texture.
void Render_Texture_Array(hgeSprite* Spr_Array[], int Array_Size, float Start_X, float Start_Y, float End_X, float End_Y)
{
 
}

//Returns the angle needed to "face" a position
float FaceRotation(float Object_X, float Object_Y, float Pos_X, float Pos_Y)
 { 
   float Theta;
   float XComp = Object_X - Pos_X;
   float YComp = Object_Y - Pos_Y;
   //This does the final calculation
   Theta = atan2(XComp, YComp);
   return -Theta - M_PI/2;
 }

#endif
