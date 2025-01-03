#pragma once
#include <raylib.h>
#include <string>

#define FPS 60
#define OUTER_RING_RADIUS 200
#define SECTOR_A_RADIUS 150
#define SECTOR_B_RADIUS 100
#define SECTOR_C_RADIUS 50
#define FINAL_SECTOR_RADIUS 50
#define ORIGIN 0
// #include <vector>

void playerMove(Vector2 &p);
//void cpuMove(Vector2 &p);
void colorPlayer(Color &playerColor, Vector2 &playerPOS, Vector2 &screenHalfVector);

float fplayerVectorDistance(Vector2 &playerPOS, Vector2 &vec);
bool inRing(Vector2 &pointVector, Vector2 &ringVector, float innerRadius, float outerRadius);
bool canMove(float &distance, float &score, float sectorScore, int innerRadius, int outerRadius);
// maybe radius but may just include that in a vector or struct later
bool bbodyCollision(Vector2 &vecA, Vector2 &vecB, float distance);

void enforceBoundary(Vector2 &playerPOS, Vector2 &vec, float boundaryRadius);
void twoGateCollision(int outerGateRadius, int innerGateRadius, Vector2 &playerPOS, Vector2 &screenHalfVector);
void cpuMove(Vector2 &p2, const Vector2 &screenCenter, float radius, float &angle, float speed);
void bodyCollision(Vector2 &vecA, Vector2 &vecB, float force);

// Basic unit testing for the functions
void testPlayerMove();
void testColorPlayer();
void testFplayerVectorDistance();
void testInRing();
void testCanMove();
void testBbodyCollision();
void testEnforceBoundary();
void testTwoGateCollision();
void testCpuMove();



