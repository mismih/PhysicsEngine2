#include "raylib.h"

typedef struct {
	Vector2 pos;
	float radius;
	Color color;
} Circle;

Circle c = {
	.pos = {960, 550},
	.radius = 50,
	.color = SKYBLUE
};

void DrawCircleEntity(Circle* c) {
	DrawCircleV(c->pos, c->radius, c->color);
}

int main(void)
{
	const int screenWidth = 1920;
	const int screenHeight = 1080;

	int buttonIsClicked = 0;


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

		Rectangle button = { 1700, 100, 100, 60 };
		DrawRectangle(1700, 100, 100, 60, RED);
		DrawText("Drop", 1735, 125, 8, BLACK);

		if (CheckCollisionPointRec(GetMousePosition(), button) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
			buttonIsClicked = 1;
		}
		
		if (buttonIsClicked) {
			DrawCircleEntity(&c);
		}
		


		EndDrawing();
	}
	CloseWindow();  

	return 0;
}
