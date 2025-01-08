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
#define STARTING_SCORE 0

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
//void cpuMove(Vector2 &p);
void colorPlayer(Color &playerColor, Vector2 &playerPOS, Vector2 &screenHalfVector);

float fplayerVectorDistance(Vector2 &playerPOS, Vector2 &vec);
bool inRing(Vector2 &pointVector, Vector2 &ringVector, float innerRadius, float outerRadius);
bool canMove(float &distance, float &score, float sectorScore, int innerRadius, int outerRadius);
// maybe radius but may just include that in a vector or struct later
bool bbodyCollision(Vector2 &vecA, Vector2 &vecB, float distance);

void enforceBoundary(Player &player, const Vector2 &vec, float boundaryRadius);
void twoGateCollision(int outerGateRadius, int innerGateRadius, Player &player, Vector2 &screenCenter);
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
void sectorRegulation(Player &player, float playerDistance, float &score, const float sectorInnerRadius, const float sectorOuterRadius, const float sectorScore, Vector2 &centerScreenVector);

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