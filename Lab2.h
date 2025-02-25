#pragma once
#include "FL/Enumerations.H"
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/fl_draw.H>
#include <cmath>
#include <iostream>

#define PI 3.141592f

class MyWidget : public Fl_Window {
    float XFm, XFM, YFm, YFM;
    int XPm, XPM, YPm, YPM;
    int tip_tran; // tip_tran == 0 -> scalare neuniforma, tip_tran != 0 ->
                  // scalare uniforma
    float sx, sy, tx, ty;

    int width, height;

    typedef float (*MyFuncPtrType)(float);

  public:
    MyWidget(int width = 512, int height = 512)
        : Fl_Window(200, 200, width, height, "SPG Lab2"), width(width),
          height(height) {}

    void init_grafic() {
        XFm = YFm = XFM = YFM = 0;
        XPm = YPm = XPM = YPM = 0;
        tip_tran = 0;
        sx = sy = tx = ty = 0;
    }
    void calctran() {
        if (XFM > XFm && YFM > YFm) {
            // se determina sx, sy, tx, ty
            sx = (XPM - XPm) / (XFM - XFm);
            sy = (YPM - YPm) / (YFM - YFm);
            tx = XPm - XFm * sx;
            ty = YPm - YFm * sy;
        } else
            sx = sy = tx = ty = 0;
    }
    int XDisp(float xf) {
        // transformarea fereastra-poarta pt coordonata x
        return xf * sx + tx;
    }
    int YDisp(float yf) {
        // transformarea fereastra-poarta pt coordonata y
        return yf * sy + ty;
    }
    void cadru_poarta() {
        fl_line(XPm, YPm, XPm, YPM);
        fl_line(XPm, YPM, XPM, YPM);
        fl_line(XPM, YPM, XPM, YPm);
        fl_line(XPM, YPm, XPm, YPm);
    }
    static float f1(float x) { return sin(x); }

    static float f2(float x) { return tan(x); }
    static float f3(float x) { return sin(4 * x); }

    void grafic(float xmin, float xmax, float pas, MyFuncPtrType f) {
        XFm = xmin;
        XFM = xmax;
        // se determina YFm si YFM ca fiind valoarea minima, respectiv maxima a
        // functiei f in intervalul XFm, XFM
        for (float x = xmin; x < xmax - pas; x += pas) {
            auto fx = f(x);
            YFm = fx < YFm ? fx : YFm;
            YFM = fx > YFM ? fx : YFM;
        }
        if ((YFM - YFm) / (XFM - XFm) > 5) {
            YFM = 5 * (XFM - XFm);
            YFm = -5 * (XFM - XFm);
        }

        cadru_poarta();
        calctran();
        // trasarea axei x
        if (YFm < 0 && YFM > 0) {
            fl_line(XPm, YDisp(0), XPM, YDisp(0));
        }
        // trasarea axei y
        if (XFm < 0 && XFM > 0) {
            fl_line(XDisp(0), YPm, XDisp(0),  YPM);
        }
        // trasare grafic

        float last_y, last_x;

        bool first = true;
        bool last_crossed = false;
        bool can_draw = true;
        for (float x = xmin; x < xmax - pas; x += pas) {
            auto y = f(x);

            if (!first) {
                if (y > YFM || y < YFm) {
                    if (y > YFM) {
                        y = YFM;
                    }
                    if (y < YFm) {
                        y = YFm;
                    }
                    if (last_crossed) {
                        can_draw = false;
                    }

                    last_crossed = true;
                } else {
                    last_crossed = false;
                    can_draw = true;
                }

                if (can_draw) {
                    fl_line(XDisp(last_x), YDisp(last_y), XDisp(x), YDisp(y));
                }
            }

            last_y = y;
            last_x = x;
            first = false;
        }
    }

    void text(const char *str) { fl_draw(str, XPm, YPM + 12); }

    void draw() override {
        fl_color(FL_WHITE);

        int xmaxe, ymaxe, stg = 50, drt = 50;
        float pas = 0.01;
        init_grafic();

        xmaxe = width;
        ymaxe = height;

        XPm = stg;
        XPM = xmaxe / 2 - drt / 2;
        YPm = stg;
        YPM = ymaxe / 2 - drt / 2;

        grafic(-5, 5, pas, f2);
        text("sin(x)");
    }
};
