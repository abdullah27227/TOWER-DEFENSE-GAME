#include "Tower.h"

#include <cmath>
using namespace std;
using namespace sf;

projectile::projectile()
    : x(0.f), y(0.f), target_x(0.f), target_y(0.f), speed(0.f), damage(0), active(false), color(Color::White), slow_amount(0.f) 
{
}

projectile::projectile(float x, float y, float tx, float ty, float speed, int damage, Color color, float slow)
    : x(x), y(y), target_x(tx), target_y(ty), speed(speed), damage(damage), active(true), color(color), slow_amount(slow)
{
}

void projectile::update(float dt)
{
    const float dx = target_x - x;
    const float dy = target_y - y;
    const float dist = sqrt(dx * dx + dy * dy);

    if (dist < 8.f) // range of towers (if distance more than 8 pixels... then don't shoot)
    {
        active = false;
        return;
    }

    x += (dx / dist) * speed * dt;
    y += (dy / dist) * speed * dt;
}

void projectile::render(RenderWindow& win) // projectile visual controls
{
    CircleShape shape(4.f);
    shape.setFillColor(color);
    shape.setOrigin(4.f, 4.f);
    shape.setPosition(x, y);
    win.draw(shape);
}

bool projectile::is_active() const
{ 
    return active;
}
int projectile::get_damage() const
{ 
    return damage; 
}
float projectile::get_slow_amount() const 
{ 
    return slow_amount;
}
float projectile::get_x() const 
{ 
    return x;
}
float projectile::get_y() const 
{
    return y;
}
void projectile::deactivate() 
{
    active = false;
}

tower::tower(float x, float y, float range, float fire_rate, int damage, int build_cost, Color color)
    : entity(x, y, 9999),
    range(range),
    fire_rate(fire_rate),
    fire_cooldown(0.f),
    damage(damage),
    build_cost(build_cost),
    level(1),
    color(color)
{
}

tower::~tower() = default;

void tower::update(float dt)
{
    if (fire_cooldown > 0.f) 
    {
        fire_cooldown -= dt;
    }
}

void tower::render(RenderWindow& win) 
{
   
    RectangleShape shape({ 32.f, 32.f });
    shape.setFillColor(color);
    shape.setOutlineThickness(2.f);
    shape.setOutlineColor(Color::White);
    shape.setOrigin(16.f, 16.f);
    shape.setPosition(x, y);
    win.draw(shape);
   
}

void tower::take_damage(int)
{
}

enemy* tower::find_closest_enemy(enemy* enemies[], int enemy_count)
{
    enemy* closest = nullptr;
    float min_dist = range + 1.f;

    for (int i = 0; i < enemy_count; i++)
    {
        if (!enemies[i] || !enemies[i]->is_alive())
        {
            continue;
        }

        const float dx = enemies[i]->get_x() - x;
        const float dy = enemies[i]->get_y() - y;
        const float dist = sqrt(dx * dx + dy * dy);
        if (dist <= range && dist < min_dist) {
            min_dist = dist;
            closest = enemies[i];
        }
    }
    return closest;
}

bool tower::can_upgrade() const
{
    return level < 3;
}

int tower::get_upgrade_cost() const // upgrading cost control
{
    if (level == 1)
    {
        return build_cost / 2;
    }
    if (level == 2)
    {
        return build_cost;
    }
    return 0;
}

void tower::upgrade_stats() // benefits of doing upgrade
{
    if (!can_upgrade())
    {
        return;
    }
    level++;
    damage = static_cast<int>(damage * 1.35f);
    range += 12.f;
    fire_rate *= 1.15f;
}

int tower::get_level() const
{
    return level;
}

int tower::get_build_cost() const
{
    return build_cost;
}

cannon_tower::cannon_tower(float x, float y)
    : tower(x, y, 120.f, 0.8f, 80, 100, Color(180, 90, 0)) // cannon tower controls
{
}

void cannon_tower::attack(enemy* enemies[], int enemy_count, projectile projectiles[], int& projectile_count)
{
    if (fire_cooldown > 0.f)
    {
        return;
    }
    enemy* target = find_closest_enemy(enemies, enemy_count);
    if (!target)
    {
        return;
    }

    if (projectile_count < max_projectiles)
    {
        projectiles[projectile_count++] = projectile(x, y, target->get_x(), target->get_y(), 300.f, damage, Color(255, 140, 0));//projectile controls of cannon
    }
    fire_cooldown = 1.f / fire_rate;
}

