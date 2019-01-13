/*
 * JankyAutoEntry.h


 *
 *  Created on: Feb 4, 2018
 *      Author: Maxine, Anisha
 */
#include "frc/WPILib.h"


#ifndef INCLUDE_JANKYAUTOENTRY_H_
#define INCLUDE_JANKYAUTOENTRY_H_

class JankyAutoEntry {
public:

	JankyAutoEntry();
	virtual ~JankyAutoEntry();

	/**
	 *
	 * @brief Checks whether or the entry is complete
	 * if called when active is false, it will call Start
	 * if isComplete is true, it would call End then it would return true.
	 */
	bool IsComplete();

	virtual bool JobDone() = 0;
	virtual void RunAction() = 0;
	void Abort();


protected:
	virtual void Start() = 0;
	virtual void End() = 0;

};

#endif /* INCLUDE_JANKYAUTOENTRY_H_ */
