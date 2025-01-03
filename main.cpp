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
void cpuMove(Vector2 &p2, const Vector2 &screenCenter, float radius, float &angle, float speed)
{
    // Update the angle
    angle += speed;
    
    // Keep the angle between 0 and 2π
    if (angle > 2 * PI) angle -= 2 * PI;
    
    // Calculate new position
    p2.x = screenCenter.x + radius * cos(angle);
    p2.y = screenCenter.y + radius * sin(angle);
}


bool canMove(float &distance, float &score, float sectorScore, int innerRadius, int outerRadius)
{
    return score < sectorScore && distance > innerRadius && distance < outerRadius;
}
float fplayerVectorDistance(Vector2 &playerPOS, Vector2 &vec)
{
    // d = √[(x2 - x1)2 + (y2 - y1)2]
    float dx = playerPOS.x - vec.x;
    float dy = playerPOS.y - vec.y;
    return sqrtf( (dx*dx) + (dy*dy) );
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
void twoGateCollision(int outerGateRadius, int innerGateRadius, Vector2 &playerPOS, Vector2 &screenHalfVector)
{
    if (outerGateRadius - fplayerVectorDistance(playerPOS, screenHalfVector) < 5)
    {
        enforceBoundary(playerPOS, screenHalfVector, outerGateRadius);
    }
    else
    {
        enforceBoundary(playerPOS, screenHalfVector, innerGateRadius);
    }
}
bool inRing(Vector2 &pointVector, Vector2 &ringVector, float innerRadius, float outerRadius)
{
    float distance = fplayerVectorDistance(pointVector, ringVector);
    return (distance <= outerRadius) && (distance >= innerRadius);
}
void colorPlayer(Color &playerColor, Vector2 &playerPOS, Vector2 &screenHalfVector)
{
    bool inSector1 = inRing(playerPOS, screenHalfVector, SECTOR_A_RADIUS, OUTER_RING_RADIUS);
    bool inSector2 = inRing(playerPOS, screenHalfVector, SECTOR_B_RADIUS, SECTOR_A_RADIUS);
    bool inSector3 = inRing(playerPOS, screenHalfVector, SECTOR_C_RADIUS, SECTOR_B_RADIUS);
    bool inSector4 = inRing(playerPOS, screenHalfVector, ORIGIN, FINAL_SECTOR_RADIUS);

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
    SetTargetFPS(FPS);
    float cpuSpeed = .020f;

    int screenHalf = GetScreenHeight() / 2;
    float fScreenHalf = static_cast<float>(screenHalf);
    Vector2 screenHalfVector = {fScreenHalf, fScreenHalf};
    Vector2 playerPOS =
    {
        250,
        60
    };
    Vector2 cpuPOS =
    {
        240,
        60
    };

    Color playerColor;
    float currentDistance;
    
    int outermostRadiusGate = 200;
    int sectorOneRadiusGate = 152;
    int sectorTwoRadiusGate = 102;
    int sectorThreeRadiusGate = 52;
    int sectorFourRadiusGate = 50;


    float score = 0;
    float sectorOneScore = 50;
    float sectorTwoScore = 500;
    float sectorThreeScore = 1000;
    float sectorFourScore = 5000;
    float angle = 0;

    while (!WindowShouldClose())
    {
        if (CheckCollisionCircles(playerPOS, 5, cpuPOS, 5)){
            enforceBoundary(playerPOS, cpuPOS, cpuPOS.x);
        }
        score += .001;
        
        currentDistance = fplayerVectorDistance(playerPOS, screenHalfVector);

        playerMove(playerPOS);
        cpuMove(cpuPOS, screenHalfVector, fplayerVectorDistance(playerPOS, screenHalfVector), angle, cpuSpeed);
        colorPlayer(playerColor, playerPOS, screenHalfVector);
        colorPlayer(playerColor, cpuPOS, screenHalfVector);

        BeginDrawing();

        ClearBackground(BLUE);
        // SECTOR 1
        score < sectorOneScore ? DrawRing(screenHalfVector, 150, 203, 0, 365, 1, RED) : DrawRing(screenHalfVector, 150, 203, 0, 365, 1, GREEN);
        DrawRing(screenHalfVector, 150, 200, 0, 365, 1, GREEN);
        if (!canMove(currentDistance, score, sectorOneScore, sectorOneRadiusGate, 200) && score <= sectorOneScore)
        {
            twoGateCollision(outermostRadiusGate, sectorOneRadiusGate, playerPOS, screenHalfVector);   
        }
        // SECTOR 2
        score < sectorTwoScore ? DrawRing(screenHalfVector, 100, 153, 0, 365, 1, RED) : DrawRing(screenHalfVector, 100, 153, 0, 365, 1, GREEN);
        DrawRing(screenHalfVector, 100, 150, 0, 365, 1, YELLOW);
        if (!canMove(currentDistance, score, sectorTwoScore, sectorTwoRadiusGate, sectorOneRadiusGate) 
        && score > sectorOneScore 
        && score <= sectorTwoScore)
        {
            twoGateCollision(sectorOneRadiusGate, sectorTwoRadiusGate, playerPOS, screenHalfVector);
        }
        // SECTOR 3
        score < sectorThreeScore ? DrawRing(screenHalfVector, 50, 103, 0, 365, 1, RED) : DrawRing(screenHalfVector, 50, 103, 0, 365, 1, GREEN);
        DrawRing(screenHalfVector, 50, 100, 0, 365, 1, PURPLE);
        if (!canMove(currentDistance, score, sectorThreeScore, sectorThreeRadiusGate, sectorTwoRadiusGate) && score > sectorTwoScore && score <= sectorThreeScore)
        {
            twoGateCollision(sectorTwoRadiusGate, sectorThreeRadiusGate, playerPOS, screenHalfVector);
        }
        // SECTOR 4
        score < sectorFourScore ? DrawRing(screenHalfVector, 0, 53, 0, 365, 1, RED) : DrawRing(screenHalfVector, 0, 53, 0, 365, 1, GREEN);
        DrawRing(screenHalfVector, 0, 50, 0, 365, 1, BLACK);
        if (!canMove(currentDistance, score, sectorFourScore, sectorFourRadiusGate, sectorThreeRadiusGate) && score > sectorThreeScore && score <= sectorFourScore)
        {
            if (sectorThreeRadiusGate-fplayerVectorDistance(playerPOS, screenHalfVector) < 5) 
                enforceBoundary(playerPOS, screenHalfVector, sectorThreeRadiusGate);
        }

        // PLAYER
        DrawCircle(playerPOS.x, playerPOS.y, 5, playerColor);
        DrawCircle(cpuPOS.x, cpuPOS.y, 5, playerColor);
        //DrawLineEx(playerPOS, screenHalfVector, 5, RED);
        // SCORE
        DrawText(TextFormat("%.2f", score), 50, 50, 10, RAYWHITE);
        // Players current position
        DrawText(TextFormat("PX:%.2f, PY:%.2f", playerPOS.x, playerPOS.y), 50, 60, 10, RAYWHITE);
        DrawText(TextFormat("DOC:%.2f", currentDistance), 50, 70, 10, RAYWHITE); // DOC = Dist. of Center
        //DrawText(TextFormat("FrameTime:%.2d", GetElapsed(t)), 50, 80, 10, RAYWHITE);
        DrawText("You", playerPOS.x, playerPOS.y, 5, BLACK);
        DrawText("CPU", cpuPOS.x, cpuPOS.y, 5, BLACK);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}