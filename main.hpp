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
#define TEMP_RADIUS 7
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
void collisonScorePenalty(float &score, Vector2 &entityA, Vector2 &entityB);

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

struct playerDistances;
void updatePlayerDistances(playerDistances &pds,float &p1, float &p2, float &cpu);
void sectorRegulation(Vector2& player, float playerDistance, float &score, const float sectorInnerRadius, const float sectorOuterRadius, const float sectorScore, Vector2 &centerScreenVector);

struct playersPositions;

/*
#pragma once
#include <raylib.h>

#define FPS 60
#define OUTER_RING_RADIUS 200
#define SECTOR_A_RADIUS 150
#define SECTOR_B_RADIUS 100
#define SECTOR_C_RADIUS 50
#define FINAL_SECTOR_RADIUS 50
#define ORIGIN 0
#define TEMP_RADIUS 7

enum SectorID { SECTOR_1, SECTOR_2, SECTOR_3, SECTOR_4 };

struct Player {
    Vector2 position;
    Color color;
    float score;
};

struct Sector {
    float innerRadius;
    float outerRadius;
    float requiredScore;
};

void playerMove(Player &player, bool useArrowKeys);
void cpuMove(Player &cpu, const Vector2 &screenCenter, float radius, float &angle, float speed);
void updatePlayerColor(Player &player, const Vector2 &screenCenter);
float calculateDistance(const Vector2 &a, const Vector2 &b);
bool inRing(const Vector2 &point, const Vector2 &center, float innerRadius, float outerRadius);
bool canMove(const Player &player, const Sector &sector);
void enforceBoundary(Player &player, const Vector2 &center, float boundaryRadius);
void handleCollision(Player &a, Player &b);
void updateScore(Player &player, const Player &other);
void sectorRegulation(Player &player, const Sector &sector, const Vector2 &centerScreenVector);

// Add other necessary function prototypes here

*/

/*
Code Structure
Move constant definitions from the main file to the header file for better organization.
Create a Player struct to encapsulate player-related data and functions.
Implement a Sector struct to manage sector-specific properties and logic.
Function Improvements
Rename fplayerVectorDistance to calculateDistance for clarity.
Combine playerMove and playerMoveArrow into a single function with a parameter for key set.
Replace bodyCollision with a more physics-based collision response.
Optimization
Use Vector2Add and Vector2Subtract for vector operations instead of manual calculations.
Replace sqrtf with Vector2Distance for distance calculations.
Code Clarity
Use enums for sector identifiers instead of magic numbers.
Implement a state machine for game flow management.
Additional Features
Add a simple menu system for game start and restart.
Implement a scoring system that updates based on player positions and collisions.
*/