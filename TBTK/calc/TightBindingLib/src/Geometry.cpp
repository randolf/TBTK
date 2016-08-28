/** @Geometry.cpp
 *
 *  @author Kristofer Björnson
 */

#include "../include/Geometry.h"
#include <iostream>

using namespace std;

namespace TBTK{

Geometry::Geometry(int dimensions, int numSpecifiers, Model *parentModel){
	this->dimensions = dimensions;
	this->numSpecifiers = numSpecifiers;
	this->parentModel = parentModel;

	coordinates = new double[dimensions*parentModel->getBasisSize()];
	if(numSpecifiers != 0)
		specifiers = new int[numSpecifiers*parentModel->getBasisSize()];
	else
		specifiers = NULL;
}

Geometry::~Geometry(){
	delete [] coordinates;
	if(specifiers != NULL)
		delete [] specifiers;
}

void Geometry::setCoordinates(const Index &index, const double *coordinates, const int *specifiers){
	int basisIndex = parentModel->getBasisIndex(index);
	for(int n = 0; n < dimensions; n++)
		this->coordinates[dimensions*basisIndex] = coordinates[n];
	if(specifiers != NULL){
		for(int n = 0; n < numSpecifiers; n++)
			this->specifiers[numSpecifiers*basisIndex + n] = specifiers[n];
	}
	else if(numSpecifiers != 0){
		cout << "Error in Geometry::addPoint: Geometry requires " << numSpecifiers << " specfiers.\n";
		exit(1);
	}
}

double Geometry::getDistance(const Index &index1, const Index &index2){
	int basisIndex1 = parentModel->getBasisIndex(index1);
	int basisIndex2 = parentModel->getBasisIndex(index2);

	double distanceSquared = 0.;
	for(int n = 0; n < dimensions; n++){
		double difference = coordinates[dimensions*basisIndex1 + n] - coordinates[dimensions*basisIndex2 + n];
		distanceSquared += difference*difference;
	}

	return sqrt(distanceSquared);
}

};	//End of namespace TBTK
