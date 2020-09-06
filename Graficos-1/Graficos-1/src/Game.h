#ifndef GAME_H
#define GAME_H

#include "BaseGame.h"

class Game : public BaseGame
{
private:
	Entity3D* m;
	Entity3D* m2;
	Entity3D* m3;
	Entity3D* m4;
	Entity3D* movableEntity;
public:
	bool OnStart();
	bool OnStop();
	bool OnUpdate();
	void OnDraw();
};

#endif
