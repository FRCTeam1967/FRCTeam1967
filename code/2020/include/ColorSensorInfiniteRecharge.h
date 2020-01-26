#include <iostream>
//#include "frc/WPILib.h"
#include "rev/ColorSensorV3.h"
#include "rev/ColorMatch.h"
//#include "frc/Color.h"

class ColorSensorInfiniteRecharge
{
    public:
        ColorSensorInfiniteRecharge();
        virtual ~ColorSensorInfiniteRecharge();
        enum InfiniteRechargeColors
        {
            kRed,
            kYellow,
            kBlue,
            kGreen,
            kUnknown,
            kInvalid
        };
        ColorSensorInfiniteRecharge::InfiniteRechargeColors ReadColor();
        ColorSensorInfiniteRecharge::InfiniteRechargeColors ReadRawColor(double &confidence);

    private:
        //bool detected;
        static constexpr auto i2cPort = frc::I2C::Port::kOnboard;
        rev::ColorSensorV3 sensor{i2cPort};
        rev::ColorMatch colorMatcher;
        frc::Color kBlueTarget;
        frc::Color kGreenTarget;
        frc::Color kRedTarget;
        frc::Color kYellowTarget;
};