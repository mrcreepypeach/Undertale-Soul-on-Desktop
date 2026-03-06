#ifndef _DEBUG
#ifdef _WIN32 || _WIN64
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif
#endif

#include "raylib.h"
#include "raymath.h"
#include <iostream>

const int defaultSoulSpeed = 4;

int soulSpeed = 4;
const int soulSize = 30;
const int windowSizeX = 30;
const int windowSizeY = 35;

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
		
		if (IsKeyDown(KEY_X) || IsKeyDown(KEY_RIGHT_SHIFT)) {
			soulSpeed /= 2;
		}

		if (IsKeyPressed(KEY_Z) || IsKeyPressed(KEY_ENTER)){
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

		if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
			movement.x -= soulSpeed;
		}
		if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
			movement.x += soulSpeed;
		}

		if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) { 
			movement.y -= soulSpeed;
		}
		if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) {
			movement.y += soulSpeed;
		}
		// Normalize diagonal movement
		if (movement.x != 0 || movement.y != 0){ 
			movement.x = (movement.x / sqrt(movement.x * movement.x + movement.y * movement.y)) * soulSpeed;
			movement.y = (movement.y / sqrt(movement.x * movement.x + movement.y * movement.y)) * soulSpeed;
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