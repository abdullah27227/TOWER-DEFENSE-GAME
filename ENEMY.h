#ifndef ENEMY_H
#define ENEMY_H

#include "base.h"
using namespace sf;

class enemy : public entity
{
protected:
    float speed;
    float base_speed;
    int reward;
    int waypoint_index;
    bool reached_end;
    Color color;
    const waypoint* enemy_path;
    int path_size;

public:
    enemy(float x, float y, int hp, float speed, int reward, Color color);
    virtual ~enemy();

    void set_path(const waypoint* p, int size);
    void update(float dt) override;
    void render(RenderWindow& win) override;

    bool has_reached() const;
    int get_reward() const;

    void apply_slow(float amount);
};

class basic_enemy : public enemy
{
public:
    basic_enemy(float x, float y);
};

class fast_enemy : public enemy 
{
public:
    fast_enemy(float x, float y);
    void render(RenderWindow& win) override;
};

class tank_enemy : public enemy 
{
public:
    tank_enemy(float x, float y);
    void render(RenderWindow& win) override;
};

class flying_enemy : public enemy
{
public:
    flying_enemy(float x, float y);
    void update(float dt) override;
    void render(RenderWindow& win) override;
};

class healer_enemy : public enemy
{
private:
    float heal_timer;

public:
    healer_enemy(float x, float y);
    void update(float dt) override;
    void render(RenderWindow& win) override;
};






#endif 

