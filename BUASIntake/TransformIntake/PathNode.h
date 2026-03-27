#pragma once
//--- This is just one point on the enemy path.
//A struct is used here because this is only used to store data, so it stays ligtweight and easier to read ---
struct PathNode
{
    float x = 0.0f;
    float y = 0.0f;

    PathNode() = default;
    PathNode(float px, float py) : x(px), y(py) {}
};