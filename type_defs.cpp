//
// Created by smacdonald on 14/04/2021.
//

#include "type_defs.hpp"


int image::x_res = 1920;
int image::y_res = 1080;
std::vector<std::vector<uint32_t>> image::IMAGE(y_res, std::vector<uint32_t>(x_res));
