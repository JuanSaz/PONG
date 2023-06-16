#include <iostream>
#include "SDL.h"
#include "SDL_image.h"
#include <vector>
#include <time.h>

using namespace std;

//GLOBALES
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
int previousTick = 0;
int puntajeEnemigo = 0;
int miPuntaje = 0;


//INTERRUPTORES
bool isGameRunning = true;
bool canMoveUp = true;
bool canMoveDown = true;
bool dir = false;
bool dirBorder = false;
bool dirTop = false;
bool dirBot = false;
bool test = false;


//Colisiones
SDL_bool enemyCollision = SDL_FALSE;
SDL_bool myCollision = SDL_FALSE;
SDL_bool topCollision = SDL_FALSE;
SDL_bool botCollision = SDL_FALSE;
SDL_bool leftCollision = SDL_FALSE;
SDL_bool rightCollision = SDL_FALSE;

//DEBUG MODE																	//Basicamente activandolo, permite mover la paleta de la IA para testear cosas y ver colisiones
bool debugMode = true;

//CONSTANTES
const int WINDOWX = 1280;														//Definicion de Resolucion del Pong 
const int WINDOWY = 720;														//Basicamente para hacerlo responsive y no hardcodear
int FPS = 60;
const int FRAME_TARGET_TIME = (1000 / FPS);

//ESTRUCTURAS DE TDAS
struct paleta {
	int w = 50;
	int h = 250;
	int x = 100;
	int y = WINDOWY / 4;
}paleta;

struct paleta2 {
	int w = 50;
	int h = 250;
	int x = WINDOWX - 100;
	int y = WINDOWY / 4;
}paleta2;

struct pelota {
	int w = 25;
	int h = 25;
	int x = WINDOWX / 2 - w;
	int y = WINDOWY / 2 - w;
	int speedX = 400;
	int speedY = 100;
}pelota;

struct upperBorder {
	int w = WINDOWX;
	int h = 5;
	int x = 0;
	int y = 0;
}upperBorder;

struct bottomBorder {
	int w = WINDOWX;
	int h = 5;
	int x = 0;
	int y = WINDOWY - 5;
}bottomBorder;

struct leftBorder {
	int w = 1;
	int h = WINDOWY;
	int x = 0;
	int y = 5;
}leftBorder;

struct rightBorder {
	int w = 1;
	int h = WINDOWY;
	int x = WINDOWX - 10;
	int y = 0;
}rightBorder;

void initializeWindow() {

	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow("PONG by Juan", 100, 100, WINDOWX, WINDOWY, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, 0);
}

void menu() {

}

void setup() {															//INICIALIZAR LAS POSICIONES DE LOS OBJETOS
	paleta.w;
	paleta.h;
	paleta.x;
	paleta.y;

	paleta2.w;
	paleta2.h;
	paleta2.x;
	paleta2.y;

	pelota.w;
	pelota.h;
	pelota.x;
	pelota.y;

	upperBorder.w;
	upperBorder.h;
	upperBorder.x;
	upperBorder.y;

	bottomBorder.w;
	bottomBorder.h;
	bottomBorder.x;
	bottomBorder.y;

	leftBorder.w;
	leftBorder.h;
	leftBorder.x;
	leftBorder.y;

	rightBorder.w;
	rightBorder.h;
	rightBorder.x;
	rightBorder.y;

	//ACA TENGO QUE SETEAR LAS POSICIONES, Y VARIABLES INICIALES DEL JUEOGO (POSICION DE LAS DOS PALETAS, SUS VELOCIDADES DE MOVIMIENTO, LA POSICION DE LA PELOTA Y SU VELOCIDAD//
}

void timer() {
	int initial = (SDL_GetTicks() / 1000);					// SEGUNDOS DESDE EL INICIO DEL JUEGO


	if (initial == 120) {									// PASADOS LOS 120 SEGUNDOS, TERMINA EL JUEGO
		isGameRunning = false;
	}

}

void moveBall(float deltaTime) {

	pelota.x += (pelota.speedX * deltaTime);
	pelota.y += (pelota.speedY * deltaTime);

	if (enemyCollision) {
		dirBorder = false;
		float calc = 1 - 2 * (pelota.y - paleta2.y) / 250.0;
		if (calc <= 0) {																		//-45 GRADOS
			pelota.speedX = -abs(pelota.speedX);
			pelota.speedY = abs(pelota.speedY);
			pelota.x += (pelota.speedX * deltaTime);
			pelota.y += (pelota.speedY * deltaTime);

		}
		else if (calc >= 0) {																//45 GRADOS
			pelota.speedX = -abs(pelota.speedX);
			pelota.speedY = -abs(pelota.speedY);
			pelota.x += (pelota.speedX * deltaTime);
			pelota.y += (pelota.speedY * deltaTime);
		}
	}

	if (myCollision) {
		dirBorder = false;
		float calc = 1 - 2 * (pelota.y - paleta2.y) / 250.0;

		if (calc <= 0) {
			pelota.speedX = abs(pelota.speedX);
			pelota.speedY = abs(pelota.speedY);
			pelota.x += (pelota.speedX * deltaTime);
			pelota.y += (pelota.speedY * deltaTime);
		}
		else if (calc >= 0) {
			pelota.speedX = abs(pelota.speedX);
			pelota.speedY = -abs(pelota.speedY);
			pelota.x += (pelota.speedX * deltaTime);
			pelota.y += (pelota.speedY * deltaTime);
		}
	}

	if (topCollision) {
		dirBorder = true;
		dirTop = true;
		dirBot = false;
	}

	if (botCollision) {
		dirBorder = true;
		dirBot = true;
		dirTop = false;
	}

	if (dirBorder) {
		if (dirTop) {
			pelota.speedY = abs(pelota.speedY);
		}
		if (dirBot) {
			pelota.speedY = -abs(pelota.speedY);
		}
	}

}

