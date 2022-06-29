#include <cmath>

struct Vector{
	
	float x, y;
	
	Vector(){
		x = 0.0f;
		y = 0.0f;
	}
	
	Vector(float x, float y){
		this -> x = x;
		this -> y = y;
	}
	
	Vector(float k){		
		x = k;
		y = k;
	}
	
	Vector operator + (const Vector &v) const{
		return Vector(x + v.x, y + v.y);
	}
	
	Vector operator - (const Vector &v) const{
		return Vector(x - v.x, y - v.y);
	}
	
	float operator * (const Vector &v) const{
		return x * v.x + y * v.y;
	}
	
	Vector operator * (float k) const{
		return Vector(k * x, k * y);
	}
	
	Vector operator / (float k) const{
		return Vector(x / k, y / k);
	}
	
	void operator += (const Vector &v){
		x += v.x;
		y += v.y;
	}
	
	void operator -= (const Vector &v){
		x -= v.x;
		y -= v.y;
	}
	
	void operator *= (float k){
		x *= k;
		y *= k;
	}
	
	void operator /= (float k){
		x /= k;
		y /= k;
	}
	
	Vector operator - (){
		return Vector(-x, -y);
	}

};

Vector operator * (float k, const Vector &v){
	return v * k;
}
