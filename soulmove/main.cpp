#ifndef _DEBUG

#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#endif

#include "raylib.h"
#include "raymath.h"
#include <iostream>

int defaultSoulSpeed = 4;

int soulSpeed = 4;
int soulSize = 30;
int windowSizeX = 30;
int windowSizeY = 35;

int main() {
	SetConfigFlags(FLAG_WINDOW_TRANSPARENT);
	SetConfigFlags(FLAG_WINDOW_TOPMOST);
	SetConfigFlags(FLAG_WINDOW_UNDECORATED);

	InitWindow(windowSizeX, windowSizeY, "YOU");
	SetTargetFPS(60);
	
	InitAudioDevice();

	// Load image & texture data
	Image soulPNG = LoadImage("res/dmc.png");
	Texture2D soul = LoadTextureFromImage(soulPNG);
	soul.height = soulSize;
	soul.width = soulSize;

	// Setting window icon
	SetWindowIcon(soulPNG);

	// Load audio
	Sound click = LoadSound("res/click.wav");
	Music amb = LoadMusicStream("res/AUDIO_DRONE.ogg");
	amb.looping = true;

	PlayMusicStream(amb);

	while (!WindowShouldClose()) {
		// Get current window position for inputs
		UpdateMusicStream(amb);
		Vector2 curPos = GetWindowPosition();
		Vector2 movement = { 0, 0 };
		
		soulSpeed = defaultSoulSpeed;

		// Get inputs
		
		if (IsKeyDown(KEY_X)) {
			soulSpeed /= 2;
		}

		if (IsKeyPressed(KEY_Z)){
			PlaySound(click);
		}

		if (IsKeyPressed(KEY_M)) {
			if (IsMusicStreamPlaying(amb)) {
				PauseMusicStream(amb);
			}
			else {
				PlayMusicStream(amb);
			}
		}

		if (IsKeyDown(KEY_LEFT)) {
			movement.x -= soulSpeed;
		}
		if (IsKeyDown(KEY_RIGHT)) {
			movement.x += soulSpeed;
		}

		if (IsKeyDown(KEY_UP)) { 
			movement.y -= soulSpeed;
		}
		if (IsKeyDown(KEY_DOWN)) {
			movement.y += soulSpeed;
		}
		// Normalize diagonol movement
		if (movement.x != 0 || movement.y != 0){ 
			movement = Vector2Normalize(movement);
			movement.x *= soulSpeed;
			movement.y *= soulSpeed;
		}

		curPos.x += movement.x;
		curPos.y += movement.y;
		SetWindowPosition(curPos.x, curPos.y);

		// Draw the soul
		BeginDrawing();

		if (IsTextureValid(soul)){ DrawTexture(soul, 1, 1, WHITE); }
		else { std::cout << "Failed to load texture for soul"; }
		ClearBackground(BLANK);

		EndDrawing();
	}

	UnloadTexture(soul);
	UnloadImage(soulPNG);
	StopMusicStream(amb);
	UnloadMusicStream(amb);
	UnloadSound(click);

	CloseWindow();

}