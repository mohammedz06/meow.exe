#pragma once

void obstacleInit();

// Returns distance in cm
float obstacleReadCm();

// Full avoidance routine: reverse, scan left/right with servo, pick a direction
// Returns the chosen motor bias to apply after avoidance (-1 = left, 1 = right)
int   obstacleAvoid();
