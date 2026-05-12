#include "map.h"

#include <cmath>
#include <iostream>
using namespace std;
using namespace sf;

void game::setup_map() 
{

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            grid[r][c] = grass;
        }
    }

 
    for (int c = 0; c <= 18; c++)
        grid[1][c] = path;

    for (int r = 1; r <= 12; r++)
        grid[r][18] = path;

    for (int c = 18; c >= 2; c--)
        grid[12][c] = path;

    for (int r = 12; r >= 3; r--)
        grid[r][2] = path;

    for (int c = 2; c <= 15; c++)
        grid[3][c] = path;

    for (int r = 3; r <= 9; r++)
        grid[r][15] = path;

    for (int c = 15; c >= 5; c--)
        grid[9][c] = path;

    for (int r = 9; r >= 5; r--)
        grid[r][5] = path;

    for (int c = 5; c <= 11; c++)
        grid[5][c] = path;


    waypoint_count = 0;
   

    add_wp(0, 1);
    add_wp(18, 1);
    add_wp(18, 12);
    add_wp(2, 12);
    add_wp(2, 3);
    add_wp(15, 3);
    add_wp(15, 9);
    add_wp(5, 9);
    add_wp(5, 5);
    add_wp(11, 5);
}
void game::add_wp(int col, int row)
{
    waypoints[waypoint_count].x = col * tile_size + tile_size / 2.f;
    waypoints[waypoint_count].y = row * tile_size + tile_size / 2.f;
    waypoint_count++;
}
void game::setup_waves()// waves and enemy count control
{
    waves[0].count = 8; waves[0].spawn_delay = 1.5f;
    for (int i = 0; i < 8; i++)
    { 
        waves[0].enemy_types[i] = 0;
    }

    waves[1].count = 9; waves[1].spawn_delay = 1.2f;
    for (int i = 0; i < 6; i++)
    {
        waves[1].enemy_types[i] = 0; 
    }
    for (int i = 6; i < 9; i++)
    { 
        waves[1].enemy_types[i] = 1;
    }

    waves[2].count = 10; waves[2].spawn_delay = 1.0f;
    for (int i = 0; i < 5; i++) { waves[2].enemy_types[i] = 0; }
    for (int i = 5; i < 8; i++) { waves[2].enemy_types[i] = 1; }
    for (int i = 8; i < 10; i++) { waves[2].enemy_types[i] = 2; }

    waves[3].count = 12; waves[3].spawn_delay = 0.9f;
    for (int i = 0; i < 4; i++) { waves[3].enemy_types[i] = 0; }
    for (int i = 4; i < 7; i++) { waves[3].enemy_types[i] = 1; }
    for (int i = 7; i < 9; i++) { waves[3].enemy_types[i] = 2; }
    for (int i = 9; i < 11; i++) { waves[3].enemy_types[i] = 3; }
    waves[3].enemy_types[11] = 4;

    waves[4].count = 15; waves[4].spawn_delay = 0.7f;
    for (int i = 0; i < 3; i++) { waves[4].enemy_types[i] = 0; }
    for (int i = 3; i < 6; i++) { waves[4].enemy_types[i] = 1; }
    for (int i = 6; i < 10; i++) { waves[4].enemy_types[i] = 2; }
    for (int i = 10; i < 13; i++) { waves[4].enemy_types[i] = 3; }
    for (int i = 13; i < 15; i++) { waves[4].enemy_types[i] = 4; }
}

void game::setup_shop() 
{
    shop_costs[0] = 100; shop_names[0] = "CANNON $100 \n(L2 $50,L3 $100)";
    shop_costs[1] = 150; shop_names[1] = "SNIPER $150 \n(L2 $75,L3 $150)";
    shop_costs[2] = 120; shop_names[2] = "MACHINE GUN \n$120 (L2 $60,L3 $120)";
    shop_costs[3] = 90;  shop_names[3] = "SLOW GUN $90 \n(L2 $45,L3 $90)";
    shop_costs[4] = 180; shop_names[4] = "BOMBER $180 \n(L2 $90,L3 $180)";
}

