#include "myLibrary.hpp"
#include <array>
#include <cmath>
#include <iostream>

std::array<double,3> convert_rgb_to_decimal_rgb(const std::array<int,3> &rgb)
{
  std::array<double,3> decimalRGB;
  double highestPossibleValue{255.0};

  for(size_t index{0}; index < rgb.size(); index++)
   decimalRGB[index]=rgb[index]/highestPossibleValue;

  return decimalRGB;
}

std::array<double,3> convert_decimal_rgb_to_CIE_XYZ(const std::array<double,3> &rgbDecimal)
{
    std::array<double,3> linearizedRGB, CIEXYZ, multiplicationValues1{.4124564,.2126729,.0193339},multiplicationValues2{.3575761,.7151522,.1191920}, multiplicationValues3{.1804375,.0721750,.9503041};

    for(size_t index{0}; index < rgbDecimal.size(); index++)
    {
        if (rgbDecimal[index]<=.04045)
            linearizedRGB[index]=rgbDecimal[index]/12.92;
        else
            linearizedRGB[index]=std::pow(((rgbDecimal[index]+.055)/1.055),2.4);
     }

    for(size_t index{0}; index < rgbDecimal.size(); index++)
    {
        CIEXYZ[index]=100*(multiplicationValues1[index]*linearizedRGB[0]+multiplicationValues2[index]*linearizedRGB[1]+multiplicationValues3[index]*linearizedRGB[2]);
    }

        return CIEXYZ;
}

std::array<double,3> convert_CIE_XYZ_to_CIE_Lab(const std::array<double,3> &CIEXYZ)
{
    double sigma{6.0/29.0}, XSubN{95.0489}, YSubN{100.0}, ZSubN{108.8840};
    std::array<double,3> CIELab,t,functionValues,subNValues{XSubN,YSubN,ZSubN};

    for(size_t index{0}; index < CIEXYZ.size(); index++)
    {
        t[index]=CIEXYZ[index]/subNValues[index];

        if (t[index]>std::pow(sigma,3))
            functionValues[index]=std::pow(t[index],(1.0/3.0));
        else
            functionValues[index]=(t[index]/(3*std::pow(sigma,2)))+(4.0/29.0);
     }

    CIELab[0]=116*functionValues[1]-16;
    CIELab[1]=500*(functionValues[0]-functionValues[1]);
    CIELab[2]=200*(functionValues[1]-functionValues[2]);

    return CIELab;
}

double convert_CIE_Lab_to_Delta_E_76(const std::array<double,3> &CIELab1, const std::array<double,3> &CIELab2)
{
    double deltaE76,squaredDifferenceOfLs{std::pow((CIELab1[0]-CIELab2[0]),2)},squaredDifferenceOfas{std::pow((CIELab1[1]-CIELab2[1]),2)},squaredDifferenceOfbs{std::pow((CIELab1[2]-CIELab2[2]),2)};

    deltaE76=std::sqrt(squaredDifferenceOfLs+squaredDifferenceOfas+squaredDifferenceOfbs);

    return deltaE76;
}

double calculate_delta_e76(const std::array<int,3> &RGB1, const std::array<int,3> &RGB2)
{
    double deltaE76{0};
    std::array<double,3> decimalRGB1,decimalRGB2,CIEXYZ1,CIEXYZ2,CIELab1,CIELab2;

    decimalRGB1=convert_rgb_to_decimal_rgb(RGB1);
    decimalRGB2=convert_rgb_to_decimal_rgb(RGB2);

    CIEXYZ1=convert_decimal_rgb_to_CIE_XYZ(decimalRGB1);
    CIEXYZ2=convert_decimal_rgb_to_CIE_XYZ(decimalRGB2);

    CIELab1=convert_CIE_XYZ_to_CIE_Lab(CIEXYZ1);
    CIELab2=convert_CIE_XYZ_to_CIE_Lab(CIEXYZ2);

    deltaE76=convert_CIE_Lab_to_Delta_E_76(CIELab1,CIELab2);

    return deltaE76;
}
