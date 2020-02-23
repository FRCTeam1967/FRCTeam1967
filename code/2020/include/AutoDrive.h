#include <iostream>

#ifndef AUTO_DRIVE_H
#define AUTO_DRIVE_H

class AutoDrive
{
    public:
        AutoDrive();
        virtual ~AutoDrive();

        void DriveForward();
        void DriveBackward();
        void TurnLeft(double angle); 
        void TurnRight(double angle); 

    private:

};
#endif