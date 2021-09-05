//
// Created by ultimatenick on 16/08/21.
//

#ifndef CPP_COSTANTS_HPP
static constexpr float PI = static_cast<float>(3.14159265359);
static constexpr int MAX_CHUNKS = 8192;
static constexpr int WORKERS = 8192;
static constexpr int MAX_TEXT_LENGTH = 256;
static constexpr int MAX_MESSAGES = 256;
static constexpr int CHUNK_SIZE = 32;
static constexpr int MAX_SIGN_WIDTH = 64;
static constexpr float SIGN_LINE_HEIGHT = 1.25;
static constexpr int N_SIGN_LINES = 1024;
static constexpr int MAX_LINE_LENGTH = 1024;

static constexpr int INDICES_FACE_COUNT = 6;
static constexpr int VERTICES_FACE_COUNT = 4;
static constexpr float N = 0.5f;
static constexpr int RENDER_CHUNK_RADIUS = 4;
static constexpr double SCROLL_TRESHOLD = 0.1;

// config
static constexpr bool SHOW_LIGHT = true;
static constexpr bool SHOW_PLANTS = true;
static constexpr bool SHOW_TREES = true;
static constexpr bool SHOW_CLOUDS = true;

static constexpr int WINDOW_WIDTH = 1024;
static constexpr int WINDOW_HEIGTH = 768;

static constexpr bool FULLSCREEN = false;
#define CPP_COSTANTS_HPP

#endif //CPP_COSTANTS_HPP
