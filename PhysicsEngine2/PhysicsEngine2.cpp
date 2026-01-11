#include "raylib.h"

typedef struct {
	Vector2 pos;
	float radius;
	Color color;
} Circle;

Circle c = {
	.pos = {960, 550},
	.radius = 50,
	.color = BLUE
};

void DrawCircleEntity(Circle* c) {
	DrawCircleV(c->pos, c->radius, c->color);
}

int main(void)
{
	const int screenWidth = 1920;
	const int screenHeight = 1080;


	InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

	SetTargetFPS(60);

	bool isClickedOnCircle = false;

	while (!WindowShouldClose()) 
	{

		BeginDrawing();

		ClearBackground(RAYWHITE);

		Vector2 mouse = GetMousePosition();

		Vector2 circlePos = { 960, 550 };
		float radius = 50;


		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
			if (CheckCollisionPointCircle(mouse, circlePos, radius)) {
				isClickedOnCircle = !isClickedOnCircle;
			}
		}

		if (isClickedOnCircle) {
			DrawCircle(960, 550, 50, RED);
		}
		else {
			DrawCircle(960, 550, 50, BLUE);
		}

		if (IsCursorOnScreen()) {
			DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
		}
		else {
			DrawText("Move the mouse cursor inside the window to see this text.", 100, 200, 20, LIGHTGRAY);
		}

		EndDrawing();
	}
	CloseWindow();  

	return 0;
}
