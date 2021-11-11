//
// Created by ultimatenick on 16/08/21.
//

#ifndef CPP_COSTANTS_HPP
#define CPP_COSTANTS_HPP

namespace CraftCpp {
    static constexpr int CHUNK_SIZE = 64;

#ifdef NDEBUG
    static constexpr int CREATE_CHUNK_RADIUS = 3;
#else
    static constexpr int CREATE_CHUNK_RADIUS = 0;
#endif
    static constexpr int RENDER_CHUNK_RADIUS = (CREATE_CHUNK_RADIUS > 0) ? CREATE_CHUNK_RADIUS - 1 : 1;
    static constexpr int DELETE_CHUNK_RADIUS = CREATE_CHUNK_RADIUS + 3;

    static constexpr double SCROLL_TRESHOLD = 0.1;
    static constexpr int ALIGN_LEFT = 0;
    static constexpr int ALIGN_CENTER = 1;
    static constexpr int ALIGN_RIGHT = 2;


    static constexpr bool SHOW_PLANTS = true;
    static constexpr bool SHOW_TREES = true;
    static constexpr bool SHOW_CLOUDS = true;

    static constexpr int WINDOW_WIDTH = 1024;
    static constexpr int WINDOW_HEIGHT = 768;

    static constexpr bool FULLSCREEN = false;
    static constexpr bool SHOW_CROSSHAIRS = true;
    static constexpr bool SHOW_INFO_TEXT = true;
    static constexpr bool VSYNC = false;
    static constexpr float INITIAL_FOV = 45;
}

#endif //CPP_COSTANTS_HPP
