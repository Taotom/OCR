#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_operations.h" 
#include "../Struct/matrix.h"
#include <err.h>


void init_sdl()
{
    if(SDL_Init(SDL_INIT_VIDEO) == -1)
        errx(1,"Could not initialize SDL: %s.\n", SDL_GetError());
}

SDL_Surface* load_image(char *path)
{
    SDL_Surface *img;

    img = IMG_Load(path);
    if (!img)
        errx(3, "can't load %s: %s", path, IMG_GetError());

    return img;
}

void SDL_FreeSurface(SDL_Surface *surface);

static inline
Uint8* pixel_ref(SDL_Surface *surf, unsigned x, unsigned y)
{
    int bpp = surf->format->BytesPerPixel;
    return (Uint8*)surf->pixels + y * surf->pitch + x * bpp;
}


int my_strlen(char *str)
{
    int n = 0;
	while (str[n] != '\0') n++;
	return n;
}

char *my_strcat(char *str, char elm)
{
    int n = my_strlen(str)+2;
	char *concat = malloc(n * sizeof(char));
    for (int i = 0 ; i < n-2 ; i++) concat[i] = str[i];
	concat[n-2] = elm;
	concat[n-1] = '\0';
    return concat;
}



void Matrix_Img(Matrix M, char *path)
{
  	SDL_Surface* image_surface;
  	Uint32 pixel;

  	init_sdl();
  	image_surface = SDL_CreateRGBSurface(0, M.width, M.height, 32, 0, 0, 0, 0);

  	for (int i = 0 ; i < M.height ; i++)
  	{
    		for (int j = 0 ; j < M.width ; j++)
    		{
      			if (M.matrix[i*M.width+j] == 1) pixel = SDL_MapRGB(image_surface->format, 0, 0, 0);
      			else pixel = SDL_MapRGB(image_surface->format, 255, 255, 255);

      			put_pixel(image_surface, j, i, pixel);
    		}
  	}

  	SDL_SaveBMP(image_surface, path);
  	SDL_FreeSurface(image_surface);
}


Matrix Img_Matrix (char *path) //binarisation with OTSU method
{
  	SDL_Surface* image_surface;
  	
  	Uint32 pixel;
  	Uint8 r, g, b;
  	
  	int histo[256];
  	for (size_t i = 0; i < 256; i++) histo[i] = 0;

  	init_sdl();

  	image_surface = load_image(path);
  	
  	int width = image_surface->w;
  	int height = image_surface->h;
  
  	Matrix M = new_Matrix(height, width);

	// loop to build the histogram
  	for (int i = 0 ; i < height ; i++)
  	{
    		for (int j = 0 ; j < width ; j++)
    		{
      			pixel = get_pixel(image_surface, j, i);
      			SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
      			
      			Uint8 average = 0.3 * r + 0.59 * g + 0.11 * b; // niveau de gris
      			histo[average] ++;
      			
      			Uint32 pixell = SDL_MapRGB(image_surface->format, average, average, average);
				put_pixel(image_surface, j, i, pixell);
    		}
  	}
  	
  	
  	// loop to determine the threshold
  	
  	//initialization
  	double nbtotpixel = (double)width * height;
  	double w1;
  	double w2;
  	double mu1;
  	double mu2;
  	double sig;
  	
  	int seuil = 0;
  	
  	double sigmax;
  	sigmax = 0;
  	
  	for (int t = 1; t < 256; t++)
  	{
		//class probabilities (w1,w2)
		w1 = 0;
		w2 = 0;
		for (int k = 0; k < t+1; k++)
		{
			w1 += histo[k];
		}
		for (int k = t+1; k < 256; k++)
		{
			w2 += histo[k];
		}
		w1 /= nbtotpixel;
		w2 /= nbtotpixel;
		
		
		//class means (mu1,mu2)
		mu1 = 0;
		mu2 = 0;
		
		for (int k = 0; k < t+1; k++)
		{
			mu1 += (k+1) * histo[k];
		}
		for (int k = t+1; k < 256; k++)
		{
			mu2 += (k+1) * histo[k];
		}
		mu1 /= w1;
		mu2 /= w2;
		
		//inter-class variance (sig)
		sig = w1 * w2 * ((mu1 - mu2) * (mu1 - mu2));
		
		//determine the maximum inter-class variance
		if (sig > sigmax)
		{
			sigmax = sig;
			seuil = t;
		}
	}
	
	
	// binarization loop according to the threshold / matrix build
	for (int i = 0 ; i < height ; i++)
  	{
    		for (int j = 0 ; j < width ; j++)
    		{
      			pixel = get_pixel(image_surface, j, i);
      			SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
      			
      			M.matrix[i*M.width+j] = r < seuil ? 1 : 0;
    		}
  	}
  	 	
  	SDL_FreeSurface(image_surface);

	char *command = "rm ";	// Remove the temp file

	for (int i = 0 ; path[i] != '\0' ; i++) command = my_strcat(command, path[i]);
	system(command);
  	
  	return M;
}




//convolution matrix filtering
char* Filtrage (char* path)
{
  	SDL_Surface* image_surface;

  	Uint32 pixel;
  	Uint8 r, g, b;
  
  	init_sdl();

  	image_surface = load_image(path);
  	
  	int width = image_surface->w;
  	int height = image_surface->h;
  
	// loop to build level of grey
  	for (int i = 0 ; i < height ; i++)
  	{
    		for (int j = 0 ; j < width ; j++)
    		{
      			pixel = get_pixel(image_surface, j, i);
      			SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
      			
      			Uint8 average = 0.3 * r + 0.59 * g + 0.11 * b;
      			
      			Uint32 pixell = SDL_MapRGB(image_surface->format, average, average, average);
				put_pixel(image_surface, j, i, pixell);
    		}
  	}
  	
  	//initialization
  	int sum;
  	int coef;
  	int ctot = 16;

  	for (int i = 1 ; i < height-1 ; i++) 
  	{
    		for (int j = 1 ; j < width-1 ; j++) 
    		{
				//initialization sum
				sum = 0;

				for (int k = -1; k < 2; k++) 
				{
					for (int l = -1; l < 2; l++)
					{
						coef = 1;
						
						pixel = get_pixel(image_surface, j-k, i-l);
						SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
						
						if (k == 0 || l == 0)
						{
							coef = k == l ? 4 : 2;
						}
						sum += r * coef;
					}
				}
				
				pixel = get_pixel(image_surface, j, i);
      			SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
      			
				int valmed = sum / ctot;
				Uint32 pixell = SDL_MapRGB(image_surface->format, valmed, valmed, valmed);
				put_pixel(image_surface, j, i, pixell);
    		}
  	}
  	char *save = "temp";
  	SDL_SaveBMP(image_surface, save);

  	SDL_FreeSurface(image_surface);

	return save;
}
