#ifndef __BOUNDINGBOXMANAGERSINGLETON_H_
#define __BOUNDINGBOXMANAGERSINGLETON_H_

#include "BoundingBoxClass.h"
class BoundingBoxManagerSingleton
{
	int numBoxes;
	std::vector<BoundingBoxClass*> box;
	std::vector<matrix4> matrix;
	std::vector<vector3> color;
	static BoundingBoxManagerSingleton* instance;
public:
	// Gets or constructs the singleton pointer
	static BoundingBoxManagerSingleton* GetInstance();

	// Destroys the singleton
	static void ReleaseInstance();

	// Get the number of elements
	int GetBoxTotal();

	// Generates a bounding box
	void GenerateBoundingBox(String instanceName);

	// Sets the matric that represents global position
	void SetBoundingBoxSpace(matrix4 modelToWorld, String instanceName);

	// Returns the index where the box is stored
	int IdentifyBox(String instanceName);

	// Adds the instance to the render list
	void AddBoxToRenderList(String instanceName = "ALL");


	// Calculates collisions between all cubes
	void CalculateCollision();

private:
	// Constructor
	BoundingBoxManagerSingleton(void);

	// Copy Constructor
	BoundingBoxManagerSingleton(BoundingBoxManagerSingleton const& other);

	// Copy Assignment Operator
	BoundingBoxManagerSingleton& operator=(BoundingBoxManagerSingleton const& other);

	// Destructor
	~BoundingBoxManagerSingleton(void);

	// Releases the objects memory
	void Release();

	// Initializes the object
	void Init();
};

#endif __BOUNDINGBOXMANAGERSINGLETON_H_
