
#ifndef GEARSFUEL_H_
#include "WPILib.h"
#include "CANTalon.h"
#define GEARSFUEL_H_
#include "Solenoid.h"
class GearsFuel{
public:
    GearsFuel( int gearChannel, int intakeChannel, int outtakeChannel);
        virtual ~GearsFuel(void);
        void Vert(void);
        void Vert2(void);
        void GearOut(void);
        void GearIn(void);
        void Horz(void);
        void Horz2(void);
        Solenoid * gearPiston;
        Solenoid * horzPiston;
        Solenoid * vertPiston;
};
#endif /* GEARSFUEL_H_ */