void cannon_tower::render(RenderWindow& win)  // cannon tower visuals controls
{
    CircleShape base(16.f);
    base.setFillColor(color);
    base.setOrigin(16.f, 16.f);
    base.setPosition(x, y);

    RectangleShape barrel({ 20.f, 6.f });
    barrel.setFillColor(Color(100, 50, 0));
    barrel.setOrigin(0.f, 3.f);
    barrel.setPosition(x, y);
  
    CircleShape rangeCircle(range);
    rangeCircle.setFillColor(Color(0, 0, 0, 0)); 
    rangeCircle.setOutlineThickness(1.f);
    rangeCircle.setOutlineColor(Color(100, 100, 255, 120)); 
    rangeCircle.setOrigin(range, range);
    rangeCircle.setPosition(x, y);

    win.draw(rangeCircle);
    float start_x = x - 10.f;

    for (int i = 0; i < level; i++)
    {
        CircleShape star(3.f);

        star.setFillColor(Color::Yellow);

        star.setOrigin(3.f, 3.f);

        star.setPosition(
            start_x + i * 10.f,
            y - 22.f
        );

        win.draw(star);
    }


    win.draw(base);
    win.draw(barrel);
}

sniper_tower::sniper_tower(float x, float y) //sinper tower controls
    : tower(x, y, 250.f, 0.5f, 120, 150, Color(0, 0, 200))
{
}

void sniper_tower::attack(enemy* enemies[], int enemy_count, projectile projectiles[], int& projectile_count)
{
    if (fire_cooldown > 0.f)
    {
        return;
    }
    enemy* target = find_closest_enemy(enemies, enemy_count);
    if (!target)
    {
        return;
    }

    if (projectile_count < max_projectiles)
    {
        projectiles[projectile_count++] = projectile(x, y, target->get_x(), target->get_y(), 600.f, damage, Color(0, 100, 255));  // sniper tower projectiles controls
    }
    fire_cooldown = 1.f / fire_rate;
}

void sniper_tower::render(RenderWindow& win) // sniper tower visual controls
{
    CircleShape base(14.f, 8);
    base.setFillColor(color);
    base.setOrigin(14.f, 14.f);
    base.setPosition(x, y);

    RectangleShape barrel({ 26.f, 4.f });
    barrel.setFillColor(Color(0, 0, 100));
    barrel.setOrigin(0.f, 2.f);
    barrel.setPosition(x, y);

    CircleShape rangeCircle(range);
    rangeCircle.setFillColor(Color(0, 0, 0, 0)); 
    rangeCircle.setOutlineThickness(1.f);
    rangeCircle.setOutlineColor(Color(100, 100, 255, 120)); 
    rangeCircle.setOrigin(range, range);
    rangeCircle.setPosition(x, y);
    float start_x = x - 10.f;

    for (int i = 0; i < level; i++)
    {
        CircleShape star(3.f);

        star.setFillColor(Color::Yellow);

        star.setOrigin(3.f, 3.f);

        star.setPosition(
            start_x + i * 10.f,
            y - 22.f
        );

        win.draw(star);
    }

    win.draw(rangeCircle);

    win.draw(base);
    win.draw(barrel);
}

machinegun_tower::machinegun_tower(float x, float y)// machine gun controls
    : tower(x, y, 100.f, 5.f, 15, 120, Color(80, 80, 80))
{
}

void machinegun_tower::attack(enemy* enemies[], int enemy_count, projectile projectiles[], int& projectile_count)
{
    if (fire_cooldown > 0.f)
    {
        return;
    }
    enemy* target = find_closest_enemy(enemies, enemy_count);
    if (!target)
    {
        return;
    }

    if (projectile_count < max_projectiles)
    {
        projectiles[projectile_count++] = projectile(x, y, target->get_x(), target->get_y(), 500.f, damage, Color(200, 200, 200)); // machine gun projectile controls
    }
    fire_cooldown = 1.f / fire_rate;
}

