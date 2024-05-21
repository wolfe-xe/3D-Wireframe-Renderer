#include <stdio.h>
#include <SDL.h>

#include <vector>;
#include <numeric>;
#include <cmath>;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
std::vector<SDL_FPoint> points;

int game_is_running = 0;

struct vec3 {
	float x, y, z;
};

struct connection {
	int a;
	int b;
};

int InitializeWindow(void) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "ERROR INTIALIZING \n");
		return 0;
	}

	window = SDL_CreateWindow(
		"REND",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		640, 640,
		SDL_WINDOW_SHOWN
	);
	if (!window) {
		fprintf(stderr, "ERROR INITIALIZING WINDOW \n");
		return 0;
	}

	renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_RenderSetScale(renderer, 2, 2);
	if (!renderer) {
		fprintf(stderr, "ERROR INITIALIZING RENDERER \n");
	}

	return 1; 

}

//void Rotate(vec3& points, float x = 1, float y = 1, float z = 1) {
//	float rad = 0;
//
//	rad = x;
//	points.y = std::cos(rad) * points.y - std::sin(rad) * points.z;
//	points.z = std::sin(rad) * points.y + std::cos(rad) * points.z;
//	
//	rad = y;
//	points.x = std::cos(rad) * points.x + std::sin(rad) * points.z;
//	points.z = std::sin(rad) * points.x + std::cos(rad) * points.z;
//	
//	rad = z;
//	points.x = std::cos(rad) * points.x - std::sin(rad) * points.y;
//	points.y = std::sin(rad) * points.x + std::cos(rad) * points.y;
//
//}

void RotateX(vec3& p, float angle) {
	float rad = angle;
	p.y = std::cos(rad) * p.y - std::sin(rad) * p.z;
	p.z = std::sin(rad) * p.y + std::cos(rad) * p.z;
}

void RotateY(vec3& p, float angle) {
	float rad = angle;
	p.x = std::cos(rad) * p.x + std::sin(rad) * p.z;
	p.z = -std::sin(rad) * p.x + std::cos(rad) * p.z;
}

void RotateZ(vec3& p, float angle) {
	float rad = angle;
	p.x = std::cos(rad) * p.x - std::sin(rad) * p.y;
	p.y = std::sin(rad) * p.x + std::cos(rad) * p.y;
}

void Pixel(float x, float y) {
	points.emplace_back(SDL_FPoint{x, y});
}

void Line(float x1, float y1, float x2, float y2) {
	// x1 and y1 being the starting point and x2 and y2 being the ending point
	float dx = x2 - x1;
	float dy = y2 - y1;

	// using the distance formula we are going to find the length of the line.
	float length = std::sqrtf(dx * dx + dy * dy);
	float angle = std::atan2(dy, dx);

	for (int i = 0; i < length; i++) {
		// getting the forward vector and multiply with *i* to scale it up a lil bit.
		Pixel(
			x1 + std::cos(angle) * i,
			y1 + std::sin(angle) * i
		);
	}
}

void ProcessInput() {
	SDL_Event event;
	SDL_PollEvent (&event);
	switch (event.type)
	{
	case SDL_QUIT:
		game_is_running = 0;
		break;

	case SDL_KEYDOWN:
		if (event.key.keysym.sym == SDLK_ESCAPE) {
			game_is_running = 0;
			break;
		}
	}
}

void Start() {
	//for (int i = 0; i < 100; i++) {
	//	Pixel(rand() % 640, rand() % 640);
	//}
}

void Update() {
	points.clear();

	static std::vector<vec3> points{
		{100, 100, 100},
		{200, 100, 100},
		{200, 200, 100},
		{100, 200, 100},

		{100, 100, 200},
		{200, 100, 200},
		{200, 200, 200},
		{100, 200, 200}
	};

	std::vector<connection> connections{
		{0,4},
		{1,5},
		{2,6},
		{3,7},

		{0,1},
		{1,2},
		{2,3},
		{3,0},

		{4,5},
		{5,6},
		{6,7},
		{7,4},
	};
	
	vec3 centroid{0,0,0};
	for (auto& p : points) {
		centroid.x += p.x;
		centroid.y += p.y;
		centroid.z += p.z;
	}

	centroid.x /= points.size();
	centroid.y /= points.size();
	centroid.z /= points.size();
	
	static float angle = 0.01f;
	
	for (auto& p : points) {
		p.x -= centroid.x;
		p.y -= centroid.y;
		p.z -= centroid.z;

		//Rotate(p, 0.02, 0.01, 0.04);
		RotateX(p, 0.02);
		RotateY(p, 0.01);
		RotateZ(p, 0.04);

		p.x += centroid.x;
		p.y += centroid.y;
		p.z += centroid.z;

		Pixel(p.x, p.y);
	}

	for (const auto& c : connections) {
		Line(points[c.a].x, points[c.a].y,
			points[c.b].x, points[c.b].y);
	}
}

void Render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	for (const auto& point : points) {
		SDL_RenderDrawPointF(renderer, point.x, point.y);
	}
	SDL_RenderPresent(renderer);

}

void DestroyWindow() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit;
}

int main(int argc, char* argv[]) {
	game_is_running = InitializeWindow();

	Start();

	while (game_is_running) {
		Update();
		ProcessInput();
		Render();
		SDL_Delay(30);
	}

	DestroyWindow();
	
	return 0;
}

/// TODO/FUTURE PLANS:
///
/// [] shading
/// [] camera controls