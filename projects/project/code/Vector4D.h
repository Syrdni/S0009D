#pragma once
#include <cmath>
#include <iostream>
//#include "Matrix4D.h"

class Vector4D
	{
		friend class Matrix4D;
	public:
		Vector4D();
		Vector4D(float a, float b, float c, float d);
		~Vector4D();
		Vector4D operator+(const Vector4D& inVector) const;
		Vector4D operator-(const Vector4D& inVector) const;
		Vector4D operator*(const float scaler) const;
		float operator[](const int index) const;
		float& operator[](const int index);
		float length() const;
		Vector4D normalize() const;
		float dotProduct(const Vector4D& inVector) const;
		Vector4D crossProduct(const Vector4D& inVector) const;
		void print();
		float getFloat(int i) const;
		float* getVector();

	private:
		float vector[4];
		float toPos(float value) const;
	};

	///Constructor for an identity vector (0,0,0,1).
	inline Vector4D::Vector4D() {
		vector[0] = 0;
		vector[1] = 0;
		vector[2] = 0;
		vector[3] = 1;
	}

	///Constructor for a vector that takes 3 floats as an argument (X,Y,Z). W is always 1.
	inline Vector4D::Vector4D(float a, float b, float c, float d) {
		vector[0] = a;
		vector[1] = b;
		vector[2] = c;
		vector[3] = 1;//d;
	}

	///Deconstructor.
	inline Vector4D::~Vector4D() {

	}

	///Addition for vector + vector.
	inline Vector4D Vector4D::operator+(const Vector4D& inVector) const {
		float p1 = vector[0] + inVector.vector[0];
		float p2 = vector[1] + inVector.vector[1];
		float p3 = vector[2] + inVector.vector[2];
		float p4 = 1;//vector[3] + inVector.vector[3];

		return Vector4D(p1, p2, p3, p4);
	}

	///Subtraction for vector - vector.
	inline Vector4D Vector4D::operator-(const Vector4D& inVector) const {
		float p1 = vector[0] - inVector.vector[0];
		float p2 = vector[1] - inVector.vector[1];
		float p3 = vector[2] - inVector.vector[2];
		float p4 = 1;//vector[3] - inVector.vector[3];

		return Vector4D(p1, p2, p3, p4);
	}

	///Multiplication for vector * constant number.
	inline Vector4D Vector4D::operator*(const float scaler) const {
		float p1 = vector[0] * scaler;
		float p2 = vector[1] * scaler;
		float p3 = vector[2] * scaler;
		float p4 = 1;// vector[3] * scaler;

		return Vector4D(p1, p2, p3, p4);
	}

	///Gets a value from a certain position in the vector.
	inline float Vector4D::operator[](const int index) const {
		return vector[index];
	}

	///Sets a value from a certain position in the vector.
	inline float & Vector4D::operator[](const int index) {
		return vector[index];
	}

	///Returns the length of the vector.
	inline float Vector4D::length() const {
		return sqrt(pow(Vector4D::toPos(vector[0]), 2) + pow(Vector4D::toPos(vector[1]), 2) + pow(Vector4D::toPos(vector[2]), 2));
	}

	///Normalizes the vector so the length is 1.
	inline Vector4D Vector4D::normalize() const {
		float length = this->length();

		float p1 = vector[0] / length;
		float p2 = vector[1] / length;
		float p3 = vector[2] / length;
		float p4 = 1;// vector[3] / length;

		return Vector4D(p1, p2, p3, p4);
	}

	///Returns a float which is the dot product of two vectors.
	inline float Vector4D::dotProduct(const Vector4D& inVector) const {
		return(vector[0] * inVector.vector[0] +
			vector[1] * inVector.vector[1] +
			vector[2] * inVector.vector[2]);
	}

	///Returns a vector that is the cross product of two vectors.
	inline Vector4D Vector4D::crossProduct(const Vector4D& inVector) const {
		return Vector4D((vector[1] * inVector.vector[2]) - (vector[2] * inVector.vector[1]),
			(vector[2] * inVector.vector[0]) - (vector[0] * inVector.vector[2]),
			(vector[0] * inVector.vector[1]) - (vector[1] * inVector.vector[0]),
				1);

		/*(Vector4D((vector[1] * inVector.vector[2]) - (vector[2] * inVector.vector[1]),
			(vector[2] * inVector.vector[3]) - (vector[3] * inVector.vector[2]),
			(vector[3] * inVector.vector[0]) - (vector[0] * inVector.vector[3]),
			(vector[0] * inVector.vector[1]) - (vector[1] * inVector.vector[0])));*/
	}

	///Prints the vector (Used for testing).
	inline void Vector4D::print() {
		std::cout << vector[0] << "\n" << vector[1] << "\n" << vector[2] << "\n" << vector[3] << "\n\n";
	}

	inline float Vector4D::getFloat(int i) const {
		return vector[i];
	}

	inline float* Vector4D::getVector() {
		return vector;
	}

	inline float Vector4D::toPos(float value) const{
		if (value < 0)
		{
			return -value;
		}
		else
		{
			return value;
		}
	}
