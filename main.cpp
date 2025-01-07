#include "main.hpp"
// Movement
void playerMove(Player &player, bool useArrowKeys)
{
    if (useArrowKeys)
    {
        if (IsKeyDown(KEY_RIGHT))
            player.position.x += 2.0f;
        if (IsKeyDown(KEY_LEFT))
            player.position.x -= 2.0f;
        if (IsKeyDown(KEY_UP))
            player.position.y -= 2.0f;
        if (IsKeyDown(KEY_DOWN))
            player.position.y += 2.0f;
    }
    else
    {
        if (IsKeyDown(KEY_D))
            player.position.x += 2.0f;
        if (IsKeyDown(KEY_A))
            player.position.x -= 2.0f;
        if (IsKeyDown(KEY_W))
            player.position.y -= 2.0f;
        if (IsKeyDown(KEY_S))
            player.position.y += 2.0f;
    }
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
float fplayerVectorDistance(Vector2 &player1, Vector2 &vec)
{
    // d = √[(x2 - x1)2 + (y2 - y1)2]
    float dx = player1.x - vec.x;
    float dy = player1.y - vec.y;
    return sqrtf((dx * dx) + (dy * dy));
}
// Boundaries and Collisions
// We know the distance, need to offset player vector to position outside of boudnary
void enforceBoundary(Player &player, const Vector2 &vec, float boundaryRadius)
{
    // New Vector Components. -player1 so we go (-)opposite direction of offending direction <vec.x, vec.y>
    float dirX = vec.x - player.position.x;
    float dirY = vec.y - player.position.y;
    // New Vector Length
    float dirLen = sqrtf(dirX * dirX + dirY * dirY);
    // Normalize, like getting a common denom./conextualizing, conforming the vector transformations uniformally.
    float normDirX = dirX / dirLen;
    float normDirY = dirY / dirLen;
    // offset by the length
    float offsetX = normDirX * boundaryRadius;
    float offsetY = normDirY * boundaryRadius;

    // PUT THE PLAYA THEIR DAH PLACE
    player.position.x = vec.x - offsetX;
    player.position.y = vec.y - offsetY;
}
void twoGateCollision(int outerGateRadius, int innerGateRadius, Player &player, Vector2 &screenCenter)
{
    if (outerGateRadius - fplayerVectorDistance(player.position, screenCenter) < 5)
    {
        enforceBoundary(player, screenCenter, outerGateRadius);
    }
    else
    {
        enforceBoundary(player, screenCenter, innerGateRadius);
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
void colorPlayer(Color &playerColor, Vector2 &player1, Vector2 &screenCenter)
{
    bool inSector1 = inRing(player1, screenCenter, SECTOR_A_RADIUS, OUTER_RING_RADIUS);
    bool inSector2 = inRing(player1, screenCenter, SECTOR_B_RADIUS, SECTOR_A_RADIUS);
    bool inSector3 = inRing(player1, screenCenter, SECTOR_C_RADIUS, SECTOR_B_RADIUS);
    bool inSector4 = inRing(player1, screenCenter, ORIGIN, FINAL_SECTOR_RADIUS);

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

typedef struct playerDistances
{
    float playerOne;
    float playerTwo;
    float playerCPU;
} playerDistances;

void updatePlayerDistances(playerDistances &pds,float &p1, float &p2, float &cpu)
{
    pds.playerOne = p1;
    pds.playerTwo = p2;
    pds.playerCPU = cpu;
}
void sectorRegulation(Vector2& player, float playerDistance, float &score, const float sectorInnerRadius, const float sectorOuterRadius, const float sectorScore, Vector2 &centerScreenVector)
{
    if (!canMove(playerDistance, score, sectorScore, sectorInnerRadius, sectorOuterRadius) && score <= sectorScore)
        {
            twoGateCollision(sectorOuterRadius, sectorInnerRadius, player, centerScreenVector);
        }
}
typedef struct playersPositions
{
    Vector2 playerOnePOS;
    Vector2 player2;
    Vector2 cpu;
} playersPOS;

int main()
{

    InitWindow(500, 500, "title");
    // CRITICAL
    SetTargetFPS(FPS);
    float cpuSpeed = .000f;

    //int screenHalf = GetScreenHeight() / 2;
    //float fScreenHalf = static_cast<float>(screenHalf);
    //Vector2 screenCenter = {fScreenHalf, fScreenHalf};
    Vector2 screenCenter = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};

    Player player1 = {{250, 60}, WHITE, STARTING_SCORE};
    Player player2 = {{270, 65}, WHITE, STARTING_SCORE};
    Player cpu = {{260, 70}, WHITE, STARTING_SCORE};

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

    playerDistances pDists = {0.0f, 0.0f, 0.0f};
    
    while (!WindowShouldClose())
    {
        angle += .02f;
        // Collisions on players
        if (CheckCollisionCircles(player1.position, 5, cpu.position, 5))
        {
            bodyCollision(player1.position, cpu.position, 10.0f);
        }
        if (CheckCollisionCircles(player1.position, 5, player2.position, 5))
        {
            bodyCollision(player1.position, player2.position, 10.0f);
        }
        if (CheckCollisionCircles(player2.position, 5, cpu.position, 5))
        {
            bodyCollision(player2.position, cpu.position, 10.0f);
        }
        // rn, score as long as there's a collison. Eventually diff scores, diff penalties based on collision.
        collisonScorePenalty(score, player1.position, player2.position);

        
        currentDistance = fplayerVectorDistance(player1.position, screenCenter);
        currentDistancePTwo = fplayerVectorDistance(player2.position, screenCenter);
        currentDistancepCPU = fplayerVectorDistance(cpu.position, screenCenter);
        
        updatePlayerDistances(pDists, currentDistance, currentDistancePTwo, currentDistancepCPU);

        playerMove(player1, 0);
        //mousePlayerMovement(player1);
        cpuMove(cpu.position, screenCenter, 170, angle, cpuSpeed);

        colorPlayer(playerColor, player1.position, screenCenter);
        colorPlayer(playerColor, cpu.position, screenCenter);
        colorPlayer(playerColor, player2.position, screenCenter);

        BeginDrawing();

        ClearBackground(BLUE);
        // SECTOR 1
        score < sectorOneScore ? DrawRing(screenCenter, 150, 203, 0, 365, 1, RED) : DrawRing(screenCenter, 150, 203, 0, 365, 1, GREEN);
        DrawRing(screenCenter, 150, 200, 0, 365, 1, GREEN);
        sectorRegulation(player1.position, pDists.playerOne, score, sectorOneRadiusGate, outermostRadiusGate, sectorOneScore, screenCenter);
        sectorRegulation(player1.position, pDists.playerOne, score, sectorOneRadiusGate, outermostRadiusGate, sectorOneScore, screenCenter);
        sectorRegulation(player1.position, pDists.playerOne, score, sectorOneRadiusGate, outermostRadiusGate, sectorOneScore, screenCenter);

        // if (!canMove(currentDistance, score, sectorOneScore, sectorOneRadiusGate, 200) && score <= sectorOneScore)
        // {
        //     twoGateCollision(outermostRadiusGate, sectorOneRadiusGate, player1, screenCenter);
        //     //twoGateCollision(outermostRadiusGate, sectorOneRadiusGate, cpu, screenCenter);
        //     //twoGateCollision(outermostRadiusGate, sectorOneRadiusGate, player2, screenCenter);
        // }
        // if (!canMove(currentDistancePTwo, score, sectorOneScore, sectorOneRadiusGate, 200) && score <= sectorOneScore)
        // {
        //     //twoGateCollision(outermostRadiusGate, sectorOneRadiusGate, player1, screenCenter);
        //     //twoGateCollision(outermostRadiusGate, sectorOneRadiusGate, cpu, screenCenter);
        //     twoGateCollision(outermostRadiusGate, sectorOneRadiusGate, player2, screenCenter);
        // }
        // SECTOR 2
        score < sectorTwoScore ? DrawRing(screenCenter, 100, 153, 0, 365, 1, RED) : DrawRing(screenCenter, 100, 153, 0, 365, 1, GREEN);
        DrawRing(screenCenter, 100, 150, 0, 365, 1, YELLOW);
        if (!canMove(currentDistance, score, sectorTwoScore, sectorTwoRadiusGate, sectorOneRadiusGate) && score > sectorOneScore && score <= sectorTwoScore)
        {
            twoGateCollision(sectorOneRadiusGate, sectorTwoRadiusGate, player1.position, screenCenter);
        }
        if (!canMove(currentDistancePTwo, score, sectorTwoScore, sectorTwoRadiusGate, sectorOneRadiusGate) && score > sectorOneScore && score <= sectorTwoScore)
        {
            twoGateCollision(sectorOneRadiusGate, sectorTwoRadiusGate, player2.position, screenCenter);
        }
        // SECTOR 3
        score < sectorThreeScore ? DrawRing(screenCenter, 50, 103, 0, 365, 1, RED) : DrawRing(screenCenter, 50, 103, 0, 365, 1, GREEN);
        DrawRing(screenCenter, 50, 100, 0, 365, 1, PURPLE);
        // BUG, they got stuck
        if (!canMove(currentDistance, score, sectorThreeScore, sectorThreeRadiusGate, sectorTwoRadiusGate) && score > sectorTwoScore && score <= sectorThreeScore)
        {
            twoGateCollision(sectorTwoRadiusGate, sectorThreeRadiusGate, player1.position, screenCenter);
        }
        if (!canMove(currentDistancePTwo, score, sectorThreeScore, sectorThreeRadiusGate, sectorTwoRadiusGate) && score > sectorTwoScore && score <= sectorThreeScore)
        {
            twoGateCollision(sectorTwoRadiusGate, sectorThreeRadiusGate, player2.position, screenCenter);
        }
        // SECTOR 4
        score < sectorFourScore ? DrawRing(screenCenter, 0, 53, 0, 365, 1, RED) : DrawRing(screenCenter, 0, 53, 0, 365, 1, GREEN);
        DrawRing(screenCenter, 0, 50, 0, 365, 1, BLACK);
        if (!canMove(currentDistance, score, sectorFourScore, sectorFourRadiusGate, sectorThreeRadiusGate) && score > sectorThreeScore && score <= sectorFourScore)
        {
            if (sectorThreeRadiusGate - fplayerVectorDistance(player1.position, screenCenter) < 5)
                enforceBoundary(player1.position, screenCenter, sectorThreeRadiusGate);
        }
        if (!canMove(currentDistancePTwo, score, sectorFourScore, sectorFourRadiusGate, sectorThreeRadiusGate) && score > sectorThreeScore && score <= sectorFourScore)
        {
            if (sectorThreeRadiusGate - fplayerVectorDistance(player2.position, screenCenter) < 5)
                enforceBoundary(player2.position, screenCenter, sectorThreeRadiusGate);
        }

        // PLAYER
        DrawCircle(player1.position.x, player1.position.y, 5, playerColor);
        DrawCircle(cpu.position.x, cpu.position.y, 5, playerColor);
        DrawCircle(player2.position.x, player2.position.y, 5, playerColor);
        // DrawLineEx(player1, screenCenter, 5, RED);
        //  SCORE
        DrawText(TextFormat("%.2f", score), 50, 50, 10, RAYWHITE);
        // Players current position
        DrawText(TextFormat("PX:%.2f, PY:%.2f", player1.position.x, player1.position.y), 50, 60, 10, RAYWHITE);
        DrawText(TextFormat("DOC:%.2f", currentDistance), 50, 70, 10, RAYWHITE); // DOC = Dist. of Center
        DrawText(TextFormat("P1=%.2f, P2=%.2f, CPU=%.2f", pDists.playerOne, pDists.playerTwo, pDists.playerCPU), 50, 80, 10, RAYWHITE);
        DrawText("You", player1.position.x, player1.position.y, 5, BLACK);
        DrawText("CPU", cpu.position.x, cpu.position.y, 5, BLACK);
        DrawText("Player2", player2.position.x, player2.position.y, 5, BLACK);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}