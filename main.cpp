#include "main.hpp"

void playerMove(Vector2 &p)
{
    if (IsKeyDown(KEY_D))
        p.x += 2.0f;
    if (IsKeyDown(KEY_A))
        p.x -= 2.0f;
    if (IsKeyDown(KEY_W))
        p.y -= 2.0f;
    if (IsKeyDown(KEY_S))
        p.y += 2.0f;
}
bool canMove(float &distance, float &score, float sectorScore, int innerRadius, int outerRadius)
{
    return score < sectorScore && distance > innerRadius && distance < outerRadius;
}
float fplayerVectorDistance(Vector2 &playerPOS, Vector2 &vec)
{
    // d = √[(x2 - x1)2 + (y2 - y1)2]
    return sqrtf((powf((playerPOS.x - vec.x), 2.0f)) + (powf((playerPOS.y - vec.y), 2.0f)));
}
// We know the distance, need to offset player vector to position outside of boudnary
void enforceBoundary(Vector2 &playerPOS, Vector2 &vec, float boundaryRadius)
{
    // New Vector Components. -playerPOS so we go (-)opposite direction of offending direction <vec.x, vec.y>
    float dirX = vec.x - playerPOS.x;
    float dirY = vec.y - playerPOS.y;
    // New Vector Length
    float dirLen = sqrt(dirX * dirX + dirY * dirY);
    // Normalize, like getting a common denom./conextualizing, conforming the vector transformations uniformally.
    float normDirX = dirX / dirLen;
    float normDirY = dirY / dirLen;
    // offset by the length
    float offsetX = normDirX * boundaryRadius;
    float offsetY = normDirY * boundaryRadius;

    // PUT THE PLAYA THEIR DAH PLACE
    playerPOS.x = vec.x - offsetX;
    playerPOS.y = vec.y - offsetY;
}

bool CheckInPointRing(Vector2 &pointVector, Vector2 &ringVector, float innerRadius, float outerRadius)
{
    float distance = fplayerVectorDistance(pointVector, ringVector);
    return (distance <= outerRadius) && (distance >= innerRadius);
}
bool CheckCollisionPointRing(Vector2 &pointVector, Vector2 &ringVector, float innerRadius, float outerRadius)
{
    float distance = fplayerVectorDistance(pointVector, ringVector);
    return (distance == outerRadius) || (distance == innerRadius);
}
void colorPlayer(Color &playerColor, Vector2 &playerPOS, Vector2 &screenHalfVector)
{
    bool inSector1 = CheckInPointRing(playerPOS, screenHalfVector, 150, 200);
    bool inSector2 = CheckInPointRing(playerPOS, screenHalfVector, 100, 150);
    bool inSector3 = CheckInPointRing(playerPOS, screenHalfVector, 50, 100);
    bool inSector4 = CheckInPointRing(playerPOS, screenHalfVector, 0, 50);

    if (inSector1)
        playerColor = YELLOW;
    else if (inSector2)
        playerColor = MAGENTA;
    else if (inSector3)
        playerColor = ORANGE;
    else if (inSector4)
        playerColor = BLUE;
    else
        playerColor = WHITE;
}

