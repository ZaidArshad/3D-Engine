#pragma once
#include <string>

const std::string STR_OBJ_VERTEX_POSITION = "v";
const std::string STR_OBJ_VERTEX_NORMAL = "vn";
const std::string STR_OBJ_VERTEX_TEXTURE = "vt";
const std::string STR_OBJ_FACE = "f";

constexpr unsigned int NUM_OBJ_VERTEX_COORDS = 3;
constexpr unsigned int NUM_OBJ_TEXTURE_COORDS = 2;
constexpr unsigned int NUM_OBJ_NORMAL_COORDS = 3;

constexpr unsigned int ANIMATION_FRAME_GAP = 5;