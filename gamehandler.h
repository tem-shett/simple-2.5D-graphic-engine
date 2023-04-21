#ifndef UNTITLED23_GAMEHANDLER_H
#define UNTITLED23_GAMEHANDLER_H

#include <string>
#include "gameobjects.h"
#include "geom_funcs.h"
#include "bsp.h"

enum texture_type {
    WALL, ENEMY
};

struct GameHandler {
    Player player;
    std::vector<Wall> walls;
    std::vector<Wall> bg_walls;
    std::vector<Floor> floors;
    std::vector<NPC> enemies;
    std::vector<Texture *> wall_textures, enemy_textures;
    std::shared_ptr<BSP_node> root = nullptr;

    GameHandler() : player(Player(Pt<double>(20.8418, -20.0389), 2.008, 4)) {};

    void add_texture(const std::string filename, texture_type textureType);

    void add_wall(Wall wall);

    Pt<double> get_next_pos(Pt<double> pos, double h1, double h2, Pt<double> ch, double min_dist, bool is_player);

    void player_move(float time_pass);

    void move(float time_pass);
};

GameHandler create_GameHandler();

#endif //UNTITLED23_GAMEHANDLER_H
