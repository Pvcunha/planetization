#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <raylib.h>
#include <string.h>
#include <math.h>

#define MIN_WIDTH 800
#define MIN_HEIGHT 800

// TODO: min max para raio dos planetas
// TODO: fator de aceleracao do tempo

const double G = 6.67e-11;
const double AU = (149.6e6 * 1000); //149.6 million km, in meters.
const double SCALE = 75/ AU;

const int timestep = 24*3600;

int width = 800;
int height = 800;

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
  if(!IsWindowFullscreen()) {
    int monitor = GetCurrentMonitor();
    width = GetMonitorWidth(monitor);
    height = GetMonitorHeight(monitor);
    SetWindowSize(GetMonitorWidth(monitor), GetMonitorHeight(monitor));
    ToggleFullscreen();
  } 
  else {
    ToggleFullscreen();
    SetWindowSize(w, h);
    width = w;
    height = h;
  }
}

Body *setupPlanets(int size) {
  
  // factsheet https://nssdc.gsfc.nasa.gov/planetary/factsheet/

  Body sun = makeBody(0.0, 0.0, 18.0, 1.98892 * pow(10, 30), YELLOW);

  Body earth = makeBody(-1*AU, 0.0, 9.0,   5.9742*pow(10, 24), BLUE);
  earth.velocity.y = 29.783 * 1000; //km/s

  Body venus = makeBody(0.723*AU, 0.0, 5.0, 4.865 * pow(10, 24), PURPLE); 
  venus.velocity.y = -35.02 * 1000;

  Body mercury = makeBody(-0.388*AU, 0.0, 3.5, 0.33*pow(10,24), GREEN);
  mercury.velocity.y = 47.4*1000;

  Body mars = makeBody(1.53*AU, 0.0, 5, 0.642*pow(10, 24), BROWN);
  mars.velocity.y = -24.1 * 1000;

  Body jupiter = makeBody(5.22*AU, 0.0, 12, 1898*pow(10,24), GRAY);
  jupiter.velocity.y = 13.1 * 1000;

  Body saturn = makeBody(-9.61*AU, 0.0, 10, 568*pow(10,24), RED);
  saturn.velocity.y = 9.7 * 1000;
  
  Body uranus = makeBody(19.1*AU, 0.0, 8, 86.8*pow(10,24), PINK);
  uranus.velocity.y = 6.8 * 1000;

  Body neptune = makeBody(-30.3*AU, 0.0, 8, 102*pow(10, 24), DARKPURPLE);
  neptune.velocity.y = -5.4 * 1000;

  Body *bodies = (Body *)malloc(size*sizeof(Body));

  bodies[0] = sun;
  bodies[1] = earth;
  bodies[2] = venus;
  bodies[3] = mercury;
  bodies[4] = mars;
  bodies[5] = jupiter;
  bodies[6] = saturn;
  bodies[7] = uranus;
  bodies[8] = neptune;

  return bodies;
}

int main () {
  


  InitWindow(width, height, "my window");
  SetWindowState(FLAG_WINDOW_RESIZABLE);

  

  SetTargetFPS(60);
  int size = 9;
  Body *bodies = setupPlanets(size);

  while(!WindowShouldClose()) {

    updateBodies(bodies, size);  
    BeginDrawing();
    ClearBackground(BLACK);
    
    for(int i = 0; i < size; i++) {
      drawBody(bodies[i]);
    }
    EndDrawing();

    if(GetKeyPressed() == KEY_F) {
      togglefullscreen(MIN_WIDTH, MIN_HEIGHT);
    }
  }

  free(bodies);

  return 0;
}
