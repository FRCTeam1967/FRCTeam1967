#include <iostream>
#include "AutoEntry.h"

#ifndef AUTO_SHOOT_H
#define AUTO_SHOOT_H

class AutoShoot : public AutoEntry
{
    public:
        AutoShoot();
        virtual ~AutoShoot();

        // auto entry methods
        void Start();
        bool JobDone();
        void RunAction();
        void End();

    private:

};
#endif