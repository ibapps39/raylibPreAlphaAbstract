#include "main.hpp"

// Movement
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
void playerMoveArrow(Vector2 &p)
{
    if (IsKeyDown(KEY_RIGHT))
        p.x += 2.0f;
    if (IsKeyDown(KEY_LEFT))
        p.x -= 2.0f;
    if (IsKeyDown(KEY_UP))
        p.y -= 2.0f;
    if (IsKeyDown(KEY_DOWN))
        p.y += 2.0f;
}
void mousePlayerMovement(Vector2 &p) 
{
    p = GetMousePosition();
}
void cpuMove(Vector2 &p2, const Vector2 &screenCenter, float radius, float &angle, float speed)
{
    // Update the angle
    angle += speed;

    // Keep the angle between 0 and 2π
    if (angle > 2 * PI)
        angle -= 2 * PI;

    // Calculate new position
    p2.x = screenCenter.x + radius * cos(angle);
    p2.y = screenCenter.y + radius * sin(angle);
}

bool canMove(float &distance, float &score, float sectorScore, int innerRadius, int outerRadius)
{
    return score < sectorScore && distance > innerRadius && distance < outerRadius;
}
// Player Distance
float fplayerVectorDistance(Vector2 &playerPOS, Vector2 &vec)
{
    // d = √[(x2 - x1)2 + (y2 - y1)2]
    float dx = playerPOS.x - vec.x;
    float dy = playerPOS.y - vec.y;
    return sqrtf((dx * dx) + (dy * dy));
}
// Boundaries and Collisions
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
void bodyCollision(Vector2 &vecA, Vector2 &vecB, float force)
{
    float dx = vecA.x - vecB.x;
    float dy = vecA.y - vecB.y;
    float dist = sqrtf(dx * dx + dy * dy);
    if (dist <= TEMP_RADIUS)
    {
        vecA.x += dx / dist * force;
        vecA.y += dy / dist * force;
        vecB.x -= dx / dist * force;
        vecB.y -= dy / dist * force;
    }
}

bool inRing(Vector2 &pointVector, Vector2 &ringVector, float innerRadius, float outerRadius)
{
    float distance = fplayerVectorDistance(pointVector, ringVector);
    return (distance <= outerRadius) && (distance >= innerRadius);
}
bool bbodyCollision(Vector2 &vecA, Vector2 &vecB)
{
    // ok...so TEMP_RADIUS is FINE as is in bodyCollision but not here?!? (hence +1)
    return fplayerVectorDistance(vecA, vecB) <= TEMP_RADIUS+1;
}

// Player aesethetics
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

// FOR NOW: only player1
void collisonScorePenalty(float &score, Vector2 &entityA, Vector2 &entityB)
{
    if (bbodyCollision(entityA, entityB))
    {
        score += 50;
    }
}



