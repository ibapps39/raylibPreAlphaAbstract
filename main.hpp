#include <raylib.h>
#include <string>
// #include <vector>

void playerMove(Vector2 &p);
void colorPlayer(Color &playerColor, Vector2 &playerPOS, Vector2 &screenHalfVector);

float fplayerVectorDistance(Vector2 &playerPOS, Vector2 &vec);

bool CheckInPointRing(Vector2 &pointVector, Vector2 &ringVector, float innerRadius, float outerRadius);
bool CheckCollisionPointRing(Vector2 &pointVector, Vector2 &ringVector, float innerRadius, float outerRadius);
bool canMove(float &distance, float &score, float sectorScore, int innerRadius, int outerRadius);
void enforceBoundary(Vector2 &playerPOS, Vector2 &vec, float boundaryRadius);