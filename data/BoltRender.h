#define numbolts 5
Lightning2 bolt[numbolts];


void RenderBolts()
{
     for (int n = 0; n < numbolts; n++)
     {
         int res = 16;
         int x1, y1, x2, y2;
         vector<Point> rendered_bolt(res * (bolt[n].length-1) + 1);
         for (int i = 0; i < bolt[n].length; i++)
         {
             float angle = rand()%360*(float)M_PI/180;
             float mag = 0.4f * (4 * i * (bolt[n].length-1) - i * i * 4);
             //Calc_Distance(x, y, (a.x+b.x)/2, (a.y+b.y)/2)
             //float mag = 20 * cos((float) M_PI * ((float) i / (bolt[n].length-1) - 0.5f));
             rendered_bolt[res * i].x = bolt[n].point[i].x + mag * cos(angle);
             rendered_bolt[res * i].y = bolt[n].point[i].y + mag * sin(angle);
             if (i == 0)
                 continue;
             // Interpolate between rendered_bolt[res*(i-1)] and rendered_bolt[res*i]
             for (int a = res*(i-1), b = res*i, j = a+1; j < b; j++)
             {
                 float angle2 = rand()%360*(float)M_PI/180;
                 float mag2 = 0.03f * (4 * (j-a-1) * (b-a-2) - (j-a-1)*(j-a-1) * 4);

                 rendered_bolt[j].x = rendered_bolt[a].x + (rendered_bolt[b].x - rendered_bolt[a].x) * (float) (j-a) / (b-a) + mag2 * cos(angle2);
                 rendered_bolt[j].y = rendered_bolt[a].y + (rendered_bolt[b].y - rendered_bolt[a].y) * (float) (j-a) / (b-a) + mag2 * sin(angle2);
             }
         }
         for (int i = 0; i < res * (bolt[n].length-1); i++)
         {
             hge->Gfx_RenderLine(rendered_bolt[i].x, rendered_bolt[i].y,
                                 rendered_bolt[i+1].x, rendered_bolt[i+1].y, 0xFFEEEEFF - 0x33333300 * n);
         }
         bolt[n].update();
     }
}
