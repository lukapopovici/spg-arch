#pragma once
//#define WIN32 // doar in Windows
#include <iostream>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/fl_draw.H>
#include <cmath>
#define PI 3.141592f

class MyWidget2 : public Fl_Window
{
	int xc, yc; // centrul ferestrei
	float x[4], y[4]; // coordonatele virfurilor unui patrat
	int xcp, ycp; // coondonatele centrului patratului
	int raza, latura;
	int N; // nr de patrate

public:
	MyWidget2() : Fl_Window(200, 200, 512, 512, "SPG Lab1")
	{
		xc = this->w() / 2;
		yc = this->h() / 2;
		N = 10;
		init_obiect(100, 40);
	}

	void desen()
	{
		//deseneaza un patrat cu varfurile (x[0] y[0]), (x[1] y[1]), etc.
		for (int i = 0; i < 3; i++)
			fl_line(x[i], y[i], x[i + 1], y[i + 1]);
		fl_line(x[3], y[3], x[0], y[0]);
	}

	void init_obiect(int raza, int latura)
	{
		//functie ce calculeaza coordonatele varfurilor in pozitia initiala

		// . . .
	}

	void draw() override
	{
		Fl_Window::draw(); // apeleaza metoda din clasa de baza pentru a desena background-ul
		fl_color(FL_BLUE);

		float du = 2 * PI / N; //unghiul dintre doua patrate succesive
		float c = cos(du);
		float s = sin(du);
		for (int i = 0; i < N; i++)
		{
			// se deseneaza patratul curent
			// se calculeaza noua pozitie a centrului patratului
			// . . .

			// se determina coordonatele varfurilor noului patrat functie de pozitia noului centru
			// . . .
		}
	}

};


