#include <iostream>
//#include "frc/WPILib.h"
#include "ColorSensorInfiniteRecharge.h"
#include "rev/ColorSensorV3.h"
#include "rev/ColorMatch.h"
#include <frc/smartdashboard/SmartDashboard.h>

//ReadColor() = returns color as an enum of colors
//enum of colors: blue, green, red, yellow, unknown
//ReadRawColor()

//want: to have it read colors + be able to tell number of rotations from it (see colorsensortest)

ColorSensorInfiniteRecharge::ColorSensorInfiniteRecharge()
{
    kBlueTarget = frc::Color(0.143, 0.427, 0.429);
    kGreenTarget = frc::Color(0.197, 0.561, 0.240);
    kRedTarget = frc::Color(0.561, 0.232, 0.114);
    kYellowTarget = frc::Color(0.361, 0.524, 0.113);

    colorMatcher.AddColorMatch(kBlueTarget);
    colorMatcher.AddColorMatch(kGreenTarget);
    colorMatcher.AddColorMatch(kRedTarget);
    colorMatcher.AddColorMatch(kYellowTarget);
}

ColorSensorInfiniteRecharge::~ColorSensorInfiniteRecharge()
{

}

std::string ColorSensorInfiniteRecharge::ReadColor()
{
    double confidence = 0.0;
    frc::Color detectedColor = sensor.GetColor();
    frc::Color matchedColor = colorMatcher.MatchClosestColor(detectedColor, confidence);
    std::string colorString;
    int color=9;

    if(confidence < 0.92)
    {
        color = ColorSensorInfiniteRecharge::kUnknown;
    }
    else if(matchedColor == kRedTarget)
    {
        color = ColorSensorInfiniteRecharge::kRed;
    }
    else if(matchedColor == kYellowTarget)
    {
        color = ColorSensorInfiniteRecharge::kYellow;
    }
    else if(matchedColor == kBlueTarget)
    {
        color = ColorSensorInfiniteRecharge::kBlue;
    }
    else if(matchedColor == kGreenTarget)
    {
        color = ColorSensorInfiniteRecharge::kGreen;
    }
    else
    {
        color = ColorSensorInfiniteRecharge::kInvalid;
    }

    switch (color)
    {
      case ColorSensorInfiniteRecharge::kRed: colorString = "red";
        break;
      case ColorSensorInfiniteRecharge::kYellow: colorString = "yellow";
        break;
      case ColorSensorInfiniteRecharge::kBlue: colorString = "blue";
        break;
      case ColorSensorInfiniteRecharge::kGreen: colorString = "green";
        break;
      case ColorSensorInfiniteRecharge::kUnknown: colorString = "unknown";
        break;
      default: colorString = "invalid";
    }

    return colorString;
}

std::string ColorSensorInfiniteRecharge::ReadRawColor(double &confidence)
{
    frc::Color detectedColor = sensor.GetColor();
    frc::Color matchedColor = colorMatcher.MatchClosestColor(detectedColor, confidence);
    int color=9;
    std::string colorString;

    if(matchedColor == kRedTarget)
    {
        color = ColorSensorInfiniteRecharge::kRed;
    }
    else if(matchedColor == kYellowTarget)
    {
        color = ColorSensorInfiniteRecharge::kYellow;
    }
    else if(matchedColor == kBlueTarget)
    {
        color = ColorSensorInfiniteRecharge::kBlue;
    }
    else if(matchedColor == kGreenTarget)
    {
        color = ColorSensorInfiniteRecharge::kGreen;
    }

    switch (color)
    {
      case ColorSensorInfiniteRecharge::kRed: colorString = "red";
        break;
      case ColorSensorInfiniteRecharge::kYellow: colorString = "yellow";
        break;
      case ColorSensorInfiniteRecharge::kBlue: colorString = "blue";
        break;
      case ColorSensorInfiniteRecharge::kGreen: colorString = "green";
        break;
      default: colorString = "invalid";
    }

    return colorString;
}