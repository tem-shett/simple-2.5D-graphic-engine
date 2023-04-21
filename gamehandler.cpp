#include <cassert>
#include <random>
#include <iostream>
#include <algorithm>
#include "gamehandler.h"
#include "texture.h"


void GameHandler::add_texture(const std::string filename, texture_type textureType) {
  auto texture = new Texture(filename);
  if (textureType == texture_type::WALL) {
    wall_textures.push_back(texture);
  } else if (textureType == texture_type::ENEMY) {
    enemy_textures.push_back(texture);
  }
}

void GameHandler::add_wall(Wall wall) {
  walls.push_back(wall);
}

GameHandler create_GameHandler() {
  GameHandler gh;
  // load textures
  std::vector<std::string> filenames_walls = {"legoOBI", "cfa2", "OBIdeath", "walltext", "razryv", "oblaka"};
  for (auto& filename : filenames_walls) {
    gh.add_texture("../bmp_files/" + filename + ".bmp", texture_type::WALL);
  }
  std::vector<std::string> filenames_enemies = {"baronada"};
  for (auto& filename : filenames_enemies) {
    gh.add_texture("../bmp_files/" + filename + ".bmp", texture_type::ENEMY);
  }
  std::mt19937 rnd_(773);

  // add walls
  std::vector<Wall> walls = {{*new Pt<double>(10, 5), *new Pt<double>(13, -6), 0, 10, nullptr}};
//                               {*new Pt<double>(-1000, 1000), *new Pt<double>(1000, 1000), -1000, 1000, nullptr},
//                               {*new Pt<double>(1000, 1000), *new Pt<double>(1000, -1000), -1000, 1000, nullptr},
//                               {*new Pt<double>(1000, -1000), *new Pt<double>(-1000, -1000), -1000, 1000, nullptr},
//                               {*new Pt<double>(-1000, -1000), *new Pt<double>(-1000, 1000), -1000, 1000, nullptr}};
  std::vector<uint8_t> tex_ind = {0};
  int kekek = 30, radius = 30000;
  for (int i = 0; i < kekek; i++) {
    double ang1 = 2 * pi / kekek * i, ang2 = 2 * pi / kekek * (i + 1);
    Wall wall(Pt<double>(cos(ang1)
    *radius, sin(ang1) * radius), Pt<double>(cos(ang2) * radius, sin(ang2) * radius), -2 * radius, 2 * radius,
            nullptr);
    wall.left_cut = i / (double) kekek, wall.right_cut = 1 - (i + 1) / (double) kekek;
    wall.texture = gh.wall_textures.back();
    gh.bg_walls.push_back(wall);
  }
//  gh.floors.push_back({{Pt<double>(-20, -20), Pt<double>(20, -20), Pt<double>(20, 20), Pt<double>(-20, 20)}, 0, 2179});
//  gh.floors.push_back(
//          {{Pt<double>(-20, -20), Pt<double>(20, -20), Pt<double>(20, 20), Pt<double>(-20, 20)}, 20, 22179});
//  for (int i = 0; i < 10; ++i) {
//    int mm = 20;
//    gh.floors.push_back({{Pt<double>(rnd_() % mm, rnd_() % mm), Pt<double>(rnd_() % mm, rnd_() % mm),
//                          Pt<double>(rnd_() % mm, rnd_() % mm)}, i * 2.0 + 1, (uint32_t) i * 10000 + 179});
//  }
//  {
//    int mm = 30;
//    std::vector<Pt<double>> pts;
//    for (int i = 0; i < 10; ++i) {
//      pts.push_back(Pt<double>(rnd_() % mm + mm, rnd_() % mm + mm));
//    }
//    gh.floors.push_back({pts, 10, (uint32_t) 150000 + 179});
//  }
  for (int i = 0; i < 50; i++) {
    int mm = 150;
    walls.push_back({Pt<double>(rnd_() % mm, rnd_() % mm), Pt<double>(rnd_() % mm, rnd_() % mm),
                     (double) (rnd_() % 5), (double) (rnd_() % 10) + 10,
                     nullptr});

    tex_ind.push_back(rnd_() % ((int) filenames_walls.size() - 1));
  }
  assert(tex_ind.size() >= walls.size());
  for (int i = 0; i < (int) walls.size(); i++) {
    walls[i].texture = gh.wall_textures[tex_ind[i]];
    walls[i].ind = i;
//        walls[i].right_cut = 3.0/6;
    gh.add_wall(walls[i]);
  }

  // add enemies
  std::vector<NPC> enemies = {{*new Pt<double>(3, 5), 0, 10, 3, gh.enemy_textures[0]}};
//    for (int i = 0; i < 20; i++) {
//        enemies.push_back({Pt<double>(rnd_() % 150, rnd_() % 150), 0, 10, 3, gh.enemy_textures[0]});
//    }
//    std::vector<NPC> enemies = {{*new Pt<double>(3, 5), 0, 10, 3, gh.enemy_textures[0]},
//                                {*new Pt<double>(10, 10), 0, 10, 3, gh.enemy_textures[0]}};
  for (int i = 0; i < (int) enemies.size(); i++) {
        erase_color(enemies[i].texture, enemies[i].texture->get_color(0, 0));
        gh.enemies.push_back(enemies[i]);
  }

  gh.root = bsp_algo(gh.walls, gh.floors);
  std::cout << "BSP size: " << gh.root->cnt_walls << std::endl;
  return gh;
}

