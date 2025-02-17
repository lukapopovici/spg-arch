#pragma once
// #define WIN32 // doar in Windows
#include "FL/Enumerations.H"
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/fl_draw.H>
#include <cmath>
#include <iostream>

#define PI 3.141592f

class MyWidget1 : public Fl_Window {
  int xc, yc;       // centrul ferestrei
  float x[4], y[4]; // coordonatele varfurilor unui patrat
  int N;            // nr de patrate

public:
  MyWidget1() : Fl_Window(200, 200, 512, 512, "SPG Lab1") {
    xc = this->w() / 2;
    yc = this->h() / 2;
    N = 10;
    init_obiect(100, 40);
  }

  void desen() {
    // deseneaza un patrat cu varfurile (x[0] y[0]), (x[1] y[1]), etc.
    for (int i = 0; i < 3; i++) {
      fl_line(x[i], y[i], x[i + 1], y[i + 1]);
    }
    fl_line(x[3], y[3], x[0], y[0]);
  }

  void init_obiect(int raza, int latura) {
    // functie ce calculeaza coordonatele varfurilor in pozitia initiala
    x[0] = raza - latura / 2.0 + xc;
    y[0] = -latura / 2.0 + xc;

    x[1] = raza + latura / 2.0 + xc;
    y[1] = -latura / 2.0 + xc;

    x[2] = raza + latura / 2.0 + xc;
    y[2] = latura / 2.0 + xc;

    x[3] = raza - latura / 2.0 + xc;
    y[3] = latura / 2.0 + xc;
  }

  void draw() override {
    Fl_Window::draw(); // apeleaza metoda din clasa de baza pentru a desena
                       // background-ul

    float du = 2 * PI / N; // unghiul dintre doua patrate succesive
    float c = cos(du);
    float s = sin(du);
    fl_color(FL_BLUE);
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < 4; ++j) {
        float xcp = x[j];
        float ycp = y[j];

        x[j] = (xcp - xc) * c - (ycp - yc) * s + xc;
        y[j] = (xcp - xc) * s + (ycp - yc) * c + yc;
      }
      desen();
    }

    fl_color(FL_RED);
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < 4; ++j) {
        float xcp = x[j];
        float ycp = y[j];

        x[j] = (xcp - xc) * c - (ycp - yc) * s + xc;
        y[j] = (xcp - xc) * s + (ycp - yc) * c + yc;

        xcp = x[j];
        ycp = y[j];

        float x_square_center = cos((2 * PI / N) * (i + 1)) * 100 + xc;
        float y_square_center = sin((2 * PI / N) * (i + 1)) * 100 + yc;

        x[j] = (xcp - x_square_center) * cos(-du) -
               (ycp - y_square_center) * sin(-du) + x_square_center;
        y[j] = (xcp - x_square_center) * sin(-du) +
               (ycp - y_square_center) * cos(-du) + y_square_center;
      }
      desen();
    }
  }
};
