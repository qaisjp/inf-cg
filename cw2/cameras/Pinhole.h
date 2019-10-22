/*
 * Pinhole.h
 *
 *
 */

#ifndef PINHOLE_H_
#define PINHOLE_H_

#include "core/Camera.h"

namespace rt{

class Pinhole:public Camera{
public:

	//
	// Constructors
	//
	Pinhole();
	Pinhole(int width, int height, int fov);

	//
	//Destructor
	//
	~Pinhole(){};

	//
	// print function (implementing abstract function of base class)
	//
	void printCamera();
};



} //namespace rt




#endif /* PINHOLE_H_ */
