#include "Settings.h"
#include "frc/WPILib.h"
#include "ctre/Phoenix.h"
#include "jankyTask.h"

#define HATCHINTAKE_H_

using namespace frc;

class HatchIntake//: public JankyTask
{
    public:
        HatchIntake(int pistonTopChannel, int pistonBottomChannel);
        virtual ~HatchIntake();
        //Bottom Piston
        void BottomPistonSwitch();
        void BottomPistonOut();
        void BottomPistonIn();
        bool GetBottomPistonStatus();
        //Top Piston
        void TopPistonSwitch();
        void TopPistonOut();
        void TopPistonIn();
        bool GetTopPistonStatus();
        //Unused
        /*void Go();
        virtual void Run();*/

    private:
        Solenoid*pistonTop;
        Solenoid*pistonBottom;
        bool hatchPistonsIn;
        bool topPistonIn;
        //Unused
        /*bool actuating;
        double fullActuationTime;
        double fullCycleTime;
        int i;
        Timer cycleTimer;*/
};