int main()
{

    InitWindow(500, 500, "title");
    // CRITICAL
    SetTargetFPS(60);

    int screenHalf = GetScreenHeight() / 2;
    float f_screenHalf = static_cast<float>(screenHalf);
    Vector2 screenHalfVector = {f_screenHalf, f_screenHalf};
    Vector2 playerPOS =
    {
        250,
        60
    };

    Color playerColor;
    float currentDistance;
    int sectorOneRadiusGate = 152;
    int sectorTwoRadiusGate = 102;
    int sectorThreeRadiusGate = 52;
    int sectorFourRadiusGate = 50;


    float score = 0;
    float sectorOneScore = 50;
    float sectorTwoScore = 500;
    float sectorThreeScore = 1000;
    float sectorFourScore = 5000;

    while (!WindowShouldClose())
    {
        score += 1;
        currentDistance = fplayerVectorDistance(playerPOS, screenHalfVector);

        playerMove(playerPOS);
        colorPlayer(playerColor, playerPOS, screenHalfVector);

        BeginDrawing();

        ClearBackground(BLUE);
        // SECTOR 1
        score < sectorOneScore ? DrawRing(screenHalfVector, 150, 203, 0, 365, 1, RED) : DrawRing(screenHalfVector, 150, 203, 0, 365, 1, GREEN);
        DrawRing(screenHalfVector, 150, 200, 0, 365, 1, GREEN);
        if (!canMove(currentDistance, score, sectorOneScore, sectorOneRadiusGate, 200) && score <= sectorOneScore)
        {
            // d = √[(x2 - x1)2 + (y2 - y1)2]
            // sqrtf(  (powf((playerPOS.x-vec.x), 2.0f)) + (powf((playerPOS.y-vec.y), 2.0f)) );
            // d^2 = (px-shvx)2+(py-shvy)2
            //
            if (200-fplayerVectorDistance(playerPOS, screenHalfVector) < 5) 
            {
                enforceBoundary(playerPOS, screenHalfVector, 200);
            } else {
                enforceBoundary(playerPOS, screenHalfVector, sectorOneRadiusGate);
            }
            
        }
        // SECTOR 2
        score < sectorTwoScore ? DrawRing(screenHalfVector, 100, 153, 0, 365, 1, RED) : DrawRing(screenHalfVector, 100, 153, 0, 365, 1, GREEN);
        DrawRing(screenHalfVector, 100, 150, 0, 365, 1, YELLOW);
        if (!canMove(currentDistance, score, sectorTwoScore, sectorTwoRadiusGate, sectorOneRadiusGate) 
        && score > sectorOneScore 
        && score <= sectorTwoScore)
        {
            // d = √[(x2 - x1)2 + (y2 - y1)2]
            // sqrtf(  (powf((playerPOS.x-vec.x), 2.0f)) + (powf((playerPOS.y-vec.y), 2.0f)) );
            // d^2 = (px-shvx)2+(py-shvy)2
            //
            if (sectorOneRadiusGate-fplayerVectorDistance(playerPOS, screenHalfVector) < 5) {
                enforceBoundary(playerPOS, screenHalfVector, sectorOneRadiusGate);
            } else {
                enforceBoundary(playerPOS, screenHalfVector, sectorTwoRadiusGate);
            }
        }
        // SECTOR 3
        score < 1000 ? DrawRing(screenHalfVector, 50, 103, 0, 365, 1, RED) : DrawRing(screenHalfVector, 50, 103, 0, 365, 1, GREEN);
        DrawRing(screenHalfVector, 50, 100, 0, 365, 1, PURPLE);
        if (!canMove(currentDistance, score, sectorThreeScore, sectorThreeRadiusGate, sectorTwoRadiusGate) && score > sectorTwoScore && score <= sectorThreeScore)
        {
            // d = √[(x2 - x1)2 + (y2 - y1)2]
            // sqrtf(  (powf((playerPOS.x-vec.x), 2.0f)) + (powf((playerPOS.y-vec.y), 2.0f)) );
            // d^2 = (px-shvx)2+(py-shvy)2
            //
            if (sectorTwoRadiusGate-fplayerVectorDistance(playerPOS, screenHalfVector) < 5) {
                enforceBoundary(playerPOS, screenHalfVector, sectorTwoRadiusGate);
            } else {
                enforceBoundary(playerPOS, screenHalfVector, sectorThreeRadiusGate);
            }
        }
        // SECTOR 4
        score < 5000 ? DrawRing(screenHalfVector, 0, 53, 0, 365, 1, RED) : DrawRing(screenHalfVector, 0, 53, 0, 365, 1, GREEN);
        DrawRing(screenHalfVector, 0, 50, 0, 365, 1, BLACK);
        if (!canMove(currentDistance, score, sectorFourScore, sectorFourRadiusGate, sectorThreeRadiusGate) && score > sectorThreeScore && score <= sectorFourScore)
        {
            // d = √[(x2 - x1)2 + (y2 - y1)2]
            // sqrtf(  (powf((playerPOS.x-vec.x), 2.0f)) + (powf((playerPOS.y-vec.y), 2.0f)) );
            // d^2 = (px-shvx)2+(py-shvy)2
            if (sectorThreeRadiusGate-fplayerVectorDistance(playerPOS, screenHalfVector) < 5) 
                enforceBoundary(playerPOS, screenHalfVector, sectorThreeRadiusGate);
        }

        // PLAYER
        DrawCircle(playerPOS.x, playerPOS.y, 5, playerColor);
        DrawLineEx(playerPOS, screenHalfVector, 5, RED);
        // SCORE
        DrawText(TextFormat("%.2f", score), 50, 50, 10, RAYWHITE);
        // Players current position
        DrawText(TextFormat("PX:%.2f, PY:%.2f", playerPOS.x, playerPOS.y), 50, 60, 10, RAYWHITE);
        DrawText(TextFormat("DOC:%.2f", currentDistance), 50, 70, 10, RAYWHITE); // DOC = Dist. of Center
        //DrawText(TextFormat("FrameTime:%.2d", GetElapsed(t)), 50, 80, 10, RAYWHITE);
        DrawText(TextFormat("%f", powf((playerPOS.x-screenHalfVector.x), 2.0f) + powf((playerPOS.y-screenHalfVector.y), 2.0f)), 50, 90, 10, RAYWHITE);
        DrawText(TextFormat("%f", pow(200,2)), 50, 100, 10, RAYWHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}