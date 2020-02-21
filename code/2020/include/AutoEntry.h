/*
 * AutoEntry.h
 *
 *  Created on: Feb 13, 2020
 *      Author: Maddie
 */

#include "frc/WPILib.h"

#ifndef AUTO_ENTRY_H_
#define AUTO_ENTRY_H_

class AutoEntry {
public:

	AutoEntry();
	virtual ~AutoEntry();

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

#endif /* AUTO_ENTRY_H_ */