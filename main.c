#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <raylib.h>
#include <string.h>
#include <math.h>

const double G = 6.67e-11;
const double AU = (149.6e6 * 1000); //149.6 million km, in meters.
const double SCALE = 250 / AU;

const int timestep = 24*3600;

int width = 800;
int height = 400;

typedef struct Body {
  Vector2 pos;
  float radius;
  double mass;
  Color color;
  Vector2 velocity;
} Body;

void drawBody(Body b) {
  float x, y;
  x = b.pos.x * SCALE + (float)width/2;
  y = b.pos.y * SCALE + (float)height/2;
  
  DrawCircle(x, y, b.radius, b.color);
}

double norm(Vector2 a, Vector2 b) {
  return sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
}

double gForce(Body a, Body b) {
  double d = norm(a.pos, b.pos);
  return (G*(a.mass*b.mass))/(d*d);
}

Body makeBody(float posx, float posy, float radius, double mass, Color color) {
  Body b;
  b.pos.x = posx;
  b.pos.y = posy;
  b.radius = radius;
  b.mass = mass;
  b.color = color;
  b.velocity.x = 0.0;
  b.velocity.y = 0.0;
  return b;
}

void updateBodies(Body *bodies, int bodiesSize) {

  for(int i = 0; i < bodiesSize; i++)
  {
    double accGfx = 0;
    double accGfy = 0;
    for(int j = 0; j < bodiesSize; j++)
    {
      if(j == i)
        continue;

      double dx = bodies[j].pos.x - bodies[i].pos.x;
      double dy = bodies[j].pos.y - bodies[i].pos.y;
      double d = norm(bodies[i].pos, bodies[j].pos);
      
      double gf = gForce(bodies[i], bodies[j]);
      double gfx = gf*(dx/d);
      double gfy = gf*(dy/d);
      
      accGfx += gfx;
      accGfy += gfy;
    }

    bodies[i].velocity.x += accGfx/bodies[i].mass * timestep;
    bodies[i].velocity.y += accGfy/bodies[i].mass * timestep;

    bodies[i].pos.x += bodies[i].velocity.x * timestep;
    bodies[i].pos.y += bodies[i].velocity.y * timestep;
  }

}

void togglefullscreen(int w, int h) {
  SetWindowState(FLAG_WINDOW_UNDECORATED);  
  if(!IsWindowFullscreen()) {
    int monitor = GetCurrentMonitor();
    SetWindowSize(GetMonitorWidth(monitor), GetMonitorHeight(monitor));
    ToggleFullscreen();
  } 
  // else {
  //   // ToggleFullscreen();
  //   SetWindowSize(w, h);
  // }
}

int main () {
  


  InitWindow(width, height, "my window");
  // SetWindowState(FLAG_WINDOW_RESIZABLE);

  Body sun = makeBody(0.0, 0.0, 30.0, 1.98892 * pow(10, 30), YELLOW);

  Body earth = makeBody(-1*AU, 0.0, 10.0,   5.9742*pow(10, 24), BLUE);
  earth.velocity.y = 29.783 * 1000; //km/s

  Body venus = makeBody(0.723*AU, 0.0, 5.0, 4.865 * pow(10, 24), PURPLE); 
  venus.velocity.y = -35.02 * 1000;

  SetTargetFPS(60);
  
  int size = 3;
  Body *bodies = (Body *)malloc(size*sizeof(Body));
  bodies[0] = sun;
  bodies[1] = earth;
  bodies[2] = venus;

  while(!WindowShouldClose()) {

    updateBodies(bodies, size);  
    BeginDrawing();
    ClearBackground(BLACK);
    
    for(int i = 0; i < size; i++) {
      drawBody(bodies[i]);
      // printf("%d = (%f %f)\n", i, bodies[i].pos.x, bodies[i].pos.y);
    }
    // drawBody(sun);
    // drawBody(b);
    EndDrawing();

    if(GetKeyPressed() == KEY_F) {
      togglefullscreen(width, height);
    }
    
  }

  return 0;
}
