#pragma once

class Input
{
public:
	enum Actions : unsigned char {
		UP = 0,
		LEFT,
		DOWN,
		RIGHT,
		
		F11,
		ESC,

		COUNT
	};

	static void initialize();
	static void finalize();
	static void update();
	static bool key[Actions::COUNT];
	static bool keyPressed[Actions::COUNT];
	static bool keyReleased[Actions::COUNT];
};