void calculatePoint() {																						//SUMAR PUNTAJE

	if (rightCollision) {
		miPuntaje++;
	}

	if (leftCollision) {
		puntajeEnemigo++;
	}

	int random = (rand() % 2);																			// NUMERO RANDOM PARA HACER EL JUEGO MAS DINAMICO (VARIA LA VELOCIDAD DE SPAWN)

	if (random) {
		pelota.x = WINDOWX / 2;
		pelota.y = WINDOWY / 2;
		pelota.speedX *= 1;
		cout << "1" << endl;
	}
	else {
		pelota.x = WINDOWX / 2;
		pelota.y = WINDOWY / 2;
		pelota.speedX *= -1;
		cout << "0" << endl;
	}

}

void detectCollisions(SDL_Rect paleta, SDL_Rect paleta2, SDL_Rect pelota, SDL_Rect upper, SDL_Rect bottom, SDL_Rect right, SDL_Rect left) {

	enemyCollision = SDL_HasIntersection(&paleta2, &pelota);
	myCollision = SDL_HasIntersection(&paleta, &pelota);
	topCollision = SDL_HasIntersection(&upper, &pelota);
	botCollision = SDL_HasIntersection(&bottom, &pelota);
	rightCollision = SDL_HasIntersection(&right, &pelota);
	leftCollision = SDL_HasIntersection(&left, &pelota);

	if (rightCollision || leftCollision) {
		calculatePoint();
	}

}

void update() {
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), previousTick + FRAME_TARGET_TIME));			//Obliga a tener 60 FPS MAXIMOS (seteados en las CONSTANTES)
	float deltaTime = (SDL_GetTicks() - previousTick) / 1000.0;								//Obtengo el DELTATIME con una division con float asi no trunca
	previousTick = SDL_GetTicks();
	moveBall(deltaTime);
}

void render() {
	SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);										//Renderizado del fondo
	SDL_RenderClear(renderer);

	SDL_Rect paddle{																		//Renderizado de la paleta 1 con los datos del struct
		paleta.x,
		paleta.y,
		paleta.w,
		paleta.h,
	};

	SDL_Rect paddle2{																		//Renderizado de la paleta 2 con los datos del struct
		paleta2.x,
		paleta2.y,
		paleta2.w,
		paleta2.h,
	};

	SDL_Rect ball{																			//Renderizado de la pelota con los datos del struct
		pelota.x,
		pelota.y,
		pelota.w,
		pelota.h,
	};

	SDL_Rect upper{																			//Renderizado de la paleta 1 con los datos del struct
		upperBorder.x,
		upperBorder.y,
		upperBorder.w,
		upperBorder.h,
	};

	SDL_Rect bottom{
		bottomBorder.x,
		bottomBorder.y,
		bottomBorder.w,
		bottomBorder.h,
	};

	SDL_Rect left{
		leftBorder.x,
		leftBorder.y,
		leftBorder.w,
		leftBorder.h,
	};

	SDL_Rect right{
	rightBorder.x,
	rightBorder.y,
	rightBorder.w,
	rightBorder.h,
	};

	if (debugMode) {
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderFillRect(renderer, &right);

		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderFillRect(renderer, &left);

		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		SDL_RenderFillRect(renderer, &upper);

		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		SDL_RenderFillRect(renderer, &bottom);
	}
	else {
		SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
		SDL_RenderFillRect(renderer, &right);

		SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
		SDL_RenderFillRect(renderer, &left);

		SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
		SDL_RenderFillRect(renderer, &upper);

		SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
		SDL_RenderFillRect(renderer, &bottom);
	}
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &paddle);

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &paddle2);

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &ball);

	detectCollisions(paddle, paddle2, ball, upper, bottom, right, left);

	//ACA SE RENDERIZAN LOS OBJETOS DEL JUEGO
	SDL_RenderPresent(renderer);
}

void checkInput() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			isGameRunning = false;
			cout << "Se cerró el juego." << endl;
			break;
		default:
			break;

		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_w:
				paleta.y -= 30;
				break;
			case SDLK_s:
				paleta.y += 30;
				break;
			case SDLK_ESCAPE:
				isGameRunning = false;
				break;
			case SDLK_DOWN:
				paleta2.y += 30;
				break;
			case SDLK_UP:
				paleta2.y -= 30;
				break;
			default:
				break;
			}
		}
	}
}

void results() {

}

int main(int argc, char* args[]) {

	initializeWindow();
	menu();
	setup();
	srand(time(NULL));

	while (isGameRunning)
	{
		checkInput();
		update();
		render();
		timer();
	}

	results();

	return 0;
}

