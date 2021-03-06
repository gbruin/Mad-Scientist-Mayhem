#define bolt_length 256
#define height_step .0035

class Lightning
{
 public:
  vector<Point> point;
  int length;
  
  Lightning(/*Telsa_Coil coil*/);
  ~Lightning();
  void update();
  bool isdead();
  
 private:
  float height;
  // sticky to left or right rod
  Telsa_Coil sticky;
  void calculate(int a, int b, int depth);
};

Lightning::Lightning(/*Telsa_Coil coil*/) : length(bolt_length), height(0)//, sticky(coil)
{
  vector<Point> new_points(bolt_length);
  point = new_points;
  // Stick the first point to the coil
  point[0].x = 0.2f * 1024;
  //point[0].x = sticky.pos;
  
  // Stick the last point to the other coil
  point[length-1].x = 0.8f * 1024;
  //point[length-1].x = 1 - sticky.pos;
  
  // Start at height = 0
  point[0].y = 0;
  point[length-1].y = 0;
  
  // Fill in the middle points
  for (int i = 1; i < length-1; i++)
  {
      point[i].y = point[0].y + (point[length-1].y - point[0].y) * (float) i / (0-length+1);
      point[i].x = point[0].x + (point[length-1].x - point[0].x) * (float) i / (length-1);
  }
}

Lightning::~Lightning()
{
}

void Lightning::update()
{
     if (height > .999)
        height=0;

     bool sticky = true;
     // Case: touched
     if (sticky)
     {
         // Stick the bolt to the mouse
         float mx;
         float my;
         hge->Input_GetMousePos(&mx, &my);
         point[0].x = mx;
         point[0].y = my;
     }
     else
     {
         point[0].x = 0.2f * 1024;
         point[0].y = 768 * (1-height);
     }
     // Case: untouched, move the other end of the bolt up
     point[length-1].x = 0.8f * 1024;
     point[length-1].y = 768 * (1-height);
     
     // Update all the points in between
     for (int i = 1; i < length-1; i++)
         point[i].x = point[0].x + (point[length-1].x - point[0].x) * i / (length-1);
     // Calculate y values recursiveley
     calculate(0, length-1, 8);


  // Move linearly to .75, then deaccelerate 
  if (height <= .75 )
     height += height_step;
  else
     height += (1 - height) * height_step / (1 - 0.75f);
  //height = .5;
}

void Lightning::calculate(int a, int b, int depth)
{
    if (depth == 0)
    {
        /**/
        for (int i = a+1; i <= b-1; i++)
        {
            float offsety = 80.0f * cos(3.14159f * ((float) i / length - 0.5f));
            //point[i].y = (point[a].y + point[b].y ) / 2 + offset - 40.0f * cos(3.14159f * (i / length - 0.5f));
            point[i].y = point[b].y + (point[b].y - point[a].y) * (float) i / (b-a);// - offsety;
            //point[i].y = 768 * (1-height) - offsety;
        }
        /**/
    }
    else
    {
        int mid = (a+b)/2;
        int offset = 0;
        if (depth > 1)
            offset = 5 * (rand()%depth - depth/2);
        point[mid].y = (point[a].y + point[b].y ) / 2 + offset - 80.0f * cos(3.14159f * (mid / length - 0.5f));
        calculate(a, mid, depth-1);
        calculate(mid, b, depth-1);
    }
}

