#ifndef __BOUNDINGBOXCLASS_H_
#define __BOUNDINGBOXCLASS_H_

#include "MyEngine.h"


class BoundingBoxClass
{
	bool initialized;
	float xLength;
	float yLength;
	float zLength;
	vector3 centroid;
	String name;
public:
	// Constructor
	BoundingBoxClass(void);
	// Copy COnstructor
	BoundingBoxClass(BoundingBoxClass const& other);
	// Copy assignment operator
	BoundingBoxClass& operator=(BoundingBoxClass const& other);
	// Destructor
	~BoundingBoxClass(void);

	// Swaps content with another object's content
	void Swap(BoundingBoxClass& other);

	// Returns if the object is initialized
	bool IsInitialized();

	// Returns dimensions of box
	float GetXLength();
	float GetYLength();
	float GetZLength();

	// Returns the origin of where the cube is drawn
	vector3 GetCentroid();

	// Returns the name of the cube
	String GetName();

	// Creates a Cube in local space
	void GenerateBoundingBox(String instanceName);

	// Adds the cube to the render list
	void AddBoxToRenderList(matrix4 modelToWorld, vector3 color, bool renderCentroid = false);

private:
	// Releases object from memory
	void Release();

	// Initialize the object's fields
	void Init();
};

#endif __BOUNDINGBOXCLASS_H_