bool is_bad_wall(Wall wall, Pt<double> pos1, Pt<double> pos2, double min_dist) {
  auto dist = get_dist_from_wall_signed(pos2, wall);
  if ((is_beyond_wall(pos1, wall) || is_beyond_wall(pos2, wall)) && std::abs(dist) > min_dist) {
    return false;
  }
  auto dist_prev = get_dist_from_wall_signed(pos1, wall);
  bool flag_bad_wall = (dist > 0 && dist_prev < 0) || (dist < 0 && dist_prev > 0);
  dist = std::abs(dist), dist_prev = std::abs(dist_prev);
  return flag_bad_wall || (dist < min_dist && dist < dist_prev);
}

void
get_bad_walls_with_bsp(const std::shared_ptr<BSP_node>& node, Pt<double> npos, double h1, double h2, double min_dist,
                       std::vector<int>& potential_bad_walls) {
  if (!node) {
    return;
  }
  for (auto wall : node->cur_walls) {
    if (wall.h1 > h2 || wall.h2 < h1) {
      continue;
    }
    if (std::abs(get_dist_from_wall_signed(npos, wall)) < 5 * min_dist) {
      potential_bad_walls.push_back(wall.ind);
    }
  }
  get_bad_walls_with_bsp(node->ln.get_dist(npos) > 0 ? node->above : node->under, npos, h1, h2, min_dist,
                         potential_bad_walls);
}

Pt<double> GameHandler::get_next_pos(Pt<double> pos, double h1, double h2, Pt<double> ch, double min_dist,
                                     bool is_player = false) {
  std::vector<int> potential_bad_walls;
  get_bad_walls_with_bsp(root, pos + ch, h1, h2, min_dist, potential_bad_walls);
//    for (Wall wall : walls) {
//        if (wall.h1 > h2 || wall.h2 < h1) {
//            continue;
//        }
//        if (std::abs(wall.ln.get_dist(pos + ch)) < 5 * min_dist) {
//            potential_bad_walls.push_back(wall.ind);
//        }
//    }
  int sz_pbw = (int) potential_bad_walls.size();
  for (int i = 0; i < sz_pbw; i++) {
    potential_bad_walls.push_back(potential_bad_walls[i]);
  }
  auto real_ch = Pt<double>(0, 0);
  for (int _ = 0; _ < 2; _++) {
    std::reverse(potential_bad_walls.begin(), potential_bad_walls.end());
    auto ch_now = ch;
    int cnt_bad_walls = 0;
    auto new_pos = pos + ch_now;
    for (auto ind : potential_bad_walls) {
      auto wall = walls[ind];
      if (is_bad_wall(wall, pos, new_pos, min_dist)) {
        if (cnt_bad_walls >= 1) {
          ch_now.x = 0, ch_now.y = 0;
          break;
        }
        cnt_bad_walls++;
        double ang = atan2(-wall.ln.a, wall.ln.b);
        ch_now = ch_now.rotate(-ang);
        if (is_player) {
          double ll = 0, rr = ch_now.y;
          for (int q = 0; q < 15; q++) {
            double mid = (ll + rr) / 2;
            auto now_npos = pos + Pt<double>(ch_now.x, mid).rotate(ang);
            if (is_bad_wall(wall, pos, now_npos, min_dist)) {
              rr = mid;
            } else {
              ll = mid;
            }
          }
          ch_now.y = ll;
        } else {
          ch_now.y = 0;
        }
        ch_now = ch_now.rotate(ang);
        new_pos = pos + ch_now;
      }
    }
    if (real_ch.dist() < ch_now.dist()) {
      real_ch = ch_now;
    }
  }
  return pos + real_ch;
}

void GameHandler::player_move(float time_pass) {
//    std::cout << player.pos.x << ' ' << player.pos.y << std::endl;
//    std::cout << player.eye_angle << std::endl;
//    std::cout << player.h << std::endl;
  if (player.eye_angle > 2 * pi) {
    player.eye_angle -= 2 * pi;
  } else if (player.eye_angle < 0) {
    player.eye_angle += 2 * pi;
  }
  player.eye_angle += player.ch_ang * time_pass;
  // TODO: if there is no wall above/under
  player.h += player.ch_h * time_pass;
  //
  //
  player.change_real_speed(time_pass);
  double norm_speed = hypot(player.real_ch_x, player.real_ch_y);
  if (norm_speed < epsilon) {
    norm_speed = 1000;
  } else {
    norm_speed = (std::abs(player.real_ch_x) + std::abs(player.real_ch_y)) / norm_speed;
  }
  Pt<double> ch(player.real_ch_x / norm_speed * time_pass, player.real_ch_y / norm_speed * time_pass);
  ch = ch.rotate(player.eye_angle) * player.speed;
  player.pos = get_next_pos(player.pos, player.h - player.h_under_eye, player.h + player.h_above_eye, ch,
                            player.min_dist_from_wall, true);
}

void GameHandler::move(float time_pass) {
  player_move(time_pass);
  for (auto& enemy : enemies) {
    auto vec = (player.pos - enemy.pos);
    vec = vec / vec.dist() * time_pass * enemy.speed;
//        enemy.pos = enemy.pos + vec;
    enemy.pos = get_next_pos(enemy.pos, enemy.h, enemy.h + enemy.height, vec, 0.05 + enemy.width / 2);
  }
}