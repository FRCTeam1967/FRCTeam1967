#include <iostream>
// Do NOT include this global header file. Include only what you need. --Mentor-Bob: #include "frc/WPILib.h"
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
    #define CLASSLIGHTON_SENSORON

    #ifdef CLASSLIGHTON_SENSORON
        kBlueTarget = frc::Color(0.122, 0.413, 0.466);
        kGreenTarget = frc::Color(0.166, 0.573, 0.260);
        kRedTarget = frc::Color(0.500, 0.347, 0.140);
        kYellowTarget = frc::Color(0.315, 0.557, 0.125);
    #endif

    #ifdef OUTSIDE
        kBlueTarget = frc::Color(0.143, 0.427, 0.429);
        kGreenTarget = frc::Color(0.197, 0.561, 0.240);
        kRedTarget = frc::Color(0.561, 0.232, 0.114);
        kYellowTarget = frc::Color(0.361, 0.524, 0.113);
    #endif

    colorMatcher.AddColorMatch(kBlueTarget);
    colorMatcher.AddColorMatch(kGreenTarget);
    colorMatcher.AddColorMatch(kRedTarget);
    colorMatcher.AddColorMatch(kYellowTarget);
}

ColorSensorInfiniteRecharge::~ColorSensorInfiniteRecharge()
{

}

ColorSensorInfiniteRecharge::InfiniteRechargeColors ColorSensorInfiniteRecharge::ReadColor()
{
    double confidence = 0.0;
    frc::Color detectedColor = sensor.GetColor();
    frc::Color matchedColor = colorMatcher.MatchClosestColor(detectedColor, confidence);
    //std::string colorString;
    //int color=9;

    if(confidence < 0.92)
    {
        return ColorSensorInfiniteRecharge::kUnknown;
    }
    else if(matchedColor == kRedTarget)
    {
        return ColorSensorInfiniteRecharge::kRed;
    }
    else if(matchedColor == kYellowTarget)
    {
        return ColorSensorInfiniteRecharge::kYellow;
    }
    else if(matchedColor == kBlueTarget)
    {
        return ColorSensorInfiniteRecharge::kBlue;
    }
    else if(matchedColor == kGreenTarget)
    {
        return ColorSensorInfiniteRecharge::kGreen;
    }
    else
    {
        return ColorSensorInfiniteRecharge::kInvalid;
    }
}

ColorSensorInfiniteRecharge::InfiniteRechargeColors ColorSensorInfiniteRecharge::ReadRawColor(double &confidence)
{
    frc::Color detectedColor = sensor.GetColor();
    frc::Color matchedColor = colorMatcher.MatchClosestColor(detectedColor, confidence);
    //int color=9;
    std::string colorString;

    if(matchedColor == kRedTarget)
    {
        return ColorSensorInfiniteRecharge::kRed;
    }
    else if(matchedColor == kYellowTarget)
    {
        return ColorSensorInfiniteRecharge::kYellow;
    }
    else if(matchedColor == kBlueTarget)
    {
        return ColorSensorInfiniteRecharge::kBlue;
    }
    else if(matchedColor == kGreenTarget)
    {
        return ColorSensorInfiniteRecharge::kGreen;
    }
    else
    {
        return ColorSensorInfiniteRecharge::kUnknown;
    }
    
}

std::string ColorSensorInfiniteRecharge::GetColorString(ColorSensorInfiniteRecharge::InfiniteRechargeColors colorNum)
{
    std::string colorString;

    switch (colorNum)
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