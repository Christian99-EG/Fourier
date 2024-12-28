#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>

#define SCREEN_WITH 800
#define SCREEN_HEIGHT 600
#define NUM_HARMONICS 10 //numero de armonicas para la serie de fourier

//funcion para calcular la serie de Fourier
double Fourier(double x, int num_harmonics)
{
    double sum = 0.0;
    for(int i = 1; i <= num_harmonics; i += 2) //Solo terminos impares
    {
        sum += (1.0 / i ) * sin(i * x);
    }
    return sum * (4.0 / M_PI); //Escalado de serie
}

int main(int argc, char* argv[])
{

  const int window_width = 600;
  const int window_height = 800;

  if(SDL_Init(SDL_INIT_VIDEO) != 0)
  {
    printf("Error initializing video %s \n", SDL_GetError());
    return 1;
  }

  //crear ventana

  SDL_Window* window = SDL_CreateWindow
  (
    "Series de fourier", 
    SDL_WINDOWPOS_UNDEFINED, 
    SDL_WINDOWPOS_UNDEFINED,
    window_width,
    window_height,
    SDL_WINDOW_SHOWN
  );

  if(!window)
  {
    printf("ERROR initializing window %s \n", SDL_GetError());
    SDL_Quit();
    return 1;
  }

  //crear render
  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  if(!renderer)
  {
    printf("ERROR initializing render %s \n"), SDL_GetError();
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
  }

  //ciclo principal

  int running = 1;
  SDL_Event event;

  while(running)
  {
    //manejo de eventos
    while(SDL_PollEvent(&event))
    {
        if(event.type == SDL_QUIT)
        {
            running = 0;
        }
    }

    //limpiar pantalla

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    //dibujar ejes
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawLine(renderer, 0, SCREEN_HEIGHT / 2, SCREEN_WITH, SCREEN_HEIGHT /2); //eje x
    SDL_RenderDrawLine(renderer, 0, SCREEN_WITH / 2, 0, SCREEN_WITH / 2); //eje Y


    //dibujar serie de fourier

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

    for(int i = 0; i < SCREEN_WITH; i++)
    {
      double x1 = (i - SCREEN_WITH / 2) * 2 * M_PI / SCREEN_WITH; //Escala x 
      double x2 = ((i+1) - SCREEN_WITH / 2) * 2 * M_PI / SCREEN_WITH;

      double y1 = Fourier(x1, NUM_HARMONICS);
      double y2 = Fourier(x2, NUM_HARMONICS);

      int screen_x1 = i;
      int screen_x2 = i + 1;
      int screen_y1 = SCREEN_HEIGHT / 2 - (int)(y1 * SCREEN_HEIGHT / 4);
      int screen_y2 = SCREEN_HEIGHT / 2 - (int)(y1 * SCREEN_HEIGHT / 4);
      
      SDL_RenderDrawLine(renderer, screen_x1, screen_y1, screen_x2, screen_y2);

      SDL_RenderPresent(renderer);

      SDL_Delay(16); //a 60FPS
    }

    //Liberar recursos

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
  } 
  return 0;
}