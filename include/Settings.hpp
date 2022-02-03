#pragma once

class Position;

class Settings
{
private:
    Settings(){};

public:
    static Position map_size;
    static bool enable_walls;
};
