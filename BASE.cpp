#include "base.h"
using namespace sf;

entity::entity(float x_pos, float y_pos, int hp_value): x(x_pos), y(y_pos), hp(hp_value), max_hp(hp_value)
{
}

entity::~entity() = default;

void entity::take_damage(int dmg)
{
    hp -= dmg;
}

bool entity::is_alive() const
{
    return hp > 0;
}

float entity::get_x() const
{
    return x;
}

float entity::get_y() const 
{
    return y;
}

int entity::get_hp() const 
{
    return hp;
}

int entity::get_max_hp() const
{
    return max_hp;
}

void draw_hp_bar(RenderWindow& window, float x, float y, int hp, int max_hp, float width)
{
    RectangleShape bg({ width, 5.f });
    bg.setFillColor(Color::Red);  // background
    bg.setPosition(x - width / 2.f, y - 22.f);

    float ratio = static_cast<float>(hp) / static_cast<float>(max_hp);
    if (ratio < 0.f) 
    {
        ratio = 0.f;
    }

    RectangleShape fg({ width * ratio, 5.f });   //frontground
    fg.setFillColor(Color::Green);
    fg.setPosition(x - width / 2.f, y - 22.f);

    window.draw(bg);
    window.draw(fg);
}
