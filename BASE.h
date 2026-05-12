#ifndef BASE_H
#define BASE_H

#include <SFML/Graphics.hpp>
using namespace sf;

const int cols = 20;
const int rows = 15;
const int ui_height = 60;
const int tile_size = 60;
const int win_width = cols * tile_size;
const int win_height = rows * tile_size + ui_height;

const int max_waypoints = 20;
const int max_enemies = 100;
const int max_towers = 50;
const int max_projectiles = 300;
const int max_waves = 5;
const int max_spawn = 15;
const int shop_items = 5;

enum tile_type { grass, path, blocked };

struct waypoint
{
    float x;
    float y;
};

void draw_hp_bar(RenderWindow& window, float x, float y, int hp, int max_hp, float width = 30.f);

class entity
{
protected:
    float x;
    float y;
    int hp;
    int max_hp;

public:
    entity(float x, float y, int hp);
    virtual ~entity();

    virtual void update(float dt) = 0;
    virtual void render(RenderWindow& win) = 0;
    virtual void take_damage(int dmg);

    bool is_alive() const;
    float get_x() const;
    float get_y() const;
    int get_hp() const;
    int get_max_hp() const;
};




#endif 
