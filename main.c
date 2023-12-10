#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <raylib.h>
#include <string.h>
#include <math.h>

const double G = 6.67e-11;
const double AU = (149.6e6 * 1000); //149.6 million km, in meters.
const double SCALE = 250 / AU;

int width = 1280;
int height = 680;

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

void updateBodies(Body *bodies) {

}

int main () {
  

  int timestep = 24*3600;

  InitWindow(width, height, "my window");
  SetWindowState(FLAG_WINDOW_RESIZABLE);

  Body sun = makeBody(0.0, 0.0, 30.0, 1.98892 * pow(10, 30), YELLOW);
  Body b = makeBody(0.0, 0.0, 10.0,   5.9742*pow(10, 24), BLUE);
  
  b.pos.x = -1*AU;
  b.velocity.y = 29.783 * 1000; //km/s

  SetTargetFPS(60);

  int display = GetCurrentMonitor();

  while(!WindowShouldClose()) {

    double dx = (sun.pos.x - b.pos.x);
    double dy = (sun.pos.y - b.pos.y);
    double d = norm(sun.pos, b.pos);

    double gf = gForce(sun, b);

    double gfx = gf*(dx/d);
    double gfy = gf*(dy/d);

    b.velocity.x += gfx/b.mass * timestep ;
    b.velocity.y += gfy/b.mass * timestep ;
    
    b.pos.x += b.velocity.x * timestep;
    b.pos.y += b.velocity.y * timestep;

    // sleep(1);
    // printf("gf=%lf gfx=%lf gfy=%lf dx=%lf dy=%lf bvx=%f bvy=%f\n", gf, gfx, gfy, dx, dy, b.velocity.x, b.velocity.y);

    BeginDrawing();
    ClearBackground(BLACK);
    
    drawBody(sun);
    drawBody(b);
    EndDrawing();

    if(GetKeyPressed() == KEY_F) {
      printf("width=%d height=%d\n", GetScreenWidth(), GetScreenHeight());

      if(!IsWindowFullscreen()) {
        SetWindowSize(1920, 1080);
      }
      ToggleFullscreen();
    }
    
  }

  return 0;
}
