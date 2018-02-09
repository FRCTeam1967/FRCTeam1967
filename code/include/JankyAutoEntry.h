/*
 * JankyAutoEntry.h


 *
 *  Created on: Feb 4, 2018
 *      Author: Maxine, Anisha
 */
#include "WPILib.h"


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
	virtual bool IsComplete() = 0;

	/**
	 * @brief - starts the action
	 */
	 virtual void RunAction() = 0;

	 bool GetActive();

private:



	/**
	 * @brief Starts the entry and sets active to true. It will call RunAction.
	 */
	void Start();

	/**
	 * @brief End the entry and sets active to false.
	 */
	void End();

};

#endif /* INCLUDE_JANKYAUTOENTRY_H_ */