void game::spawn_enemy(int type)
{
    if (enemy_count >= max_enemies)
    {
        return;
    }

    const float sx = waypoints[0].x;
    const float sy = waypoints[0].y;

    enemy* e = nullptr;
    switch (type)
    {
    case 0:
    {
        e = new basic_enemy(sx, sy); break; 
    }
    case 1:
    { 
        e = new fast_enemy(sx, sy); break; 
    }
    case 2:
    { 
        e = new tank_enemy(sx, sy); break;
    }

    case 3:
    { 
        e = new flying_enemy(sx, sy); break;
    }
    case 4:
    { 
        e = new healer_enemy(sx, sy); break;
    }
    default: break;
    }

    if (e) {
        e->set_path(waypoints, waypoint_count);
        enemies[enemy_count++] = e;
    }
}

bool game::can_place_tower(int tx, int ty) const {
    if (tx < 0 || tx >= cols || ty < 0 || ty >= rows) return false;
    return grid[ty][tx] == grass;
}

void game::place_tower(int tile_x, int tile_y) {
    if (!can_place_tower(tile_x, tile_y)) return;
    if (selected_tower < 0 || selected_tower >= shop_items) return;
    if (gold < shop_costs[selected_tower]) return;
    if (tower_count >= max_towers) return;

    const float px = tile_x * tile_size + tile_size / 2.f;
    const float py = tile_y * tile_size + tile_size / 2.f;

    tower* t = nullptr;
    switch (selected_tower) {
    case 0: t = new cannon_tower(px, py); break;
    case 1: t = new sniper_tower(px, py); break;
    case 2: t = new machinegun_tower(px, py); break;
    case 3: t = new slow_tower(px, py); break;
    case 4: t = new bomb_tower(px, py); break;
    default: break;
    }

    if (t) {
        towers[tower_count++] = t;
        gold -= shop_costs[selected_tower];
        grid[tile_y][tile_x] = blocked;
    }
}

void game::upgrade_tower(int tile_x, int tile_y) {
    const float cx = tile_x * tile_size + tile_size / 2.f;
    const float cy = tile_y * tile_size + tile_size / 2.f;

    for (int i = 0; i < tower_count; i++) {
        if (!towers[i]) continue;
        const bool same_tile = fabs(towers[i]->get_x() - cx) < 1.f && fabs(towers[i]->get_y() - cy) < 1.f;
        if (!same_tile) continue;

        if (!towers[i]->can_upgrade()) return;
        const int cost = towers[i]->get_upgrade_cost();
        if (gold < cost) return;

        gold -= cost;
        towers[i]->upgrade_stats();
        return;
    }
}

void game::check_collisions() 
{
    for (int p = 0; p < projectile_count; p++) {
        if (!projectiles[p].is_active()) continue;

        for (int e = 0; e < enemy_count; e++) {
            if (!enemies[e] || !enemies[e]->is_alive()) continue;

            const float dx = projectiles[p].get_x() - enemies[e]->get_x();
            const float dy = projectiles[p].get_y() - enemies[e]->get_y();
            const float dist = sqrt(dx * dx + dy * dy);

            if (dist < 15.f) {
                enemies[e]->take_damage(projectiles[p].get_damage());
                if (projectiles[p].get_slow_amount() > 0.f) {
                    enemies[e]->apply_slow(projectiles[p].get_slow_amount());
                }

                if (!enemies[e]->is_alive()) {
                    gold += enemies[e]->get_reward();
                }

                score += enemies[e]->get_reward();
                projectiles[p].deactivate();
                break;
            }
        }
    }
}

void game::remove_dead_enemies() {
    int new_count = 0;
    for (int i = 0; i < enemy_count; i++) {
        if (enemies[i] && enemies[i]->is_alive()) {
            enemies[new_count++] = enemies[i];
        }
        else {
            delete enemies[i];
            enemies[i] = nullptr;
        }
    }
    enemy_count = new_count;
}

void game::remove_inactive_projectiles() {
    int new_count = 0;
    for (int i = 0; i < projectile_count; i++) {
        if (projectiles[i].is_active()) {
            projectiles[new_count++] = projectiles[i];
        }
    }
    projectile_count = new_count;
}

