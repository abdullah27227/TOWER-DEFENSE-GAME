#include "enemy.h"

#include <cmath>
using namespace std;
using namespace sf;

enemy::enemy(float x, float y, int hp, float speed, int reward, Color color): entity(x, y, hp), 
    speed(speed),
    base_speed(speed),
    reward(reward),
    waypoint_index(0),
    reached_end(false),
    color(color),
    enemy_path(nullptr),
    path_size(0)
{
}

enemy::~enemy() = default;

void enemy::set_path(const waypoint* p, int size) 
{
    enemy_path = p;
    path_size = size;
}

void enemy::update(float dt) 
{
    if (!enemy_path || waypoint_index >= path_size) 
    {
        reached_end = true;
        return;
    }

    const float tx = enemy_path[waypoint_index].x;
    const float ty = enemy_path[waypoint_index].y;
    const float dx = tx - x;
    const float dy = ty - y;
    const float dist = sqrt(dx * dx + dy * dy);

    if (dist < 5.f)
    {
        waypoint_index++;
        if (waypoint_index >= path_size)
        {
            reached_end = true;
        }
    }
    else 
    {
        x += (dx / dist) * speed * dt;
        y += (dy / dist) * speed * dt;
    }

    speed = base_speed;
}

void enemy::render(RenderWindow& win)
{
    CircleShape shape(12.f);
    shape.setFillColor(color);
    shape.setOrigin(12.f, 12.f);
    shape.setPosition(x, y);
    win.draw(shape);
    draw_hp_bar(win, x, y, hp, max_hp);
}

bool enemy::has_reached() const
{
    return reached_end;
}

int enemy::get_reward() const
{
    return reward;
}

void enemy::apply_slow(float amount) 
{
    speed = base_speed * (1.f - amount);
}

basic_enemy::basic_enemy(float x, float y) // color control of basic enemy
    : enemy(x, y, 100, 80.f, 10, Color::Red) 
{
}

fast_enemy::fast_enemy(float x, float y) // color control of fast enemy
    : enemy(x, y, 50, 180.f, 15, Color::Yellow) 
{
}

void fast_enemy::render(RenderWindow& win) 
{
    CircleShape shape(10.f, 3);
    shape.setFillColor(color);
    shape.setOrigin(10.f, 10.f);
    shape.setPosition(x, y);
    win.draw(shape);
    draw_hp_bar(win, x, y, hp, max_hp);
}

tank_enemy::tank_enemy(float x, float y) // color control of tank
    : enemy(x, y, 400, 40.f, 30, Color(150, 0, 150))
{
}

void tank_enemy::render(RenderWindow& win)
{
    RectangleShape shape({ 26.f, 26.f });
    shape.setFillColor(color);
    shape.setOrigin(13.f, 13.f);
    shape.setPosition(x, y);
    win.draw(shape);
    draw_hp_bar(win, x, y, hp, max_hp, 40.f);
}

flying_enemy::flying_enemy(float x, float y) // color control of flying enemy
    : enemy(x, y, 80, 120.f, 20, Color::Cyan) 
{
}

void flying_enemy::update(float dt)
{
    if (!enemy_path || path_size == 0)
    {
        reached_end = true;
        return;
    }

    const float tx = enemy_path[path_size - 1].x;
    const float ty = enemy_path[path_size - 1].y;
    const float dx = tx - x;
    const float dy = ty - y;
    const float dist = sqrt(dx * dx + dy * dy);

    if (dist < 5.f) 
    {
        reached_end = true;
    }
    else 
    {
        x += (dx / dist) * speed * dt;
        y += (dy / dist) * speed * dt;
    }

    speed = base_speed;
}

void flying_enemy::render(RenderWindow& win)
{
    CircleShape shape(10.f, 8);
    shape.setFillColor(color);
    shape.setOrigin(10.f, 10.f);
    shape.setPosition(x, y);

    CircleShape dot(3.f);
    dot.setFillColor(Color::Black);
    dot.setOrigin(3.f, 3.f);
    dot.setPosition(x, y);

    win.draw(shape);
    win.draw(dot);
    draw_hp_bar(win, x, y, hp, max_hp);
}

healer_enemy::healer_enemy(float x, float y) // color control of healer
    : enemy(x, y, 150, 70.f, 25, Color(0, 200, 0)), heal_timer(0.f) 
{
}

void healer_enemy::update(float dt)
{
    enemy::update(dt);
    heal_timer += dt;
    if (heal_timer >= 2.f) 
    {

        hp += 10;
        if (hp > max_hp)
        {
            hp = max_hp;
        }
        heal_timer = 0.f;
    }
}

void healer_enemy::render(RenderWindow& win) 
{
    CircleShape shape(12.f);
    shape.setFillColor(color);
    shape.setOrigin(12.f, 12.f);
    shape.setPosition(x, y);

    RectangleShape h_bar({ 14.f, 4.f }); // inner horizontal white line
    h_bar.setFillColor(Color::White);
    h_bar.setOrigin(7.f, 2.f);
    h_bar.setPosition(x, y);

    RectangleShape v_bar({ 4.f, 14.f });// inner vertical white line
    v_bar.setFillColor(Color::White);
    v_bar.setOrigin(2.f, 7.f);
    v_bar.setPosition(x, y);

    win.draw(shape);
    win.draw(h_bar);
    win.draw(v_bar);
    draw_hp_bar(win, x, y, hp, max_hp);
}
