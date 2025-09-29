// raylib includes
#include "raylib.h"
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

// json utility (for kvp)
#include <nlohmann/json.hpp>

// hashing lib
#include "xxhash.h"

// other
#include <iostream>

// Hash function using xxHash
unsigned int hash(const char *key) {
	#define TABLE_SIZE 100
    // Use XXH32 to hash the key
    unsigned int hashValue = XXH32(key, strlen(key), 0); // Seed = 0
    return hashValue % TABLE_SIZE; // Map hash value to table size
}


int main ()
{
	using json = nlohmann::json;

	json j2 = {
		{"pi", 3.141},
		{"happy", true},
		{"name", "Niels"},
		{"nothing", nullptr},
		{"answer", {
			{"everything", 42}
		}},
		{"list", {1, 0, 2}},
		{"object", {
			{"currency", "USD"},
			{"value", 42.99}
		}}
	};
	std::cout << j2.dump(4) << std::endl;

	std::cout << "Result of hash: " << hash("randomString") << std::endl;

	/*
	Raylib example file.
	This is an example main file for a simple raylib project.
	Use this as a starting point or replace it with your code.

	by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

	*/

	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(1280, 800, "Hello Raylib");

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	// Load a texture from the resources directory
	Texture wabbit = LoadTexture("wabbit_alpha.png");
	
	// game loop
	while (!WindowShouldClose())		// run the loop untill the user presses ESCAPE or presses the Close button on the window
	{
		// drawing
		BeginDrawing();

		// Setup the back buffer for drawing (clear color and depth buffers)
		ClearBackground(BLACK);

		// draw some text using the default font
		DrawText("Hello Raylib", 200,200,20,WHITE);

		// draw our texture to the screen
		DrawTexture(wabbit, 400, 200, WHITE);
		
		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
	}

	// cleanup
	// unload our texture so it can be cleaned up
	UnloadTexture(wabbit);

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
