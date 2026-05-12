#ifndef MAP_H
#define MAP_H

#include "Tower.h"

#include <string>
using namespace std;
using namespace sf;

struct wave
{
    int enemy_types[max_spawn];
    int count;
    float spawn_delay;
};

class game
{
private:
    RenderWindow window;
    Font font;
    bool font_loaded;
   
    tile_type grid[rows][cols];
    waypoint waypoints[max_waypoints];
    int waypoint_count;

    enemy* enemies[max_enemies];
    int enemy_count;

    tower* towers[max_towers];
    int tower_count;

    projectile projectiles[max_projectiles];
    int projectile_count;

    wave waves[max_waves];
    int current_wave;
    int spawn_index;
    float spawn_timer;
    bool wave_active;
    float wave_pause_timer;

    int gold;
    int lives;
    int score;
    bool game_over;
    bool game_won;

    int selected_tower;
    int shop_costs[shop_items];
    string shop_names[shop_items];

    void setup_map();
    void setup_waves();
    void setup_shop();
    void spawn_enemy(int type);
    bool can_place_tower(int tx, int ty) const;
    void place_tower(int tile_x, int tile_y);
    void upgrade_tower(int tile_x, int tile_y);
    void add_wp(int col, int row);
    void check_collisions();
    void remove_dead_enemies();
    void remove_inactive_projectiles();

    void draw_map();
    void draw_text(const string& text, float x, float y, int size = 16, Color col = Color::White);
    void draw_ui();
    void draw_game_over();
    void draw_win();

public:
    game();
    ~game();
    void run();
};
















#endif 

