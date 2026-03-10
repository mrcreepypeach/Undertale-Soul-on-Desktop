// --- YOU - A simple desktop application that allows you to move an UNDERTALE/DELTARUNE soul around the screen using keyboard and mouse inputs.
#ifndef _DEBUG

#ifdef _WIN32 || _WIN64

#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif
#endif

#include "raylib.h"
#include "raymath.h"
#include <iostream>

// Timer struct to handle the smooth movement after clicking

struct timer
{
	bool started = false;
	float dt;
	float time = 0;

	timer(float waitTime) {
		dt = GetFrameTime();
		time = waitTime;
	}
	void start(float waitTime) { // Start the timer with a specified wait time, 0 will just start the timer
		std::cout << "started timer\n";
		if (waitTime >= 0){
			started = true;
			if (waitTime > 0) {
				time = waitTime;
			}
		}
		started = true;
	}
	void update(float dt) {
		if (started) {
			std::cout << "timer: " << time << "\n";
			dt = GetFrameTime();
			time -= dt;
			if (time <= 0) {
				std::cout << "timer done\n";
				started = false;
				time = 0;
			}
		}
	}
};

const int defaultSoulSpeed = 4;

int soulSpeed = 4;
float smoothSpeed = 0.25;
const int soulSize = 30;
const int windowSizeX = 30;
const int windowSizeY = 35;

bool toggleMouseMode = false;

timer clickTimer(.25);

int main() {
	// Set window configuration flags
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
		// Delta time for frame independent movement
		float dt = GetFrameTime();

		// Update the music stream every frame
		UpdateMusicStream(amb);

		// Get mouse position
		Vector2 mousePos = GetMousePosition();
		// Get current window position for inputs
		Vector2 curPos = GetWindowPosition();
		Vector2 movement = { 0, 0 };
		
		soulSpeed = defaultSoulSpeed;

		// Get mouse inputs
		
		/* Couldnt get this to work yet, will try again in the future
		if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)){
			if (toggleMouseMode == false) {
				toggleMouseMode = true;
			}
			else {
				toggleMouseMode = false;
				smoothSpeed = 0.25;
			}
		}
		*/

		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) || toggleMouseMode == true) {
			HideCursor();
			curPos.x += (mousePos.x - soulSize / 2) * smoothSpeed;
			curPos.y += (mousePos.y - soulSize / 2) * smoothSpeed;
		}
		else { ShowCursor(); }
		// If the left mouse button is released, start the click timer to continue moving towards the mouse position for a short time to create a physics effect, will be improved in the future to be more accurate and less janky
		if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
			clickTimer.start(0.1);
		}

		// timer check
		if (clickTimer.started == true) {
			clickTimer.update(dt);
			curPos.x += (mousePos.x - soulSize / 2) * smoothSpeed;
			curPos.y += (mousePos.y - soulSize / 2) * smoothSpeed;
			if (smoothSpeed > 0) { 
				smoothSpeed -= 0.05;
				std::cout << "subtracting from smoothspeed\nsmoothSpeed value: " << smoothSpeed << "\n";
			}
		}
		else { smoothSpeed = 0.25; }
		// Get keyboard inputs
		
		// Holding SHIFT or X will slow down the soul for more precise movement
		if (IsKeyDown(KEY_X) || IsKeyDown(KEY_RIGHT_SHIFT)) {
			soulSpeed /= 2;
		}

		// Pressing Z or ENTER will play a click sound, will add interacting functionality in the future
		if (IsKeyPressed(KEY_Z) || IsKeyPressed(KEY_ENTER)){
			PlaySound(click);
		}

		// Pressing M will toggle the music on and off
		if (IsKeyPressed(KEY_M)) {
			if (IsMusicStreamPlaying(amb)) {
				PauseMusicStream(amb);
			}
			else {
				PlayMusicStream(amb);
			}
		}

		// WASD or arrow keys to move the soul
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

		// Update soul position
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

	// Unload resources and close the application

	UnloadTexture(soul);
	UnloadImage(soulPNG);
	StopMusicStream(amb);
	UnloadMusicStream(amb);
	UnloadSound(click);

	CloseWindow();

}