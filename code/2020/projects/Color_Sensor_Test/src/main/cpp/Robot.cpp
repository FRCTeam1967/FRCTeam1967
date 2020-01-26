//#include "frc/WPILib.h"
#include <iostream>
#include <frc/TimedRobot.h>
#include <frc/smartdashboard/SmartDashboard.h>

#include "rev/ColorSensorV3.h"
#include "rev/ColorMatch.h"

using namespace std;
using namespace frc;
using namespace rev;

class Robot : public frc::TimedRobot {
  static constexpr auto i2cPort = I2C::Port::kOnboard;
  ColorSensorV3 m_colorSensor{i2cPort};
  ColorMatch m_colorMatcher;

  int rotationCounter = 0, blueCounter = 0, redCounter = 0, yellowCounter = 0, greenCounter = 0;

  static constexpr Color kBlueTarget = Color(0.143, 0.427, 0.429);
  static constexpr Color kGreenTarget = Color(0.197, 0.561, 0.240);
  static constexpr Color kRedTarget = Color(0.561, 0.232, 0.114);
  static constexpr Color kYellowTarget = Color(0.361, 0.524, 0.113);

  public:
  //constructor
  Robot()
  {

  }
  //deconstructor
  ~Robot()
  {

  }
  
  virtual void RobotInit() override
  {
    m_colorMatcher.AddColorMatch(kBlueTarget);
    m_colorMatcher.AddColorMatch(kGreenTarget);
    m_colorMatcher.AddColorMatch(kRedTarget);
    m_colorMatcher.AddColorMatch(kYellowTarget);
  }

  virtual void AutonomousInit() override
  {
  
  }

  virtual void AutonomousPeriodic() override
  {

  }

  virtual void TeleopInit() override
  {

  }

  virtual void TeleopPeriodic() override
  {
    Color detectedColor = m_colorSensor.GetColor();
    
    float red = detectedColor.red;
    float green = detectedColor.green;
    float blue = detectedColor.blue;
    /*
    bool redTrue = false;
    bool greenTrue = false;
    bool blueTrue = false;
    bool yellowTrue = false;
    */
    string colorString;
    double confidence = 0.0;
    Color matchedColor = m_colorMatcher.MatchClosestColor(detectedColor, confidence);
    //double IR = m_colorSensor.GetIR();

    if (confidence < 0.944)
    {
      colorString = "Unknown";
    }
    else if (matchedColor == kBlueTarget)
    {
      colorString = "Blue";
      blueCounter++;
    }
    else if (matchedColor == kRedTarget)
    {
      colorString = "Red";
      redCounter++;
    }
    else if (matchedColor == kGreenTarget)
    {
      colorString = "Green";
      greenCounter++;
    }
    else //if (matchedColor == kYellowTarget)
    {
      colorString = "Yellow";
      yellowCounter++;
    }

    if(blueCounter>0 && redCounter>0 && yellowCounter>0 && greenCounter>0)
    {
      rotationCounter++;
      blueCounter = 0;
      redCounter = 0;
      yellowCounter = 0;
      greenCounter = 0;
    }

    SmartDashboard::PutNumber("Red", red);
    SmartDashboard::PutNumber("Green", green);
    SmartDashboard::PutNumber("Blue", blue);
    //SmartDashboard::PutNumber("IR", IR);
    SmartDashboard::PutNumber("Confidence", confidence);
    SmartDashboard::PutString("Detected color", colorString);
    SmartDashboard::PutNumber("Half Roatations", rotationCounter);

    if(rotationCounter<=8)
    {
      string stop = "now!!!";
      SmartDashboard::PutString("When to stop", stop);
    }

/*
    if (red > 0.1 && red < 0.12 && green > 0.4 && green < 0.42 && blue > 0.47 && blue <0.49)
    {
      blueTrue = true;
    }
    else if (red > 0.15 && red < 0.16 && green > 0.58 && green < 0.6 && blue < 0.25 && blue > 0.27)
    {
      greenTrue = true;
    }
    else if (red > 0.57 && red < 0.59 && green > 0.32 && green < 0.34 && blue > 0.12 && blue <0.13)
    {
      redTrue = true;
    }
    else if (red > 0.3 && red < 0.32 && green > 0.55 && green < 0.57 && blue > 0.12 && blue < 0.14)
    {
      yellowTrue = true;
    }
    */

    /*
    if (blue > red && blue > green)
    {
      blueTrue = true;
    }
    else if (red > blue && red > green)
    {
      redTrue = true;
    }
    else if (green > blue && blue > red)
    {
      greenTrue = true;
    }
    else
    {
      yellowTrue = true;
    }
    */

    //SmartDashboard::PutBoolean("Red: ", redTrue);
    //SmartDashboard::PutBoolean("Green: ", greenTrue);
    //SmartDashboard::PutBoolean("Blue: ", blueTrue);
    //SmartDashboard::PutBoolean("Yellow: ", yellowTrue);

    //uint32_t proximity = m_colorSensor.GetProximity();
    //SmartDashboard::PutNumber("Proximity", proximity);
  }

  virtual void TestPeriodic() override
  {

  }

private:
};
int main() 
{ 
  return frc::StartRobot<Robot>(); 
}