#include "Vector.h"

struct Body{
	
	float m;
	float r;
	Vector pos;
	Vector vel;
	Vector acc;

	Body(float m, float r) : r(r), m(m){
		pos = 0;
		vel = 0;
		acc = 0;
	}
	
	void setPos(float x, float y){
		pos.x = x;
		pos.y = y;
	}
	
	void setVel(float x, float y){
		vel.x = x;
		vel.y = y;
	}
	
	void PulledBy(const Body &OtherBody){
		const float G = 1.0f;
		float dist = sqrt((pos - OtherBody.pos) * (pos - OtherBody.pos));
		acc += G * OtherBody.m * (OtherBody.pos-pos) / dist / dist / dist;
	}
	
	void Update(float dt){
		vel += dt * acc;
		pos += dt * vel;
		acc = 0;
	}
	
};