int main()
{

    InitWindow(500, 500, "title");
    // CRITICAL
    SetTargetFPS(FPS);
    float cpuSpeed = .000f;

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
        Vector2 playerTwoPOS =
        {
            270,
            65
        };

    Color playerColor;

    float currentDistance = 0;
    float currentDistancePTwo = 0;
    float currentDistancepCPU = 0;

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
        angle += .02f;
        // Collisions on players
        if (CheckCollisionCircles(playerPOS, 5, cpuPOS, 5))
        {
            bodyCollision(playerPOS, cpuPOS, 10.0f);
        }
        if (CheckCollisionCircles(playerPOS, 5, playerTwoPOS, 5))
        {
            bodyCollision(playerPOS, playerTwoPOS, 10.0f);
        }
        if (CheckCollisionCircles(playerTwoPOS, 5, cpuPOS, 5))
        {
            bodyCollision(playerTwoPOS, cpuPOS, 10.0f);
        }
        // rn, score as long as there's a collison. Eventually diff scores, diff penalties based on collision.
        collisonScorePenalty(score, playerPOS, playerTwoPOS);

        
        currentDistance = fplayerVectorDistance(playerPOS, screenHalfVector);
        currentDistancePTwo = fplayerVectorDistance(playerTwoPOS, screenHalfVector);
        currentDistancepCPU = fplayerVectorDistance(cpuPOS, screenHalfVector);

        playerMove(playerPOS);
        //mousePlayerMovement(playerPOS);
        cpuMove(cpuPOS, screenHalfVector, 170, angle, cpuSpeed);
        playerMoveArrow(playerTwoPOS);

        colorPlayer(playerColor, playerPOS, screenHalfVector);
        colorPlayer(playerColor, cpuPOS, screenHalfVector);
        colorPlayer(playerColor, playerTwoPOS, screenHalfVector);

        BeginDrawing();

        ClearBackground(BLUE);
        // SECTOR 1
        score < sectorOneScore ? DrawRing(screenHalfVector, 150, 203, 0, 365, 1, RED) : DrawRing(screenHalfVector, 150, 203, 0, 365, 1, GREEN);
        DrawRing(screenHalfVector, 150, 200, 0, 365, 1, GREEN);

        if (!canMove(currentDistance, score, sectorOneScore, sectorOneRadiusGate, 200) && score <= sectorOneScore)
        {
            twoGateCollision(outermostRadiusGate, sectorOneRadiusGate, playerPOS, screenHalfVector);
            //twoGateCollision(outermostRadiusGate, sectorOneRadiusGate, cpuPOS, screenHalfVector);
            //twoGateCollision(outermostRadiusGate, sectorOneRadiusGate, playerTwoPOS, screenHalfVector);
        }
        if (!canMove(currentDistancePTwo, score, sectorOneScore, sectorOneRadiusGate, 200) && score <= sectorOneScore)
        {
            //twoGateCollision(outermostRadiusGate, sectorOneRadiusGate, playerPOS, screenHalfVector);
            //twoGateCollision(outermostRadiusGate, sectorOneRadiusGate, cpuPOS, screenHalfVector);
            twoGateCollision(outermostRadiusGate, sectorOneRadiusGate, playerTwoPOS, screenHalfVector);
        }
        // SECTOR 2
        score < sectorTwoScore ? DrawRing(screenHalfVector, 100, 153, 0, 365, 1, RED) : DrawRing(screenHalfVector, 100, 153, 0, 365, 1, GREEN);
        DrawRing(screenHalfVector, 100, 150, 0, 365, 1, YELLOW);
        if (!canMove(currentDistance, score, sectorTwoScore, sectorTwoRadiusGate, sectorOneRadiusGate) && score > sectorOneScore && score <= sectorTwoScore)
        {
            twoGateCollision(sectorOneRadiusGate, sectorTwoRadiusGate, playerPOS, screenHalfVector);
        }
        if (!canMove(currentDistancePTwo, score, sectorTwoScore, sectorTwoRadiusGate, sectorOneRadiusGate) && score > sectorOneScore && score <= sectorTwoScore)
        {
            twoGateCollision(sectorOneRadiusGate, sectorTwoRadiusGate, playerTwoPOS, screenHalfVector);
        }
        // SECTOR 3
        score < sectorThreeScore ? DrawRing(screenHalfVector, 50, 103, 0, 365, 1, RED) : DrawRing(screenHalfVector, 50, 103, 0, 365, 1, GREEN);
        DrawRing(screenHalfVector, 50, 100, 0, 365, 1, PURPLE);
        // BUG, they got stuck
        if (!canMove(currentDistance, score, sectorThreeScore, sectorThreeRadiusGate, sectorTwoRadiusGate) && score > sectorTwoScore && score <= sectorThreeScore)
        {
            twoGateCollision(sectorTwoRadiusGate, sectorThreeRadiusGate, playerPOS, screenHalfVector);
        }
        if (!canMove(currentDistancePTwo, score, sectorThreeScore, sectorThreeRadiusGate, sectorTwoRadiusGate) && score > sectorTwoScore && score <= sectorThreeScore)
        {
            twoGateCollision(sectorTwoRadiusGate, sectorThreeRadiusGate, playerTwoPOS, screenHalfVector);
        }
        // SECTOR 4
        score < sectorFourScore ? DrawRing(screenHalfVector, 0, 53, 0, 365, 1, RED) : DrawRing(screenHalfVector, 0, 53, 0, 365, 1, GREEN);
        DrawRing(screenHalfVector, 0, 50, 0, 365, 1, BLACK);
        if (!canMove(currentDistance, score, sectorFourScore, sectorFourRadiusGate, sectorThreeRadiusGate) && score > sectorThreeScore && score <= sectorFourScore)
        {
            if (sectorThreeRadiusGate - fplayerVectorDistance(playerPOS, screenHalfVector) < 5)
                enforceBoundary(playerPOS, screenHalfVector, sectorThreeRadiusGate);
        }
        if (!canMove(currentDistancePTwo, score, sectorFourScore, sectorFourRadiusGate, sectorThreeRadiusGate) && score > sectorThreeScore && score <= sectorFourScore)
        {
            if (sectorThreeRadiusGate - fplayerVectorDistance(playerTwoPOS, screenHalfVector) < 5)
                enforceBoundary(playerTwoPOS, screenHalfVector, sectorThreeRadiusGate);
        }

        // PLAYER
        DrawCircle(playerPOS.x, playerPOS.y, 5, playerColor);
        DrawCircle(cpuPOS.x, cpuPOS.y, 5, playerColor);
        DrawCircle(playerTwoPOS.x, playerTwoPOS.y, 5, playerColor);
        // DrawLineEx(playerPOS, screenHalfVector, 5, RED);
        //  SCORE
        DrawText(TextFormat("%.2f", score), 50, 50, 10, RAYWHITE);
        // Players current position
        DrawText(TextFormat("PX:%.2f, PY:%.2f", playerPOS.x, playerPOS.y), 50, 60, 10, RAYWHITE);
        DrawText(TextFormat("DOC:%.2f", currentDistance), 50, 70, 10, RAYWHITE); // DOC = Dist. of Center
        DrawText(TextFormat("%f", fplayerVectorDistance(playerPOS, playerTwoPOS)), 50, 80, 10, RAYWHITE);
        DrawText("You", playerPOS.x, playerPOS.y, 5, BLACK);
        DrawText("CPU", cpuPOS.x, cpuPOS.y, 5, BLACK);
        DrawText("Player2", playerTwoPOS.x, playerTwoPOS.y, 5, BLACK);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}