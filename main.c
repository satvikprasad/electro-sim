#include <math.h>
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static inline double sign(double v);
static inline double principal_val(double v);

double l, g, m, b, d, R, tao, theta, theta_dot;
double *y;

int main(int argc, char **argv) {
  if(argc != 5) {
    printf("Format: main $l$ $theta$ $thetadot$ $B$\n");

    l = 0.2;
    b = 5;
    theta = (30*PI)/180.f;
    theta_dot = 0;
  } else {
    l = atof(argv[1]);
    theta = (atof(argv[2])*PI)/180.f;
    theta_dot = (atof(argv[3])*PI)/180.f;
    b = atof(argv[4]);
  }

  g = 9.8;
  m = l*4;
  d = 0.2;
  R = 5;
  tao = (b*b*d*d)/(R*m);

  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(1280, 720, "Pendulum in B-field");
  SetTargetFPS(144);

  y = calloc(GetScreenWidth(), sizeof(double));
  for (size_t i = 0; i < GetScreenWidth(); ++i) {
    y[i] = -1;
  }

  while (!WindowShouldClose()) {
    double w = GetScreenWidth();
    double h = GetScreenHeight();
    if (IsWindowResized()) {
      y = realloc(y, sizeof(double) * w);
    }

    double dt = GetFrameTime()/2;


    double theta2dot = (-g / l) * sinf(theta) - (tao)*theta_dot*cosf(theta)*cosf(theta);

    theta_dot += theta2dot*dt;
    theta += theta_dot*dt;

    if (theta > PI) {
      theta = theta - 2*PI;
    }

    if (theta < -PI) {
      theta = theta + 2*PI;
    }

    Vector2 origin = {w/2, 100};

    Vector2 end = {l * sinf(theta), l * cosf(theta)};
    end.x *= 1000.f;
    end.y *= 1000.f;
    end.x += origin.x;
    end.y += origin.y;

    char vel[50];
    char angle[50];
    char current[50];
    char k[50];
    char b_vec[50];

    snprintf(vel, 50, "Angular velocity: %2.1frad s^-1", theta_dot);
    snprintf(angle, 50, "Angle: %2.1frad", theta);
    snprintf(current, 50, "Induced current: %2.1fA", -(b*l*w*theta_dot*cosf(theta))/R);
    snprintf(k, 50, "Kinetic energy of system: %2.1fJ", m*l*theta_dot/2);
    snprintf(b_vec, 50, "B-field vector magnitude: %2.1fT", b);

    memmove(y + 1, y, sizeof(double)*(w - 1));
    y[0] = theta/PI;

    BeginDrawing();

    ClearBackground(RAYWHITE);

    DrawLineEx(origin, end, 10, BLACK);

    double width;
    int n;
    if (b == 0) { 
      n = 0;
    } else {
      width = 200/b;
      n = (w/(2*width));
    }

    int half = n/2;

    for (int i = 0; i < n; ++i) {
      Vector2 mid = (Vector2){(i-half)*width + w/2, h/2};

      DrawLineV((Vector2){(i-half)*width + w/2, 0}, (Vector2){(i-half)*width + w/2, h}, (Color){0, 0, 100, 50});
      DrawTriangleLines(mid, (Vector2){(i-half)*width + w/2 - width/2 + 2, h/2 + width}, (Vector2){(i-half)*width + w/2 + width/2 - 2, h/2 + width}, (Color){0, 0, 100, 50});
    }

    for (int i = 0; i < w; ++i) {
      double t = y[i];
      DrawPixel(i, h - t*100 - 100, BLACK);
    }

    DrawLine(0, h - 200, w, h - 200, (Color){0, 0, 50, 100});
    DrawLine(0, h - 100, w, h - 100, (Color){0, 0, 50, 100});
    DrawLine(0, h, w, h, (Color){0, 0, 50, 100});

    DrawText(b_vec, 3*w/4, h-20, 20, (Color){0, 0, 100, 50});

    DrawCircleV(end, 10, RED);
    DrawText("I", end.x, end.y-5, 15, BLACK);
    DrawText(vel, 0, 0, 20, BLACK);
    DrawText(angle, 0, 20, 20, BLACK);
    DrawText(current, 0, 20*2, 20, BLACK);
    DrawText(k, 0, 20*3, 20, BLACK);

    EndDrawing();
  }

  printf("bean\n");

  CloseWindow();

  return 0;
}

static inline double sign(double v) {
  if (fabs(v) == 0) {
    return 1.f;
  } 

  return v/fabs(v);
}
