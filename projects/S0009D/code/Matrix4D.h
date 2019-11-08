#pragma once
#include "iostream"
#include <cmath>
#include "Vector4D.h"


class Matrix4D
	{
	public:
		Matrix4D(float a, float b, float c, float d, float e, float f, float g, float h, float i, float j, float k, float l, float m, float n, float o, float p);
		Matrix4D(const Matrix4D& inMatrix);
		Matrix4D(float inMatrix[16]);
		Matrix4D();
		~Matrix4D();
		Matrix4D operator*(const Matrix4D& inMatrix) const;
		Matrix4D operator*(const float scaler) const;
		Matrix4D operator+(const Matrix4D& inMatrix) const;
		float operator[](const int index) const;
		float& operator[](const int index);
		static Matrix4D transpose(const Matrix4D& inMatrix);
		void print();
		static Matrix4D rotX(float rotation);
		static Matrix4D rotY(float rotation);
		static Matrix4D rotZ(float rotation);
		Vector4D operator*(const Vector4D& inVector) const;
		static Matrix4D inverse(const Matrix4D& inMatrix);
		static Matrix4D rotVector(const Vector4D& inVector, const float rad);
		float* getMatrix();
		static Matrix4D getPositionMatrix(Vector4D& inVector);
		static Matrix4D getScaleMatrix(Vector4D& invector);
		static Matrix4D lookAt(Vector4D cameraPos, Vector4D target, Vector4D up);
		static Matrix4D rotationDir(const Vector4D& direction, const Vector4D& base = Vector4D(0, 0, 1, 1));

	private:
		float matrix[16];
};

	///Constructor for a 4x4 matrix that doesnt take any arguments. Creates a identity matrix.
	inline Matrix4D::Matrix4D() {
		matrix[0] = 1;
		matrix[1] = 0;
		matrix[2] = 0;
		matrix[3] = 0;

		matrix[4] = 0;
		matrix[5] = 1;
		matrix[6] = 0;
		matrix[7] = 0;

		matrix[8] = 0;
		matrix[9] = 0;
		matrix[10] = 1;
		matrix[11] = 0;

		matrix[12] = 0;
		matrix[13] = 0;
		matrix[14] = 0;
		matrix[15] = 1;
	}

	///Constructor for a 4x4 matrix that takes a float for every position in the matrix.
	inline Matrix4D::Matrix4D(float a, float b, float c, float d, float e, float f, float g, float h, float i, float j, float k, float l, float m, float n, float o, float p) {
		matrix[0] = a;
		matrix[1] = b;
		matrix[2] = c;
		matrix[3] = d;
		matrix[4] = e;
		matrix[5] = f;
		matrix[6] = g;
		matrix[7] = h;
		matrix[8] = i;
		matrix[9] = j;
		matrix[10] = k;
		matrix[11] = l;
		matrix[12] = m;
		matrix[13] = n;
		matrix[14] = o;
		matrix[15] = p;
	};

	///Constructor for a 4x4 matrix that copies the values from another 4x4 matrix.
	inline Matrix4D::Matrix4D(const Matrix4D& inMatrix) {
		matrix[0] = inMatrix.matrix[0];
		matrix[1] = inMatrix.matrix[1];
		matrix[2] = inMatrix.matrix[2];
		matrix[3] = inMatrix.matrix[3];
		matrix[4] = inMatrix.matrix[4];
		matrix[5] = inMatrix.matrix[5];
		matrix[6] = inMatrix.matrix[6];
		matrix[7] = inMatrix.matrix[7];
		matrix[8] = inMatrix.matrix[8];
		matrix[9] = inMatrix.matrix[9];
		matrix[10] = inMatrix.matrix[10];
		matrix[11] = inMatrix.matrix[11];
		matrix[12] = inMatrix.matrix[12];
		matrix[13] = inMatrix.matrix[13];
		matrix[14] = inMatrix.matrix[14];
		matrix[15] = inMatrix.matrix[15];
	}

	///Constructor for a 4x4 matrix that takes an array of floats with length 16 as argument.
	inline Matrix4D::Matrix4D(float inMatrix[16])
	{
		matrix[0] = inMatrix[0];
		matrix[1] = inMatrix[1];
		matrix[2] = inMatrix[2];
		matrix[3] = inMatrix[3];
		matrix[4] = inMatrix[4];
		matrix[5] = inMatrix[5];
		matrix[6] = inMatrix[6];
		matrix[7] = inMatrix[7];
		matrix[8] = inMatrix[8];
		matrix[9] = inMatrix[9];
		matrix[10] = inMatrix[10];
		matrix[11] = inMatrix[11];
		matrix[12] = inMatrix[12];
		matrix[13] = inMatrix[13];
		matrix[14] = inMatrix[14];
		matrix[15] = inMatrix[15];
	}

	///Deconstructor.
	inline Matrix4D::~Matrix4D()
	{

	}

	///Matrix multiplication with two matrixes (matrix * matrix).
	inline Matrix4D Matrix4D::operator*(const Matrix4D& inMatrix) const {

		float temparr[16];

		temparr[0] = matrix[0] * inMatrix.matrix[0] + matrix[1] * inMatrix.matrix[4] + matrix[2] * inMatrix.matrix[8] + matrix[3] * inMatrix.matrix[12];
		temparr[1] = matrix[0] * inMatrix.matrix[1] + matrix[1] * inMatrix.matrix[5] + matrix[2] * inMatrix.matrix[9] + matrix[3] * inMatrix.matrix[13];
		temparr[2] = matrix[0] * inMatrix.matrix[2] + matrix[1] * inMatrix.matrix[6] + matrix[2] * inMatrix.matrix[10] + matrix[3] * inMatrix.matrix[14];
		temparr[3] = matrix[0] * inMatrix.matrix[3] + matrix[1] * inMatrix.matrix[7] + matrix[2] * inMatrix.matrix[11] + matrix[3] * inMatrix.matrix[15];

		temparr[4] = matrix[4] * inMatrix.matrix[0] + matrix[5] * inMatrix.matrix[4] + matrix[6] * inMatrix.matrix[8] + matrix[7] * inMatrix.matrix[12];
		temparr[5] = matrix[4] * inMatrix.matrix[1] + matrix[5] * inMatrix.matrix[5] + matrix[6] * inMatrix.matrix[9] + matrix[7] * inMatrix.matrix[13];
		temparr[6] = matrix[4] * inMatrix.matrix[2] + matrix[5] * inMatrix.matrix[6] + matrix[6] * inMatrix.matrix[10] + matrix[7] * inMatrix.matrix[14];
		temparr[7] = matrix[4] * inMatrix.matrix[3] + matrix[5] * inMatrix.matrix[7] + matrix[6] * inMatrix.matrix[11] + matrix[7] * inMatrix.matrix[15];

		temparr[8] = matrix[8] * inMatrix.matrix[0] + matrix[9] * inMatrix.matrix[4] + matrix[10] * inMatrix.matrix[8] + matrix[11] * inMatrix.matrix[12];
		temparr[9] = matrix[8] * inMatrix.matrix[1] + matrix[9] * inMatrix.matrix[5] + matrix[10] * inMatrix.matrix[9] + matrix[11] * inMatrix.matrix[13];
		temparr[10] = matrix[8] * inMatrix.matrix[2] + matrix[9] * inMatrix.matrix[6] + matrix[10] * inMatrix.matrix[10] + matrix[11] * inMatrix.matrix[14];
		temparr[11] = matrix[8] * inMatrix.matrix[3] + matrix[9] * inMatrix.matrix[7] + matrix[10] * inMatrix.matrix[11] + matrix[11] * inMatrix.matrix[15];

		temparr[12] = matrix[12] * inMatrix.matrix[0] + matrix[13] * inMatrix.matrix[4] + matrix[14] * inMatrix.matrix[8] + matrix[15] * inMatrix.matrix[12];
		temparr[13] = matrix[12] * inMatrix.matrix[1] + matrix[13] * inMatrix.matrix[5] + matrix[14] * inMatrix.matrix[9] + matrix[15] * inMatrix.matrix[13];
		temparr[14] = matrix[12] * inMatrix.matrix[2] + matrix[13] * inMatrix.matrix[6] + matrix[14] * inMatrix.matrix[10] + matrix[15] * inMatrix.matrix[14];
		temparr[15] = matrix[12] * inMatrix.matrix[3] + matrix[13] * inMatrix.matrix[7] + matrix[14] * inMatrix.matrix[11] + matrix[15] * inMatrix.matrix[15];

		return Matrix4D(temparr);
	}

	///Matrix multiplication with a matrix and a constant number (matrix * nr).
	inline Matrix4D Matrix4D::operator*(const float scaler) const {
		float temparr[16];

		temparr[0] = matrix[0] * scaler;
		temparr[1] = matrix[1] * scaler;
		temparr[2] = matrix[2] * scaler;
		temparr[3] = matrix[3] * scaler;
		temparr[4] = matrix[4] * scaler;
		temparr[5] = matrix[5] * scaler;
		temparr[6] = matrix[6] * scaler;
		temparr[7] = matrix[7] * scaler;
		temparr[8] = matrix[8] * scaler;
		temparr[9] = matrix[9] * scaler;
		temparr[10] = matrix[10] * scaler;
		temparr[11] = matrix[11] * scaler;
		temparr[12] = matrix[12] * scaler;
		temparr[13] = matrix[13] * scaler;
		temparr[14] = matrix[14] * scaler;
		temparr[15] = matrix[15] * scaler;

		return Matrix4D(temparr);
	}

	///Matrix addition with another matrix (matrix + matrix)
	inline Matrix4D Matrix4D::operator+(const Matrix4D& inMatrix) const {
		float temparr[16];

		temparr[0] = matrix[0] + inMatrix.matrix[0];
		temparr[1] = matrix[1] + inMatrix.matrix[1];
		temparr[2] = matrix[2] + inMatrix.matrix[2];
		temparr[3] = matrix[3] + inMatrix.matrix[3];
		temparr[4] = matrix[4] + inMatrix.matrix[4];
		temparr[5] = matrix[5] + inMatrix.matrix[5];
		temparr[6] = matrix[6] + inMatrix.matrix[6];
		temparr[7] = matrix[7] + inMatrix.matrix[7];
		temparr[8] = matrix[8] + inMatrix.matrix[8];
		temparr[9] = matrix[9] + inMatrix.matrix[9];
		temparr[10] = matrix[10] + inMatrix.matrix[10];
		temparr[11] = matrix[11] + inMatrix.matrix[11];
		temparr[12] = matrix[12] + inMatrix.matrix[12];
		temparr[13] = matrix[13] + inMatrix.matrix[13];
		temparr[14] = matrix[14] + inMatrix.matrix[14];
		temparr[15] = matrix[15] + inMatrix.matrix[15];

		return Matrix4D(temparr);
	}

	///Gets a value from a certain position in the matrix.
	inline float Matrix4D::operator[](const int index) const {
		return matrix[index];
	}

	///Sets a value on a certain position in the matrix.
	inline float& Matrix4D::operator[](const int index) {
		return matrix[index];
	}

	///Prints the Matrix (Used for testing).
	inline void Matrix4D::print() {
		for (int i = 0; i < 16; i++)
		{
			std::cout << matrix[i] << " ";
			if (i % 4 == 3 && i != 0)
			{
				std::cout << "\n";
			}
		}
		std::cout << "\n";
	}

	///Returns a Matrix4D that is the transpose of the matrix taken as argument.
	inline Matrix4D Matrix4D::transpose(const Matrix4D& inMatrix) {
		float temparr[16];

		temparr[0] = inMatrix.matrix[0];
		temparr[1] = inMatrix.matrix[4];
		temparr[2] = inMatrix.matrix[8];
		temparr[3] = inMatrix.matrix[12];
		temparr[4] = inMatrix.matrix[1];
		temparr[5] = inMatrix.matrix[5];
		temparr[6] = inMatrix.matrix[9];
		temparr[7] = inMatrix.matrix[13];
		temparr[8] = inMatrix.matrix[2];
		temparr[9] = inMatrix.matrix[6];
		temparr[10] = inMatrix.matrix[10];
		temparr[11] = inMatrix.matrix[14];
		temparr[12] = inMatrix.matrix[3];
		temparr[13] = inMatrix.matrix[7];
		temparr[14] = inMatrix.matrix[11];
		temparr[15] = inMatrix.matrix[15];

		return Matrix4D(temparr);
	}

	///Returns a rotation matrix for the X-axis. Takes radians as argument.
	inline Matrix4D Matrix4D::rotX(float rotation)
	{
		float sinus = sinf(rotation);
		float cosinus = cosf(rotation);

		float temparr[16];

		temparr[0] = 1;
		temparr[1] = 0;
		temparr[2] = 0;
		temparr[3] = 0;

		temparr[4] = 0;
		temparr[5] = cosinus;
		temparr[6] = -sinus;
		temparr[7] = 0;

		temparr[8] = 0;
		temparr[9] = sinus;
		temparr[10] = cosinus;
		temparr[11] = 0;

		temparr[12] = 0;
		temparr[13] = 0;
		temparr[14] = 0;
		temparr[15] = 1;

		return(Matrix4D(temparr));
	}

	///Returns a rotation matrix for the Y-axis. Takes radians as argument.
	inline Matrix4D Matrix4D::rotY(float rotation)
	{
		float sinus = sinf(rotation);
		float cosinus = cosf(rotation);

		float temparr[16];

		temparr[0] = cosinus;
		temparr[1] = 0;
		temparr[2] = sinus;
		temparr[3] = 0;

		temparr[4] = 0;
		temparr[5] = 1;
		temparr[6] = 0;
		temparr[7] = 0;

		temparr[8] = -sinus;
		temparr[9] = 0;
		temparr[10] = cosinus;
		temparr[11] = 0;

		temparr[12] = 0;
		temparr[13] = 0;
		temparr[14] = 0;
		temparr[15] = 1;

		return(Matrix4D(temparr));
	}

	///Returns a rotation matrix for the Z-axis. Takes radians as argument.
	inline Matrix4D Matrix4D::rotZ(float rotation)
	{
		float sinus = sinf(rotation);
		float cosinus = cosf(rotation);

		float temparr[16];

		temparr[0] = cosinus;
		temparr[1] = -sinus;
		temparr[2] = 0;
		temparr[3] = 0;

		temparr[4] = sinus;
		temparr[5] = cosinus;
		temparr[6] = 0;
		temparr[7] = 0;

		temparr[8] = 0;
		temparr[9] = 0;
		temparr[10] = 1;
		temparr[11] = 0;

		temparr[12] = 0;
		temparr[13] = 0;
		temparr[14] = 0;
		temparr[15] = 1;

		return(Matrix4D(temparr));
	}

	///Multiplication with a Vector4D. Returns a Vector4D. (Matrix * Vector).
	inline Vector4D Matrix4D::operator*(const Vector4D& inVector) const {
		float p1 = matrix[0] * inVector.getFloat(0) + matrix[1] * inVector.getFloat(1) + matrix[2] * inVector.getFloat(2) + matrix[3] * inVector.getFloat(3);
		float p2 = matrix[4] * inVector.getFloat(0) + matrix[5] * inVector.getFloat(1) + matrix[6] * inVector.getFloat(2) + matrix[7] * inVector.getFloat(3);
		float p3 = matrix[8] * inVector.getFloat(0) + matrix[9] * inVector.getFloat(1) + matrix[10] * inVector.getFloat(2) + matrix[11] * inVector.getFloat(3);
		float p4 = matrix[12] * inVector.getFloat(0) + matrix[12] * inVector.getFloat(1) + matrix[14] * inVector.getFloat(2) + matrix[15] * inVector.getFloat(3);

		return Vector4D(p1, p2, p3, p4);
	}

	///Returns a Matrix4D that is the reverse of the matrix sent as an argument.
	inline Matrix4D Matrix4D::inverse(const Matrix4D& inMatrix) {
		double inv[16], det;
		float invOut[16];
		int i;

		inv[0] = inMatrix.matrix[5] * inMatrix.matrix[10] * inMatrix.matrix[15] -
			inMatrix.matrix[5] * inMatrix.matrix[11] * inMatrix.matrix[14] -
			inMatrix.matrix[9] * inMatrix.matrix[6] * inMatrix.matrix[15] +
			inMatrix.matrix[9] * inMatrix.matrix[7] * inMatrix.matrix[14] +
			inMatrix.matrix[13] * inMatrix.matrix[6] * inMatrix.matrix[11] -
			inMatrix.matrix[13] * inMatrix.matrix[7] * inMatrix.matrix[10];

		inv[4] = -inMatrix.matrix[4] * inMatrix.matrix[10] * inMatrix.matrix[15] +
			inMatrix.matrix[4] * inMatrix.matrix[11] * inMatrix.matrix[14] +
			inMatrix.matrix[8] * inMatrix.matrix[6] * inMatrix.matrix[15] -
			inMatrix.matrix[8] * inMatrix.matrix[7] * inMatrix.matrix[14] -
			inMatrix.matrix[12] * inMatrix.matrix[6] * inMatrix.matrix[11] +
			inMatrix.matrix[12] * inMatrix.matrix[7] * inMatrix.matrix[10];

		inv[8] = inMatrix.matrix[4] * inMatrix.matrix[9] * inMatrix.matrix[15] -
			inMatrix.matrix[4] * inMatrix.matrix[11] * inMatrix.matrix[13] -
			inMatrix.matrix[8] * inMatrix.matrix[5] * inMatrix.matrix[15] +
			inMatrix.matrix[8] * inMatrix.matrix[7] * inMatrix.matrix[13] +
			inMatrix.matrix[12] * inMatrix.matrix[5] * inMatrix.matrix[11] -
			inMatrix.matrix[12] * inMatrix.matrix[7] * inMatrix.matrix[9];

		inv[12] = -inMatrix.matrix[4] * inMatrix.matrix[9] * inMatrix.matrix[14] +
			inMatrix.matrix[4] * inMatrix.matrix[10] * inMatrix.matrix[13] +
			inMatrix.matrix[8] * inMatrix.matrix[5] * inMatrix.matrix[14] -
			inMatrix.matrix[8] * inMatrix.matrix[6] * inMatrix.matrix[13] -
			inMatrix.matrix[12] * inMatrix.matrix[5] * inMatrix.matrix[10] +
			inMatrix.matrix[12] * inMatrix.matrix[6] * inMatrix.matrix[9];

		inv[1] = -inMatrix.matrix[1] * inMatrix.matrix[10] * inMatrix.matrix[15] +
			inMatrix.matrix[1] * inMatrix.matrix[11] * inMatrix.matrix[14] +
			inMatrix.matrix[9] * inMatrix.matrix[2] * inMatrix.matrix[15] -
			inMatrix.matrix[9] * inMatrix.matrix[3] * inMatrix.matrix[14] -
			inMatrix.matrix[13] * inMatrix.matrix[2] * inMatrix.matrix[11] +
			inMatrix.matrix[13] * inMatrix.matrix[3] * inMatrix.matrix[10];

		inv[5] = inMatrix.matrix[0] * inMatrix.matrix[10] * inMatrix.matrix[15] -
			inMatrix.matrix[0] * inMatrix.matrix[11] * inMatrix.matrix[14] -
			inMatrix.matrix[8] * inMatrix.matrix[2] * inMatrix.matrix[15] +
			inMatrix.matrix[8] * inMatrix.matrix[3] * inMatrix.matrix[14] +
			inMatrix.matrix[12] * inMatrix.matrix[2] * inMatrix.matrix[11] -
			inMatrix.matrix[12] * inMatrix.matrix[3] * inMatrix.matrix[10];

		inv[9] = -inMatrix.matrix[0] * inMatrix.matrix[9] * inMatrix.matrix[15] +
			inMatrix.matrix[0] * inMatrix.matrix[11] * inMatrix.matrix[13] +
			inMatrix.matrix[8] * inMatrix.matrix[1] * inMatrix.matrix[15] -
			inMatrix.matrix[8] * inMatrix.matrix[3] * inMatrix.matrix[13] -
			inMatrix.matrix[12] * inMatrix.matrix[1] * inMatrix.matrix[11] +
			inMatrix.matrix[12] * inMatrix.matrix[3] * inMatrix.matrix[9];

		inv[13] = inMatrix.matrix[0] * inMatrix.matrix[9] * inMatrix.matrix[14] -
			inMatrix.matrix[0] * inMatrix.matrix[10] * inMatrix.matrix[13] -
			inMatrix.matrix[8] * inMatrix.matrix[1] * inMatrix.matrix[14] +
			inMatrix.matrix[8] * inMatrix.matrix[2] * inMatrix.matrix[13] +
			inMatrix.matrix[12] * inMatrix.matrix[1] * inMatrix.matrix[10] -
			inMatrix.matrix[12] * inMatrix.matrix[2] * inMatrix.matrix[9];

		inv[2] = inMatrix.matrix[1] * inMatrix.matrix[6] * inMatrix.matrix[15] -
			inMatrix.matrix[1] * inMatrix.matrix[7] * inMatrix.matrix[14] -
			inMatrix.matrix[5] * inMatrix.matrix[2] * inMatrix.matrix[15] +
			inMatrix.matrix[5] * inMatrix.matrix[3] * inMatrix.matrix[14] +
			inMatrix.matrix[13] * inMatrix.matrix[2] * inMatrix.matrix[7] -
			inMatrix.matrix[13] * inMatrix.matrix[3] * inMatrix.matrix[6];

		inv[6] = -inMatrix.matrix[0] * inMatrix.matrix[6] * inMatrix.matrix[15] +
			inMatrix.matrix[0] * inMatrix.matrix[7] * inMatrix.matrix[14] +
			inMatrix.matrix[4] * inMatrix.matrix[2] * inMatrix.matrix[15] -
			inMatrix.matrix[4] * inMatrix.matrix[3] * inMatrix.matrix[14] -
			inMatrix.matrix[12] * inMatrix.matrix[2] * inMatrix.matrix[7] +
			inMatrix.matrix[12] * inMatrix.matrix[3] * inMatrix.matrix[6];

		inv[10] = inMatrix.matrix[0] * inMatrix.matrix[5] * inMatrix.matrix[15] -
			inMatrix.matrix[0] * inMatrix.matrix[7] * inMatrix.matrix[13] -
			inMatrix.matrix[4] * inMatrix.matrix[1] * inMatrix.matrix[15] +
			inMatrix.matrix[4] * inMatrix.matrix[3] * inMatrix.matrix[13] +
			inMatrix.matrix[12] * inMatrix.matrix[1] * inMatrix.matrix[7] -
			inMatrix.matrix[12] * inMatrix.matrix[3] * inMatrix.matrix[5];

		inv[14] = -inMatrix.matrix[0] * inMatrix.matrix[5] * inMatrix.matrix[14] +
			inMatrix.matrix[0] * inMatrix.matrix[6] * inMatrix.matrix[13] +
			inMatrix.matrix[4] * inMatrix.matrix[1] * inMatrix.matrix[14] -
			inMatrix.matrix[4] * inMatrix.matrix[2] * inMatrix.matrix[13] -
			inMatrix.matrix[12] * inMatrix.matrix[1] * inMatrix.matrix[6] +
			inMatrix.matrix[12] * inMatrix.matrix[2] * inMatrix.matrix[5];

		inv[3] = -inMatrix.matrix[1] * inMatrix.matrix[6] * inMatrix.matrix[11] +
			inMatrix.matrix[1] * inMatrix.matrix[7] * inMatrix.matrix[10] +
			inMatrix.matrix[5] * inMatrix.matrix[2] * inMatrix.matrix[11] -
			inMatrix.matrix[5] * inMatrix.matrix[3] * inMatrix.matrix[10] -
			inMatrix.matrix[9] * inMatrix.matrix[2] * inMatrix.matrix[7] +
			inMatrix.matrix[9] * inMatrix.matrix[3] * inMatrix.matrix[6];

		inv[7] = inMatrix.matrix[0] * inMatrix.matrix[6] * inMatrix.matrix[11] -
			inMatrix.matrix[0] * inMatrix.matrix[7] * inMatrix.matrix[10] -
			inMatrix.matrix[4] * inMatrix.matrix[2] * inMatrix.matrix[11] +
			inMatrix.matrix[4] * inMatrix.matrix[3] * inMatrix.matrix[10] +
			inMatrix.matrix[8] * inMatrix.matrix[2] * inMatrix.matrix[7] -
			inMatrix.matrix[8] * inMatrix.matrix[3] * inMatrix.matrix[6];

		inv[11] = -inMatrix.matrix[0] * inMatrix.matrix[5] * inMatrix.matrix[11] +
			inMatrix.matrix[0] * inMatrix.matrix[7] * inMatrix.matrix[9] +
			inMatrix.matrix[4] * inMatrix.matrix[1] * inMatrix.matrix[11] -
			inMatrix.matrix[4] * inMatrix.matrix[3] * inMatrix.matrix[9] -
			inMatrix.matrix[8] * inMatrix.matrix[1] * inMatrix.matrix[7] +
			inMatrix.matrix[8] * inMatrix.matrix[3] * inMatrix.matrix[5];

		inv[15] = inMatrix.matrix[0] * inMatrix.matrix[5] * inMatrix.matrix[10] -
			inMatrix.matrix[0] * inMatrix.matrix[6] * inMatrix.matrix[9] -
			inMatrix.matrix[4] * inMatrix.matrix[1] * inMatrix.matrix[10] +
			inMatrix.matrix[4] * inMatrix.matrix[2] * inMatrix.matrix[9] +
			inMatrix.matrix[8] * inMatrix.matrix[1] * inMatrix.matrix[6] -
			inMatrix.matrix[8] * inMatrix.matrix[2] * inMatrix.matrix[5];

		det = inMatrix.matrix[0] * inv[0] + inMatrix.matrix[1] * inv[4] + inMatrix.matrix[2] * inv[8] + inMatrix.matrix[3] * inv[12];

		if (det == 0)
			throw "Matrix does not have an inverse";

		det = 1.0 / det;

		for (i = 0; i < 16; i++)
			invOut[i] = inv[i] * det;

		return Matrix4D(invOut);
	}

	///Returns a rotation matrix that rotates around a vector.
	inline Matrix4D Matrix4D::rotVector(const Vector4D& inVector, const float rad) {
		Vector4D normVector = inVector.normalize();
		Matrix4D C(0, -normVector.getFloat(2), normVector.getFloat(1), 0,
			normVector.getFloat(2), 0, -normVector.getFloat(0), 0,
			-normVector.getFloat(1), normVector.getFloat(0), 0, 0,
			0, 0, 0, 1);
		Matrix4D I;

		Matrix4D rot = I + (C * sin(rad)) + (C * C)*(1 - cos(rad));
		rot.matrix[15] = 1;

		return rot;
	}

	inline float * Matrix4D::getMatrix()
	{
		return matrix;
	}

	inline Matrix4D Matrix4D::getPositionMatrix(Vector4D& inVector) {
		float temparr[16];

		temparr[0] = 1;
		temparr[1] = 0;
		temparr[2] = 0;
		temparr[3] = inVector.getFloat(0);

		temparr[4] = 0;
		temparr[5] = 1;
		temparr[6] = 0;
		temparr[7] = inVector.getFloat(1);

		temparr[8] = 0;
		temparr[9] = 0;
		temparr[10] = 1;
		temparr[11] = inVector.getFloat(2);

		temparr[12] = 0;
		temparr[13] = 0;
		temparr[14] = 0;
		temparr[15] = 1;

		return Matrix4D(temparr);
	}

	inline Matrix4D Matrix4D::getScaleMatrix(Vector4D& inVector)
	{
		float temparr[16];

		temparr[0] = inVector.getFloat(0);
		temparr[1] = 0;
		temparr[2] = 0;
		temparr[3] = 0;

		temparr[4] = 0;
		temparr[5] = inVector.getFloat(1);
		temparr[6] = 0;
		temparr[7] = 0;

		temparr[8] = 0;
		temparr[9] = 0;
		temparr[10] = inVector.getFloat(2);
		temparr[11] = 0;

		temparr[12] = 0;
		temparr[13] = 0;
		temparr[14] = 0;
		temparr[15] = 1;

		return Matrix4D(temparr);
	}

	inline Matrix4D Matrix4D::lookAt(Vector4D cameraPosition, Vector4D cameraTarget, Vector4D cameraUp) {

		Vector4D f = (cameraTarget - cameraPosition);
		Vector4D u = cameraUp.normalize();
		Vector4D s = (f.crossProduct(u)).normalize();
		u = s.crossProduct(f);
		
		Matrix4D lookAt = Matrix4D(s.getFloat(0), s.getFloat(1), s.getFloat(2), -(s.dotProduct(cameraPosition)),
								   u.getFloat(0), u.getFloat(1), u.getFloat(2), -(u.dotProduct(cameraPosition)),
								  -f.getFloat(0), -f.getFloat(1), -f.getFloat(2), f.dotProduct(cameraPosition),
								   0, 0, 0,1
		);

		/*Matrix4D lookAt = Matrix4D(s.getFloat(0), u.getFloat(0),  f.getFloat(0), 0,
									s.getFloat(1), u.getFloat(1), f.getFloat(1), 0,
									s.getFloat(2), u.getFloat(2), f.getFloat(2), 0,
									-(s.dotProduct(cameraPosition)), -(u.dotProduct(cameraPosition)), -(f.dotProduct(cameraPosition)), 1
		);*/

		return lookAt;
	}

	inline Matrix4D Matrix4D::rotationDir(const Vector4D& direction, const Vector4D& base)
	{
		Matrix4D rot;
		if (direction[0] == base[0] && direction[1] == base[1] && direction[2] == base[2])
			return rot;
		
		Vector4D invertedZ = direction;
		invertedZ[2] *= -1;
		invertedZ = invertedZ.normalize();

		Vector4D V = base.crossProduct(invertedZ);
		V = V.normalize();

		float phi = acos(base.dotProduct(invertedZ));
		float rcos = cos(phi);
		float rsin = sin(phi);

		rot[0] = 		 rcos + V[0] * V[0] * (1.0 - rcos);
		rot[1] =  V[2] * rsin + V[1] * V[0] * (1.0 - rcos);
		rot[2] = -V[1] * rsin + V[2] * V[0] * (1.0 - rcos);
		rot[3] = 0.0f;
		
		rot[4] = -V[2] * rsin + V[0] * V[1] * (1.0 - rcos);
		rot[5] = 		 rcos + V[1] * V[1] * (1.0 - rcos);
		rot[6] = -V[0] * rsin + V[2] * V[1] * (1.0 - rcos);
		rot[7] = 0.0f;

		rot[8] =   V[1] * rsin + V[0] * V[2] * (1.0 - rcos);
		rot[9] =  -V[0] * rsin + V[1] * V[2] * (1.0 - rcos);
		rot[10] = 		  rcos + V[2] * V[2] * (1.0 - rcos);
		rot[11] = 0.0f;

		rot[12] = 0.0f;
		rot[13] = 0.0f;
		rot[14] = 0.0f;
		rot[15] = 1.0f;

		return rot;
	}
