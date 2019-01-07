/*
 * Giraffe.h
 *
 *  Created on: Nov 15, 2018
 *      Author: maddie
 */

#ifndef SRC_GIRAFFE_H_
#define SRC_GIRAFFE_H_

#include <string>
using namespace std;

class Giraffe {
public:
	Giraffe(double height, double lengthOfHorns, double age); //constructor
	virtual ~Giraffe();

	//methods
	void walk(int distance); //method --> data type + name + () + ;
	void makeGiraffeSound(); // method --> data type + name + () + ;
	void sleep(); // method --> data type + name + () + ;

	//getter method
	double getAge(); // method --> data type + name + () + ;

	//setter method
	void setHoursOfSleep(double hours); // method --> data type + name + ( parameter ) + ;

	std::string saySomething();

private:
	//variables
	double giraffeHeight; //type + name + ;
	double hornLength; //type + name + ;
	double giraffeAge; //type + name + ;
};

#endif /* SRC_GIRAFFE_H_ */
