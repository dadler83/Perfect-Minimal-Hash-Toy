// raylib includes
#include "raylib.h"
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

// json utility (for kvp)
#include <nlohmann/json.hpp>
using json = nlohmann::json;

// hashing lib
#include "xxhash.h"

// other
#include <iostream>
#include <bitset>
#include <unordered_set>
#include <unordered_map>


// Hash function using xxHash
unsigned int hash(const char *key, unsigned int numBits, unsigned int seed = 0) {
    // Use XXH32 to hash the key
    unsigned int hashValue = XXH32(key, strlen(key), seed); // Seed = 0
    return hashValue % numBits; // Map hash value to table size
}

void printBitset(std::vector<bool> &set) {
	for (unsigned long i = 0; i < set.size(); i++) {
		if (set[i]) {
			std::cout << "1";
		} else {
			std::cout << "0";
		}
	}
	std::cout << "\n";
}

void PMHF_FromJson(std::vector<bool> &out, json &kvp, std::unordered_set<std::string> &mappedKeySet, 
					std::vector<int> *levelSizes = nullptr, std::vector<int> *hashSeeds = nullptr,
					unsigned int slotOffset = 0, int depth = 0) {
	// break condo.
	if (mappedKeySet.size() == kvp.size())
		return;

	unsigned int intervalSize = kvp.size() - mappedKeySet.size();
	
	out.reserve(out.size() + intervalSize);
	for (unsigned long i = 0; i < intervalSize; i++)
		out.push_back(false);
	
	std::vector<bool> filledBits;
	filledBits.reserve(intervalSize);
	for (unsigned long i = 0; i < intervalSize; i++)
		filledBits.push_back(false);
	std::unordered_map<int, std::string> slotToKeyMapping;

	printBitset(out);
	std::cout << "Remaining Keys To Map: " << intervalSize << "\n";

	for (json::iterator it = kvp.begin(); it != kvp.end(); ++it) {
		// if (!it.is_object()) continue; -- it'll just throw an error for non-kvp
		if (mappedKeySet.find(it.key()) != mappedKeySet.end())
			continue;
		std::cout << it.key() << ", ";

		unsigned int slot = hash(static_cast<const char *>(it.key().c_str()), intervalSize, depth);
		slot += slotOffset;
		if (filledBits[slot] == true) {
			// std::cout << "Collision at slot: " << slot << "\n";
			if (slotToKeyMapping.find(slot) != slotToKeyMapping.end() && mappedKeySet.find(slotToKeyMapping[slot]) != mappedKeySet.end())
				mappedKeySet.erase(slotToKeyMapping[slot]);
			if (slotToKeyMapping.find(slot) != slotToKeyMapping.end())
				slotToKeyMapping.erase(slot);
			out[slot] = false;
		} else {
			slotToKeyMapping[slot] = it.key();
			mappedKeySet.insert(it.key());
			out[slot] = true;
			filledBits[slot] = true;
		}
	}
	std::cout << std::endl;
	
	if (intervalSize == kvp.size() - mappedKeySet.size()) {
		// remove bits for unused level
		for (unsigned long i = 0; i < intervalSize; i++)
			out.pop_back();
	} else {
		// save level's size, and its seed
		if (levelSizes != nullptr)
		levelSizes->push_back(intervalSize);
		if (hashSeeds != nullptr)
			hashSeeds->push_back(depth);
	}
	PMHF_FromJson(out, kvp, mappedKeySet, levelSizes, hashSeeds, out.size(), depth + 1);
}

class PMHF
{
public:

	PMHF(json &kvp) {
		std::unordered_set<std::string> keySet;
		PMHF_FromJson(pmhfBitSet, kvp, keySet, &levelSizes, &hashSeeds);
	}

private:
	std::vector<int> levelSizes;
	std::vector<int> hashSeeds;
	std::vector<char *> keyValueData;
	std::vector<bool> pmhfBitSet;

	// #define PAGE_SIZE (4096) // 4KB pages
	// char serialData[PAGE_SIZE];
};


int main ()
{
	json kvp = {
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
		}},
		{"12345", nullptr}
	};
	// std::cout << kvp.dump(4) << std::endl;

	// create bitset for level
	std::vector<bool> bitset;
	std::unordered_set<std::string> keySet;

	PMHF_FromJson(bitset, kvp, keySet);

	std::cout << "Resulting bitset of size " << bitset.size() << std::endl;
	printBitset(bitset);
	

	// simple hash test
	// std::cout << "Result of hash: " << hash("randomString", 20) << std::endl;

	/*
	Raylib example file.
	This is an example main file for a simple raylib project.
	Use this as a starting point or replace it with your code.

	by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

	*/
	/*

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
	*/

	return 0;
}
