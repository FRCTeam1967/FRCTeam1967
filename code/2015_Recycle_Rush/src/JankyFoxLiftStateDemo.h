/*
 * JankyFoxLiftStateDemo.h
 *
 *  Created on: Feb 14, 2015
 *      Author: Natasha
 */

#ifndef SRC_JANKYFOXLIFTSTATEDEMO_H_
#define SRC_JANKYFOXLIFTSTATEDEMO_H_
#include "WPILib.h"
#include "jankyStateMachine.h"


#include "JankyFoxliftState.h"

class JankyFoxLiftStateDemo: public JankyFoxliftState {
public:
	JankyFoxLiftStateDemo();
	virtual ~JankyFoxLiftStateDemo();

	//Member variables

	//Member functions
	void GoUp();
		void GoDown();
		void Reorient();
		void SpinIn();
		void SpinOut();
		void DoneSingulating();
		void DoneReorienting();
};


#endif /* SRC_JANKYFOXLIFTSTATEDEMO_H_ */
