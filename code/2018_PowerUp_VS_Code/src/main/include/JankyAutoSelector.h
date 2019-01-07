/*
 * JankyAutoSelector.h
 *
 *  Created on: Jan 18, 2018
 *      Author: AnishaKabir
 */
#include "frc/WPILib.h"
#ifndef JANKYAUTOSELECTOR_H_
#define JANKYAUTOSELECTOR_H_

class JankyAutoSelector {
public:
	JankyAutoSelector();
	virtual ~JankyAutoSelector();
	void Init();
	void PrintValues();
	int GetDelayTime();
	int GetAutoMode(char switchPos, char scalePos);
};

#endif /* JANKYAUTOSELECTOR_H_ */