void game::draw_map()
{
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            RectangleShape tile({ static_cast<float>(tile_size) - 1, static_cast<float>(tile_size) - 1 });
            tile.setPosition(static_cast<float>(c * tile_size), static_cast<float>(r * tile_size));
            if (grid[r][c] == path)
            {
                tile.setFillColor(Color(170, 210, 255));
            }

            else if (grid[r][c] == blocked)
            {
                tile.setFillColor(Color(120, 140, 170));
            }

            else
            {
                tile.setFillColor(Color(225, 240, 255));
            }

            tile.setOutlineThickness(1.f);
            tile.setOutlineColor(Color(180, 200, 220));
            window.draw(tile);
        }
    }
}

void game::draw_text(const string& text, float x, float y, int size , Color col) {
    if (!font_loaded) return;
    Text t;
    t.setFont(font);
    t.setString(text);
    t.setCharacterSize(size);
    t.setFillColor(col);
    t.setPosition(x, y);
    window.draw(t);
}

void game::draw_ui()
{
    RectangleShape bar({ static_cast<float>(win_width), static_cast<float>(ui_height) });
    bar.setFillColor(Color(20, 20, 20));
    bar.setPosition(0.f, static_cast<float>(rows * tile_size));
    window.draw(bar);

    const float button_w = win_width / static_cast<float>(shop_items);
    for (int i = 0; i < shop_items; i++) {
        Color col(80, 80, 80);
        if (i == selected_tower) col = Color(200, 200, 120);
        if (gold < shop_costs[i]) col = Color(60, 60, 60);

        RectangleShape button({ button_w - 4.f, 50.f });
        button.setPosition(i * button_w + 2.f, static_cast<float>(rows * tile_size) + 5.f);
        button.setFillColor(col);
        window.draw(button);

        draw_text(shop_names[i], i * button_w + 5.f, static_cast<float>(rows * tile_size) + 10.f, 20); // adjust text position on button
    }

    draw_text("Score: " + to_string(score), 10.f, 5.f, 30, Color::Black);
    draw_text("Gold: " + to_string(gold) + "$", 150.f, 5.f, 30, Color::Black);
    draw_text("Lives: " + to_string(lives), 300.f, 5.f,30, Color::Black);
    draw_text("Wave: " + to_string(current_wave) + "/" + to_string(max_waves), 450.f, 5.f,30, Color::Black);
    draw_text("Right Click to Upgrade TOWER", 10.f, 850.f, 30, Color::Black);
}

void game::draw_game_over()
{
    window.clear(Color(20, 0, 0));
    draw_text("GAME OVER...You lose!", win_width / 2.f - 250.f, win_height / 2.f - 80.f, 60, Color::Red);
    draw_text("Final Score: " + to_string(score), win_width / 2.f - 120.f, win_height / 2.f + 10.f, 24);
}

void game::draw_win()
{
    window.clear(Color(0, 20, 0));
    draw_text("GAME OVER...You Won!", win_width / 2.f - 250.f, win_height / 2.f - 80.f, 60, Color::Green);
    draw_text("Final Score: " + to_string(score), win_width / 2.f - 120.f, win_height / 2.f + 10.f, 24);
}

game::game()// settung initial conditions
    : window(VideoMode(win_width, win_height), "TOWER DEFENCE GAME BY Muhammad Abdullah", Style::Titlebar | Style::Close),
    font_loaded(false),
    waypoint_count(0),
    enemy_count(0),
    tower_count(0),
    projectile_count(0),
    current_wave(0),
    spawn_index(0),
    spawn_timer(0.f),
    wave_active(false),
    wave_pause_timer(3.f),
    gold(150),
    lives(20),
    score(0),
    game_over(false),
    game_won(false),
    selected_tower(-1) 
{
    window.setFramerateLimit(60);
    if (font.loadFromFile("Abdullah.ttf")) 
    {
        font_loaded = true;
    }

    for (int i = 0; i < max_enemies; i++)
    {
        enemies[i] = nullptr;
    }
    for (int i = 0; i < max_towers; i++)
    {
        towers[i] = nullptr;
    }

    setup_map();
    setup_waves();
    setup_shop();
}

