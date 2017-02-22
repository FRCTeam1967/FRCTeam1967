#include "WPILib.h"
#include "GearsFuel.h"
#define XBOX_CHANNEL 1
#define GEAR_CHANNEL 1
#define INTAKE_CHANNEL 2
#define OUTTAKE_CHANNEL 3
GearsFuel::GearsFuel(int gearChannel, int intakeChannel, int outtakeChannel){
    gearPiston = new Solenoid(9,gearChannel);
    horzPiston = new Solenoid(9,intakeChannel);
    vertPiston = new Solenoid(9,outtakeChannel);
}
GearsFuel::~GearsFuel(void) {
    delete gearPiston;
    delete horzPiston;
    delete vertPiston;
}
void GearsFuel::GearOut(void){
    //move box out
    gearPiston->Set(true);
}
void GearsFuel::GearIn(void){
    //move box in
    gearPiston->Set(false);
}
void GearsFuel::Horz(void) {  //horizontal piston
    //move door up
    horzPiston->Set(true);
}
void GearsFuel::Horz2(void){
    //move door down
    horzPiston->Set(false);
}
void GearsFuel::Vert(void){
    vertPiston->Set(true);
}
void GearsFuel::Vert2(void){
    vertPiston->Set(false);
}
