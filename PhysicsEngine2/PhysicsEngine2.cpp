#include "raylib.h"
#include <math.h>
#include <stdio.h>

// simple physics engine that simulates some balls dropping
// its in 1 file cuz its kinda simple, didnt want to mess w headers
// this is like a starting project so i get used to raylib
// next project finna be tuff but gonna take way longer

#define BALL_COUNT 100	// max number of balls that u can drop 

typedef struct {	// ball structure	
	Vector2 pos;
	Vector2 vel;
	float radius;
	Color color;
	bool active;
} Circle;

static void DrawCircleEntity(const Circle* c) {    // function for drawing defined ball
	DrawCircleV(c->pos, c->radius, c->color);
}

static void UpdateCircle(Circle* c, float dt, float gravity, int screenWidth, int screenHeight) {	// following every circle frame to frame, changing speed and vector
	if (!c->active) return;

	c->vel.y += gravity * dt;

	c->pos.x += c->vel.x * dt;
	c->pos.y += c->vel.y * dt;

	c->vel.x *= 0.999f;

	float floorY = (float)screenHeight - c->radius;
	if (c->pos.y > floorY) {
		c->pos.y = floorY;
		c->vel.y *= -0.7f;
		c->vel.x *= 0.98f;
		if (fabs(c->vel.y) < 30.0f) c->vel.y = 0.0f;
	}

	if (c->pos.x < c->radius) { 
		c->pos.x = c->radius;
		c->vel.x *= -0.7f;
	}

	if (c->pos.x > (float)screenWidth - c->radius) {
		c->pos.x = (float)screenWidth - c->radius;
		c->vel.x *= -0.7f;
	}
}

static void SpawnBall(Circle balls[], int* count, Vector2 spawnPos) {	// do i have to explain
	if (*count >= BALL_COUNT) return;

	Circle b = { 0 };
	b.active = true;

	b.pos = spawnPos;

	b.vel.x = (float)GetRandomValue(-600, 600);
	b.vel.y = (float)GetRandomValue(-200, 50);

	b.radius = 20.0f + (float)GetRandomValue(0, 30);

	b.color.r = (unsigned char)GetRandomValue(50, 255);    // making it more fun
	b.color.g = (unsigned char)GetRandomValue(50, 255);
	b.color.b = (unsigned char)GetRandomValue(50, 255);
	b.color.a = 255;

	balls[*count] = b;
	(*count)++;
}

static void ResolveBallBall(Circle* a, Circle* b, float restitution) {    // checking are balls colliding with eachother and resolving the conflict
	if (!a->active || !b->active) return;

	float dx = b->pos.x - a->pos.x;
	float dy = b->pos.y - a->pos.y;

	float r = a->radius + b->radius;
	float dist2 = dx * dx + dy * dy;

	if (dist2 >= r * r) return;

	float dist = sqrtf(dist2);
	if (dist < 0.0001f) {
		dist = 0.0001f;
		dx = 0.0001f;
		dy = 0.0f;
	}

	float nx = dx / dist;
	float ny = dy / dist;

	float penetration = r - dist;

	a->pos.x -= nx * (penetration * 0.5f);
	a->pos.y -= ny * (penetration * 0.5f);
	b->pos.x += nx * (penetration * 0.5f);
	b->pos.y += ny * (penetration * 0.5f);

	float rvx = b->vel.x - a->vel.x;
	float rvy = b->vel.y - a->vel.y;

	float velAlongNormal = rvx * nx + rvy * ny;
	if (velAlongNormal > 0.0f) return;

	float j = -(1.0f + restitution) * velAlongNormal / 2.0f;

	float impulseX = j * nx;
	float impulseY = j * ny;

	a->vel.x -= impulseX;
	a->vel.y -= impulseY;
	b->vel.x += impulseX;
	b->vel.y += impulseY;
}

int main(void) {
	printf("Welcome to my simple raylib physics engine!\n");	// writes this message in the console u can delete this if u want

	const int screenWidth = 1920;	
	const int screenHeight = 1080;

	InitWindow(screenWidth, screenHeight, "raylib physics sim");
	SetTargetFPS(60);

	Circle balls[BALL_COUNT] = { 0 };
	int ballCount = 0;

	Rectangle button = { 1700, 100, 120, 60 };

	Rectangle exitButton = { 100, 100, 100, 50 };


	const float gravity = 1800.0f;
	const float restitution = 0.8f;

	while (!WindowShouldClose()) {
		float dt = GetFrameTime();

		if (CheckCollisionPointRec(GetMousePosition(), button) &&
			IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
			Vector2 spawn = { 960.0f, 80.0f };
			SpawnBall(balls, &ballCount, spawn);
		}

		if (CheckCollisionPointRec(GetMousePosition(), exitButton) &&
			IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
			break;
		}

		for (int i = 0; i < ballCount; i++) {
			UpdateCircle(&balls[i], dt, gravity, screenWidth, screenHeight);
		}

		for (int iter = 0; iter < 2; iter++) {    // kinda slow but its a simple project, there is also a matrix approach for checking for collisions
			for (int i = 0; i < ballCount; i++) {
				for (int j = i + 1; j < ballCount; j++) {
					ResolveBallBall(&balls[i], &balls[j], restitution);
				}
			}
		}

		BeginDrawing();
		ClearBackground(RAYWHITE);

		DrawRectangleRec(button, BLUE);
		DrawText("Drop", (int)button.x + 35, (int)button.y + 22, 16, BLACK);

		DrawRectangleRec(exitButton, RED);
		DrawText("Exit", (int)exitButton.x + 30, (int)exitButton.y + 15, 16, BLACK);

		for (int i = 0; i < ballCount; i++) {
			DrawCircleEntity(&balls[i]);
		}
		
		EndDrawing();
	}


	CloseWindow();
	return 0;
}