void machinegun_tower::render(RenderWindow& win) // machine gun visual controls
{
    RectangleShape base({ 28.f, 28.f });
    base.setFillColor(color);
    base.setOrigin(14.f, 14.f);
    base.setPosition(x, y);

    RectangleShape barrel({ 22.f, 5.f });
    barrel.setFillColor(Color(50, 50, 50));
    barrel.setOrigin(0.f, 2.5f);
    barrel.setPosition(x, y);
    CircleShape rangeCircle(range);
    rangeCircle.setFillColor(Color(0, 0, 0, 0)); 
    rangeCircle.setOutlineThickness(1.f);
    rangeCircle.setOutlineColor(Color(100, 100, 255, 120)); 
    rangeCircle.setOrigin(range, range);
    rangeCircle.setPosition(x, y);
    float start_x = x - 10.f;

    for (int i = 0; i < level; i++)
    {
        CircleShape star(3.f);

        star.setFillColor(Color::Yellow);

        star.setOrigin(3.f, 3.f);

        star.setPosition(
            start_x + i * 10.f,
            y - 22.f
        );

        win.draw(star);
    }

    win.draw(rangeCircle);

    win.draw(base);
    win.draw(barrel);
}

slow_tower::slow_tower(float x, float y) // slow tower controls
    : tower(x, y, 110.f, 1.5f, 5, 90, Color(0, 200, 200)) {
}

void slow_tower::attack(enemy* enemies[], int enemy_count, projectile projectiles[], int& projectile_count) {
    if (fire_cooldown > 0.f)
    {
        return;
    }
    enemy* target = find_closest_enemy(enemies, enemy_count);
    if (!target)
    {
        return;
    }

    if (projectile_count < max_projectiles)
    {
        projectiles[projectile_count++] = projectile(x, y, target->get_x(), target->get_y(), 250.f, damage, Color(0, 255, 255), 0.5f);  // slow tower projectile controls
    }
    fire_cooldown = 1.f / fire_rate;
}

void slow_tower::render(RenderWindow& win) // slow tower visual controls
{
    CircleShape base(15.f, 6);
    base.setFillColor(color);
    base.setOrigin(15.f, 15.f);
    base.setPosition(x, y);
    CircleShape rangeCircle(range);
    rangeCircle.setFillColor(Color(0, 0, 0, 0)); 
    rangeCircle.setOutlineThickness(1.f);
    rangeCircle.setOutlineColor(Color(100, 100, 255, 120)); 
    rangeCircle.setOrigin(range, range);
    rangeCircle.setPosition(x, y);
    float start_x = x - 10.f;

    for (int i = 0; i < level; i++)
    {
        CircleShape star(3.f);

        star.setFillColor(Color::Yellow);

        star.setOrigin(3.f, 3.f);

        star.setPosition(
            start_x + i * 10.f,
            y - 22.f
        );

        win.draw(star);
    }

    win.draw(rangeCircle);
    win.draw(base);
}

bomb_tower::bomb_tower(float x, float y) // bomb tower controls
    : tower(x, y, 130.f, 0.6f, 60, 180, Color(220, 50, 50))
{
}

void bomb_tower::attack(enemy* enemies[], int enemy_count, projectile projectiles[], int& projectile_count)
{
    if (fire_cooldown > 0.f)
    { 
        return;
    }
    enemy* target = find_closest_enemy(enemies, enemy_count);
    if (!target)
    {
        return;
    }

    if (projectile_count < max_projectiles)
    {
        projectiles[projectile_count++] = projectile(x, y, target->get_x(), target->get_y(), 280.f, damage, Color(255, 80, 0)); // bomb tower projectile controls
    }
    fire_cooldown = 1.f / fire_rate;
}

void bomb_tower::render(RenderWindow& win)// bomb tower visual controls
{
    CircleShape base(16.f);
    base.setFillColor(color);
    base.setOrigin(16.f, 16.f);
    base.setPosition(x, y);

    CircleShape top(6.f);
    top.setFillColor(Color(255, 200, 0));
    top.setOrigin(6.f, 6.f);
    top.setPosition(x, y - 10.f);
    CircleShape rangeCircle(range);
    rangeCircle.setFillColor(Color(0, 0, 0, 0)); 
    rangeCircle.setOutlineThickness(1.f);
    rangeCircle.setOutlineColor(Color(100, 100, 255, 120)); 
    rangeCircle.setOrigin(range, range);
    rangeCircle.setPosition(x, y);
    float start_x = x - 10.f;

    for (int i = 0; i < level; i++)
    {
        CircleShape star(3.f);

        star.setFillColor(Color::Yellow);

        star.setOrigin(3.f, 3.f);

        star.setPosition(
            start_x + i * 10.f,
            y - 22.f
        );

        win.draw(star);
    }

    win.draw(rangeCircle);

    win.draw(base);
    win.draw(top);
}
