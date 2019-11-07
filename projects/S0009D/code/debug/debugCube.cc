#include "debugCube.h"

DebugCube::DebugCube(){}

DebugCube::DebugCube(Vector4D pos, float width, float height, float length)
{
    float w = width/2;
    float h = height/2;
    float l = length/2;

    vertices.push_back(pos[0]+w);    vertices.push_back(pos[1]+h);    vertices.push_back(pos[2]+l);
    vertices.push_back(pos[0]+w);    vertices.push_back(pos[1]+h);    vertices.push_back(pos[2]-l);
    vertices.push_back(pos[0]-w);    vertices.push_back(pos[1]+h);    vertices.push_back(pos[2]-l);
    vertices.push_back(pos[0]-w);    vertices.push_back(pos[1]+h);    vertices.push_back(pos[2]+l);

    vertices.push_back(pos[0]+w);    vertices.push_back(pos[1]-h);    vertices.push_back(pos[2]+l);
    vertices.push_back(pos[0]+w);    vertices.push_back(pos[1]-h);    vertices.push_back(pos[2]-l);
    vertices.push_back(pos[0]-w);    vertices.push_back(pos[1]-h);    vertices.push_back(pos[2]-l);
    vertices.push_back(pos[0]-w);    vertices.push_back(pos[1]-h);    vertices.push_back(pos[2]+l);

    indices.push_back(0);    indices.push_back(1);    indices.push_back(2);
    indices.push_back(0);    indices.push_back(2);    indices.push_back(3);

    indices.push_back(0);    indices.push_back(4);    indices.push_back(5);
    indices.push_back(0);    indices.push_back(1);    indices.push_back(5);

    indices.push_back(1);    indices.push_back(5);    indices.push_back(6);
    indices.push_back(1);    indices.push_back(2);    indices.push_back(6);

    indices.push_back(2);    indices.push_back(3);    indices.push_back(6);
    indices.push_back(6);    indices.push_back(7);    indices.push_back(3);

    indices.push_back(0);    indices.push_back(3);    indices.push_back(7);
    indices.push_back(0);    indices.push_back(4);    indices.push_back(7);

    indices.push_back(4);    indices.push_back(5);    indices.push_back(6);
    indices.push_back(4);    indices.push_back(6);    indices.push_back(7);
    
}

void DebugCube::draw(Matrix4D viewMatrix){}

void DebugCube::setup(){}