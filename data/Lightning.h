#define bolt_length 256
#define height_step .01


class Lightning
{
 public:
  Point* point;
  int length;
  
  Lightning(Telsa_Coil coil);
  ~Lightning();
  void update();
  
 private:
  float height;
  // sticky to left or right rod
  Telsa_Coil sticky;
};

Lightning::Lightning(Telsa_Coil coil) : length(bolt_length), height(0), sticky(coil)
{
  point = new Point[bolt_length];
  // Stick the first point to the coil
  point[0].x = sticky.pos;
  // Stick the last point to the other coil
  point[length-1].x = 1 - sticky.pos;
  // Start at height = 0
  point[0].y = point[length-1].y = 0;
  update();
}

Lightning::~Lightning()
{
  delete[] point;
}

void Lightning::update()
{
  // Move the rod up
  point[0].y += height; // Left rod
  
  // Case touched
  
  // Case untouched
  point[length-1].y += height;
  
  // Update all the points in between
  for (int i = 1; i < length-1; i++)
  {
      point[i].x = point[0].x + (point[length-1].x - point[0].x) * i / (length-1);
      point[i].y = point[0].y + (point[length-1].y - point[0].y) * i / (length-1);
  }

  // Move linearly to .75, then deaccelerate 
  if (height <= .75 )
     height += height_step;
  else
     height += (1 - height) * height_step / (1 - 0.75f);
}
