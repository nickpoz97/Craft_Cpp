//
// Created by ultimatenick on 10/08/21.
//

#include <cmath>
#include "Model.hpp"

float Model::get_day_time() const {
    if (day_length <= 0) {
        return 0.5;
    }
    float t;
    t = glfwGetTime();
    t = t / day_length;
    t = t - (int)t;
    return t;
}

float Model::get_daylight() const {
    float timer = get_day_time();
    if (timer < 0.5) {
        float t = (timer - 0.25) * 100;
        return 1 / (1 + powf(2, -t));
    }
    else {
        float t = (timer - 0.85) * 100;
        return 1 - 1 / (1 + powf(2, -t));
    }
}

int Model::get_scale_factor() const {
    int window_width, window_height;
    int buffer_width, buffer_height;

    // screen coordinates
    glfwGetWindowSize(window, &window_width, &window_height);
    // sizes in pixels
    glfwGetFramebufferSize(window, &buffer_width, &buffer_height);

    int result = buffer_width / window_width;
    result = std::max(1, result);
    result = std::min(2, result);
    return result;
}
