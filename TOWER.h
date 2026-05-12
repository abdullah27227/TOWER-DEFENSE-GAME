#ifndef TOWER_H
#define TOWER_H


#include "enemy.h"
using namespace sf;

class projectile 
{
private:
    float x;
    float y;
    float target_x;
    float target_y;
    float speed;
    int damage;
    bool active;
    Color color;
    float slow_amount;

public:
    projectile();
    projectile(float x, float y, float tx, float ty, float speed, int damage, Color color, float slow = 0.f); //initial point, final point, speed, damage, color of projectile, slowing enemy

    void update(float dt);
    void render(RenderWindow& win);

    bool is_active() const;
    int get_damage() const;
    float get_slow_amount() const;
    float get_x() const;
    float get_y() const;
    void deactivate();
};

class tower : public entity
{
protected:
    float range;
    float fire_rate;
    float fire_cooldown;
    int damage;
    int build_cost;
    int level;
    Color color;

public:
    tower(float x, float y, float range, float fire_rate, int damage, int build_cost, Color color);
    virtual ~tower();

    void update(float dt) override;
    virtual void attack(enemy* enemies[], int enemy_count, projectile projectiles[], int& projectile_count) = 0; // making tower an abstract class through pure virtual function
    void render(RenderWindow& win) override;
    void take_damage(int dmg) override;

    enemy* find_closest_enemy(enemy* enemies[], int enemy_count);
    bool can_upgrade() const;
    int get_upgrade_cost() const;
    void upgrade_stats();
    int get_level() const;
    int get_build_cost() const;
};

class cannon_tower : public tower 
{
public:
    cannon_tower(float x, float y);
    void attack(enemy* enemies[], int enemy_count, projectile projectiles[], int& projectile_count) override;
    void render(RenderWindow& win) override;
};

class sniper_tower : public tower
{
public:
    sniper_tower(float x, float y);
    void attack(enemy* enemies[], int enemy_count, projectile projectiles[], int& projectile_count) override;
    void render(RenderWindow& win) override;
};

class machinegun_tower : public tower 
{
public:
    machinegun_tower(float x, float y);
    void attack(enemy* enemies[], int enemy_count, projectile projectiles[], int& projectile_count) override;
    void render(RenderWindow& win) override;
};

class slow_tower : public tower 
{
public:
    slow_tower(float x, float y);
    void attack(enemy* enemies[], int enemy_count, projectile projectiles[], int& projectile_count) override;
    void render(RenderWindow& win) override;
};

class bomb_tower : public tower
{
public:
    bomb_tower(float x, float y);
    void attack(enemy* enemies[], int enemy_count, projectile projectiles[], int& projectile_count) override;
    void render(RenderWindow& win) override;
};








#endif
