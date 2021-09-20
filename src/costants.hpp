//
// Created by ultimatenick on 16/08/21.
//

#ifndef CPP_COSTANTS_HPP
static constexpr int MAX_CHUNKS = 500;
static constexpr int CHUNK_SIZE = 32;

static constexpr int INDICES_FACE_COUNT = 6;
static constexpr int VERTICES_FACE_COUNT = 4;
static constexpr float N = 0.5f;
static constexpr int RENDER_CHUNK_RADIUS = 2;
static constexpr int CREATE_CHUNK_RADIUS = 2;
static constexpr int DELETE_CHUNK_RADIUS = 14;

static constexpr double SCROLL_TRESHOLD = 0.1;
static constexpr int ALIGN_LEFT = 0;
static constexpr int ALIGN_CENTER = 1;
static constexpr int ALIGN_RIGHT = 2;

// config
static constexpr bool SHOW_LIGHT = true;
static constexpr bool SHOW_PLANTS = true;
static constexpr bool SHOW_TREES = true;
static constexpr bool SHOW_CLOUDS = true;

static constexpr int WINDOW_WIDTH = 1024;
static constexpr int WINDOW_HEIGTH = 768;

static constexpr bool FULLSCREEN = false;
static constexpr bool SHOW_WIREFRAME = false;
static constexpr bool SHOW_CROSSHAIRS = true;
static constexpr bool SHOW_INFO_TEXT = false;
static constexpr bool SHOW_ITEM = true;
static constexpr int DAY_LENGTH = 600;
static constexpr bool VSYNC = false;
static constexpr float INITIAL_FOV = 45;
#define CPP_COSTANTS_HPP

#endif //CPP_COSTANTS_HPP
