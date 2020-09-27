#ifndef MYLIBRARY_HPP
#define MYLIBRARY_HPP
#include <array>

std::array<double,3> convert_rgb_to_decimal_rgb(const std::array<int,3> &rgb);

std::array<double,3> convert_decimal_rgb_to_CIE_XYZ(const std::array<double,3> &rgbDecimal);

std::array<double,3> convert_CIE_XYZ_to_CIE_Lab(const std::array<double,3> &CIEXYZ);

double convert_CIE_Lab_to_Delta_E_76(const std::array<double,3> &CIELab1, const std::array<double,3> &CIELab2);

double calculate_delta_e76(const std::array<int,3> &RGB1, const std::array<int,3> &RGB2);

#endif
