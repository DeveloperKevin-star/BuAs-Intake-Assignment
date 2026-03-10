#pragma once

struct PathNode
{
    float x = 0.0f;
    float y = 0.0f;

    PathNode() = default;
    PathNode(float px, float py) : x(px), y(py) {}
};