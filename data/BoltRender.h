#define numbolts 5

Lightning2 bolt[4][numbolts];

void RenderBolts()
{
     for (int b = 0; b < 3; b++)
         for (int n = 0; n < numbolts; n++)
     {
         int res = 16;
         int x1, y1, x2, y2;
         vector<Point> rendered_bolt(res * (bolt[b][n].length-1) + 1);
         for (int i = 0; i < bolt[b][n].length; i++)
         {
             float angle = rand()%360*(float)M_PI/180;
             float mag = 0.4f * (4 * i * (bolt[b][n].length-1) - i * i * 4);
             rendered_bolt[res * i].x = bolt[b][n].point[i].x + mag * cos(angle);
             rendered_bolt[res * i].y = bolt[b][n].point[i].y + mag * sin(angle);
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
         for (int i = 0; i < res * (bolt[b][n].length-1); i++)
             hge->Gfx_RenderLine(rendered_bolt[i].x, rendered_bolt[i].y,
                                 rendered_bolt[i+1].x, rendered_bolt[i+1].y, 0xFFEEEEFF - 0x33333300 * n, 1.0f);
         bolt[b][n].update();
     }
         }
}
