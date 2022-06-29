#include <bits/stdc++.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include "Body.h"

// Standard Library
using namespace std;

// Graphics
SDL_Window *window = NULL;
SDL_Surface *surface = NULL;
TTF_Font *font = NULL;
SDL_Surface *textY = NULL;
SDL_Surface *textM = NULL;
SDL_DisplayMode res;
unsigned int WindowWidth;
unsigned int WindowHeight;

const SDL_Color White = {0xff, 0xff, 0xff};
const SDL_Color Black = {0x00, 0x00, 0x00};
const SDL_Color SunShine = {0xff, 0x99, 0x00};
const SDL_Color BluePlanet = {0x00, 0x99, 0xee};

//Drawing parameters for ray, which are different than the actual rays of the bodies
const int SunRay = 40;								
const int EarthRay = 15;
const int MoonRay = 4;							
const int CharSize = 36;

void DrawDot(unsigned int x, unsigned int y, SDL_Color Col){
	Uint8* pixel_ptr = (Uint8*)surface->pixels + (y * WindowWidth + x) * 4;
	*(pixel_ptr + 2) = Col.r;
	*(pixel_ptr + 1) = Col.g;
	*(pixel_ptr) = Col.b;
}

void DrawCircle(int x, int y, int radius, SDL_Color Col){ 
    for (int w = 0; w < 2 * radius; w++){
        for (int h = 0; h < 2 * radius; h++){
            int dx = radius - w;
            int dy = radius - h;
            if ((dx * dx + dy * dy) < (radius * radius))
                DrawDot(x + dx, y + dy, Col);
        }
    }
}

bool CheckPix(unsigned int x, unsigned int y){
    Uint8* pixel_ptr = (Uint8*)surface->pixels + (y * WindowWidth + x) * 4;
    if(*(pixel_ptr + 2) == 0x00 && *(pixel_ptr + 1) == 0x00 && *(pixel_ptr + 2) == 0x00)
    	return true;
    else
    	return false;
}

void Years(bool &Switch, int &PeriodCounter, const Body &body, const float R){
	string str = " years have passed...";		
	string str1 = " year has passed...";
	string StrDef;
	stringstream ss;
	if(Switch){
		if(body.pos.x > R - 1){
			PeriodCounter++;
			Switch = false;
			ss << PeriodCounter;
			PeriodCounter != 1 ? StrDef = ss.str() + str :	StrDef = ss.str() + str1;
	    	textY = TTF_RenderText_Shaded(font, StrDef.c_str(), White, Black);	    	
		}
	}
	if(body.pos.x < R - 1) 
		Switch = true;
}

void Months(bool &Switch, int &PeriodCounter, const Body &body1, const Body &body2){
	string str = " months have passed...";		
	string str1 = " month has passed...";
	string StrDef;
	stringstream ss;
	if(Switch){
		if(body1.pos.x > body2.pos.x){
			PeriodCounter++;
			Switch = false;
			ss << PeriodCounter;
			PeriodCounter != 1 ? StrDef = ss.str() + str : 	StrDef = ss.str() + str1;
	    	textM = TTF_RenderText_Shaded(font, StrDef.c_str(), White, Black);
		}
	}
	if(body1.pos.x < body2.pos.x)
		Switch = true;
}

int main(int argc, char* argv[]){
	
	// SDL boilerplate
	SDL_Init(SDL_INIT_VIDEO);
	SDL_GetCurrentDisplayMode(0, &res);
	WindowWidth = res.w;
	WindowHeight = res.h;
	window = SDL_CreateWindow("A coded solar system", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WindowWidth, WindowHeight, SDL_WINDOW_SHOWN);
	surface = SDL_GetWindowSurface(window);
	TTF_Init();
    font = TTF_OpenFont("Cascadia.ttf", CharSize);
    
	// SDL Event handler
	SDL_Event e;

	const float EllipseSize = 2.2f;
	const float dt = 1.0 / 30.0f;
	const float r = 15.8f;
	const float R = 230.0f;
	
	Body Sun(1000.0f, 300.0f);
	Body Earth(100.0f, 4.0f);
	Body Moon(1.0f, 1.0f);
	
	Sun.setPos(30.0f, 30.0f);
	Sun.setVel(0.0f, 0.0f);
	
	Earth.setPos(R, 0.0f);
	Earth.setVel(0.0f, sqrt(Sun.m / R));
	
	Moon.setPos(R, r);
	Moon.setVel(sqrt(Earth.m / r), Earth.vel.y);
	
	vector <pair <int, int> > Stars;
	
	int YearCount = -1, MonthCount = -1;
	bool SwitchY = true, SwitchM = true;
	
	for(int i = 0; i < 300; i++){
		int x = rand() % WindowWidth;
		int y = rand() % WindowHeight;
		// Stars near the Sun are not visible (Sun centre is at res/2 + 30)
		if((res.w/2 - 80 < x && x < res.w/2 + 180) && (res.h/2 - 80 < y && y < res.h/2 + 180))
			continue;
		DrawDot(x, y, White);
		Stars.push_back(make_pair(x, y));	
	}
	
	DrawCircle(Sun.pos.x * EllipseSize + res.w/2, Sun.pos.y * EllipseSize + res.h/2, SunRay, SunShine);

	bool quit = false;
	while (!quit){
		while (SDL_PollEvent(&e) != 0)
			if (e.type == SDL_QUIT) quit = true;
		
		Moon.PulledBy(Earth);
		Moon.PulledBy(Sun);
		Earth.PulledBy(Moon);
		Earth.PulledBy(Sun);
	
		DrawCircle((Earth.pos.x * EllipseSize + res.w/2), (Earth.pos.y * EllipseSize + res.h/2), EarthRay, Black);
		DrawCircle((Moon.pos.x * EllipseSize + res.w/2), (Moon.pos.y * EllipseSize + res.h/2), MoonRay, Black);
		
		//Redrawing the stars that got covered by Earth or Moon that are actually non-covered
		//(they are black because of the black DrawCircle fun)
		for(int i = 0; i < Stars.size(); i++){
			if(CheckPix(Stars[i].first, Stars[i].second))
				DrawDot(Stars[i].first, Stars[i].second, White);		
		}
		
		Moon.Update(dt);
		Earth.Update(dt);
		Sun.Update(dt);

		DrawCircle(Earth.pos.x * EllipseSize + res.w/2, (Earth.pos.y * EllipseSize + res.h/2), EarthRay, BluePlanet);
		DrawCircle(Moon.pos.x * EllipseSize + res.w/2, (Moon.pos.y * EllipseSize + res.h/2), MoonRay, White);		
		
		/*
		It makes more sense count years or months, not both togheter (ex. 1 year and 3 months),
		because 12 month are sligthly less (in terms of time) than a year.
		The Moon rotates around the Earth in about 28 days. 28 * 12 = 336, not 365.
		1 month = 0.92 * 1/12 of year
		*/
		Years(SwitchY, YearCount, Earth, R);
		Months(SwitchM, MonthCount, Moon, Earth);
	
		SDL_BlitSurface(textM, NULL, surface, NULL);	// Print Month counter
		SDL_Rect dstY = {0, CharSize + 10, 0, 0};		// Set the space to print the Year counter
        SDL_BlitSurface(textY, NULL, surface, &dstY);	// Print year counter
        
		// Update frame buffer
		SDL_UpdateWindowSurface(window);			

	}
			
	// Quit SDL subsystems 
	SDL_FreeSurface(surface);
    SDL_FreeSurface(textY);
    SDL_FreeSurface(textM);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
    
	// Destroy window 
	SDL_DestroyWindow(window); 

	return 0;

}
