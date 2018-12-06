/*
 * Giraffe.cpp
 *
 *  Created on: Nov 15, 2018
 *      Author: maddie
 */

#include <Giraffe.h>
#include <iostream>
#include <string>
using namespace std;

Giraffe::Giraffe(double height, double lengthOfHorns, double age) {
	// TODO Auto-generated constructor stub

	//assign variables to parameters
	giraffeHeight = height;
	hornLength = lengthOfHorns;
	giraffeAge = age;
}

Giraffe::~Giraffe() {
	// TODO Auto-generated destructor stub
}

void Giraffe::makeGiraffeSound() // data type + className + :: + method name + { }
{
	// method 1
	std::cout << "bark" << std::endl;

	//method 2
	printf("bark/n");
}

void Giraffe::sleep()
{
	//method 1
	std::cout << "zzzzzz" << std::endl;

	//method 2
	printf("zzzzzz");
}

void Giraffe::walk(int distance)
{
	int dist;
	dist = distance;

	//method 1
	std::cout << "I am walking " << dist << " miles." << std::endl;

	//method 2
	printf("I am walking %d miles.", dist);
}

double Giraffe::getAge() // data type + className + :: + method name + { }
{
	return(giraffeAge); // return + ( thing you want to return ) + ;
}

void Giraffe::setHoursOfSleep(double hours) // data type + className + :: + method name + { }
{

}

std::string Giraffe::saySomething()
{
	return("Hello. I am a giraffe.");
}
