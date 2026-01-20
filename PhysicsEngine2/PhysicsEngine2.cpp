#include "raylib.h"
#include <math.h>

#define BALL_COUNT 20

typedef struct {
	Vector2 pos;
	Vector2 vel;
	float radius;
	Color color;
	bool active;
} Circle;

static void DrawCircleEntity(const Circle* c) {
	DrawCircleV(c->pos, c->radius, c->color);
}

static void UpdateCircle(Circle* c, float dt, float gravity, int screenWidth, int screenHeight) {
	if (!c->active) return;

	// gravitacija
	c->vel.y += gravity * dt;

	// integracija
	c->pos.x += c->vel.x * dt;
	c->pos.y += c->vel.y * dt;

	// pod i odskok
	float floorY = (float)screenHeight - c->radius;
	if (c->pos.y > floorY) {
		c->pos.y = floorY;
		c->vel.y *= -0.7f;
		c->vel.x *= 0.98f;

		if (fabs(c->vel.y) < 30.0) c->vel.y = 0.0f;
	}

	// zidovi
	if (c->pos.x < c->radius) { c->pos.x = c->radius; c->vel.x *= -0.7f; }
	if (c->pos.x > (float)screenWidth - c->radius) { c->pos.x = (float)screenWidth - c->radius; c->vel.x *= -0.7f; }
}

static void SpawnBall(Circle balls[], int* count, Vector2 spawnPos) {
	if (*count >= BALL_COUNT) return;

	Circle b = { 0 };
	b.active = true;

	b.pos = spawnPos;

	b.vel.x = 0.0f;
	b.vel.y = 0.0f;

	b.radius = 20.0f + (float)GetRandomValue(0, 30);

	b.color.r = (unsigned char)GetRandomValue(50, 255);
	b.color.g = (unsigned char)GetRandomValue(50, 255);
	b.color.b = (unsigned char)GetRandomValue(50, 255);
	b.color.a = 255;

	balls[*count] = b;
	(*count)++;
}

int main(void) {
	const int screenWidth = 1920;
	const int screenHeight = 1080;

	InitWindow(screenWidth, screenHeight, "raylib physics sim");
	SetTargetFPS(60);

	Circle balls[BALL_COUNT] = { 0 };
	int ballCount = 0;

	Rectangle button = { 1700, 100, 120, 60 };
	const float gravity = 1800.0f;

	while (!WindowShouldClose()) {
		float dt = GetFrameTime();

		if (CheckCollisionPointRec(GetMousePosition(), button) &&
			IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
			Vector2 spawn = { 960.0f, 80.0f };
			SpawnBall(balls, &ballCount, spawn);
		}

		for (int i = 0; i < ballCount; i++) {
			UpdateCircle(&balls[i], dt, gravity, screenWidth, screenHeight);
		}

		BeginDrawing();
		ClearBackground(RAYWHITE);

		DrawRectangleRec(button, RED);
		DrawText("Drop", (int)button.x + 35, (int)button.y + 22, 16, BLACK);

		for (int i = 0; i < ballCount; i++) {
			DrawCircleEntity(&balls[i]);
		}

		EndDrawing();
	}

	CloseWindow();
	return 0;
}
