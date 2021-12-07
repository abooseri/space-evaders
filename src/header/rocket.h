#ifndef ROCKET_H
#define ROCKET_H

class Rocket{
	public:
		Rocket(float x, float y);
		Rocket();
		void displayRocket();
		bool getComplete();
		int getX();
		int getY();
		void setComplete(bool complete);
		void isComplete();
	private:
		float posX;
		float posY;
		bool touched;
		Texture2D rocketTexture;
	};

#endif