game::~game() {
    for (int i = 0; i < max_enemies; i++) {
        delete enemies[i];
    }
    for (int i = 0; i < max_towers; i++) {
        delete towers[i];
    }
}

void game::run()
{
    Clock clock;

    while (window.isOpen()) 
    {
        float dt = clock.restart().asSeconds();
        if (dt > 0.05f)
        {
            dt = 0.05f;
        }

        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed) window.close();

            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) 
            {
                selected_tower = -1;
            }

            if (event.type == Event::MouseButtonPressed) 
            {
                const int mx = event.mouseButton.x;
                const int my = event.mouseButton.y;
                const int tx = mx / tile_size;
                const int ty = my / tile_size;

                if (event.mouseButton.button == Mouse::Left)
                {
                    if (my >= rows * tile_size) 
                    {
                        const float button_w = win_width / static_cast<float>(shop_items);
                        const int button = static_cast<int>(mx / button_w);
                        if (button >= 0 && button < shop_items) 
                        {
                            if (selected_tower == button)
                            {
                                selected_tower = -1;
                            }
                            else
                            {
                                selected_tower = button;
                            }
                        }
                    }
                    else if (selected_tower >= 0)
                    {
                        place_tower(tx, ty);
                    }
                }

                if (event.mouseButton.button == Mouse::Right && my < rows * tile_size)
                {
                    upgrade_tower(tx, ty);
                }
            }
        }

        if (!game_over && !game_won) 
        {
            if (!wave_active) 
            {
                wave_pause_timer -= dt;
                if (wave_pause_timer <= 0.f && current_wave < max_waves)
                {
                    wave_active = true;
                    spawn_index = 0;
                    spawn_timer = 0.f;
                }
                if (current_wave >= max_waves && enemy_count == 0) 
                {
                    game_won = true;
                }
            }

            if (wave_active)
            {
                spawn_timer -= dt;
                if (spawn_timer <= 0.f && spawn_index < waves[current_wave].count)
                {
                    spawn_enemy(waves[current_wave].enemy_types[spawn_index]);
                    spawn_index++;
                    spawn_timer = waves[current_wave].spawn_delay;
                }
                if (spawn_index >= waves[current_wave].count && enemy_count == 0)
                {
                    wave_active = false;
                    current_wave++;
                    const int bonus = current_wave * 50;
                    score += bonus;
                    gold += bonus;
                    wave_pause_timer = 4.f;
                }
            }

            for (int i = 0; i < enemy_count; i++)
            {
                if (!enemies[i])
                {
                    continue;
                }
                enemies[i]->update(dt);
                if (enemies[i]->has_reached())
                {
                    lives--;
                    delete enemies[i];
                    enemies[i] = nullptr;
                }
            }

            for (int i = 0; i < tower_count; i++)
            {
                if (!towers[i]) 
                {
                    continue;
                }
                towers[i]->update(dt);
                towers[i]->attack(enemies, enemy_count, projectiles, projectile_count);
            }

            for (int i = 0; i < projectile_count; i++)
            {
                projectiles[i].update(dt);
            }

            check_collisions();
            remove_dead_enemies();
            remove_inactive_projectiles();

            if (lives <= 0)
            {
                game_over = true;
            }
        }

        window.clear(Color(30, 30, 30));

        if (game_over)
        {
            draw_game_over();
            window.display();
            continue;
        }
        if (game_won)
        {
            draw_win();
            window.display();
            continue;
        }

        draw_map();
        for (int i = 0; i < tower_count; i++)
        {
            if (towers[i])
            {
                towers[i]->render(window);
            }
        }
        for (int i = 0; i < enemy_count; i++)
        { 
            if (enemies[i])
            {
                enemies[i]->render(window);
            }
        }
        for (int i = 0; i < projectile_count; i++)
        {
            if (projectiles[i].is_active())
            {
                projectiles[i].render(window);
            }
        }
        draw_ui();
        window.display();
    }
}
