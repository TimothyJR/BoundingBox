#include "BoundingBoxManagerSingleton.h"

BoundingBoxManagerSingleton* BoundingBoxManagerSingleton::instance = nullptr;

void BoundingBoxManagerSingleton::Init()
{
	numBoxes = 0;
}

void BoundingBoxManagerSingleton::Release()
{
	// Release all the boxes
	for(int i = 0; i < numBoxes; i++)
	{
		if( box[i] != nullptr)
		{
			delete box[i];
			box[i] = nullptr;
		}
	}
	box.clear();
	matrix.clear();
	color.clear();
	numBoxes = 0;
}

BoundingBoxManagerSingleton* BoundingBoxManagerSingleton::GetInstance()
{
	if(instance == nullptr)
	{
		instance = new BoundingBoxManagerSingleton();
	}

	return instance;
}

void BoundingBoxManagerSingleton::ReleaseInstance()
{
	if(instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}



BoundingBoxManagerSingleton::BoundingBoxManagerSingleton(void)
{
	Init();
}

BoundingBoxManagerSingleton::BoundingBoxManagerSingleton(BoundingBoxManagerSingleton const& other)
{
}

BoundingBoxManagerSingleton& BoundingBoxManagerSingleton::operator=(BoundingBoxManagerSingleton const& other)
{
	return *this;
}


BoundingBoxManagerSingleton::~BoundingBoxManagerSingleton(void)
{
	Release();
}

int BoundingBoxManagerSingleton::GetBoxTotal()
{
	return numBoxes;
}

void BoundingBoxManagerSingleton::GenerateBoundingBox(String instanceName)
{
	MeshManagerSingleton* pMeshMngr = MeshManagerSingleton::GetInstance();
	// Make sure it is loaded
	if(pMeshMngr->IsInstanceCreated(instanceName))
	{
		if(IdentifyBox(instanceName) == -1)
		{
			// Create a bounding box
			BoundingBoxClass* boundBox = new BoundingBoxClass();

			boundBox->GenerateBoundingBox(instanceName);

			// Add to the list
			box.push_back(boundBox);

			// Create matrix and add to the list
			matrix.push_back(matrix4(IDENTITY));

			// Add color to list
			color.push_back(vector3(1.0f));

			// Increase number of boxes
			numBoxes++;
		}
	}
}

void BoundingBoxManagerSingleton::SetBoundingBoxSpace(matrix4 modelToWorld, String instanceName)
{
	int currentBox = IdentifyBox(instanceName);
	// If a box was found
	if(currentBox != -1)
	{
		matrix[currentBox] = modelToWorld;
	}
}

int BoundingBoxManagerSingleton::IdentifyBox(String instanceName)
{
	// Look through the list
	for(int i = 0; i < numBoxes; i++)
	{
		// Return the sphere if it is the one we are looking for
		if(instanceName == box[i]->GetName())
		{
			return i;
		}
	}
	return -1;
}

void BoundingBoxManagerSingleton::AddBoxToRenderList(String instanceName)
{
	if(instanceName == "ALL")
	{
		for(int i = 0; i < numBoxes; i++)
		{
			box[i]->AddBoxToRenderList(matrix[i], color[i], true);
		}
	}
	else
	{
		int currentBox = IdentifyBox(instanceName);
		if(currentBox != -1)
		{
			box[currentBox]->AddBoxToRenderList(matrix[currentBox], color[currentBox], true);
		}
	}
}

void BoundingBoxManagerSingleton::CalculateCollision()
{
	std::vector<vector3> allCentroids;

	for(int i = 0; i < numBoxes; i++)
	{
		// Make the box white
		color[i] = vector3(1.0f);
		// Place box is global space
		allCentroids.push_back(static_cast<vector3>(matrix[i] * vector4(box[i]->GetCentroid(), 1.0f)));
	}

	// Check for collisions between all boxes
	for(int i = 0; i < numBoxes; i++)
	{
		for(int j = i+1; j < numBoxes; j++)
		{
			vector3 firstBoxMin(allCentroids[i].x - (box[i]->GetXLength() / 2), allCentroids[i].y - (box[i]->GetYLength() / 2),allCentroids[i].z - (box[i]->GetZLength() / 2)) ;
			vector3 firstBoxMax(allCentroids[i].x + (box[i]->GetXLength() / 2), allCentroids[i].y + (box[i]->GetYLength() / 2),allCentroids[i].z + (box[i]->GetZLength() / 2)) ;

			vector3 secondBoxMin(allCentroids[j].x - (box[j]->GetXLength() / 2), allCentroids[j].y - (box[j]->GetYLength() / 2),allCentroids[j].z - (box[j]->GetZLength() / 2)) ;
			vector3 secondBoxMax(allCentroids[j].x + (box[j]->GetXLength() / 2), allCentroids[j].y + (box[j]->GetYLength() / 2),allCentroids[j].z + (box[j]->GetZLength() / 2)) ;

			// Check the x axis for collisions
			if((firstBoxMin.x < secondBoxMax.x && firstBoxMin.x > secondBoxMin.x) 
				|| (firstBoxMax.x < secondBoxMax.x && firstBoxMax.x > secondBoxMin.x)
				|| (firstBoxMin.x < secondBoxMax.x && firstBoxMax.x > secondBoxMin.x))
			{
				// Check the y axis for collisions
				if((firstBoxMin.y < secondBoxMax.y && firstBoxMin.y > secondBoxMin.y) 
					|| (firstBoxMax.y < secondBoxMax.y && firstBoxMax.y > secondBoxMin.y)
					|| (firstBoxMin.y < secondBoxMax.y && firstBoxMax.y > secondBoxMin.y))
				{
					if((firstBoxMin.z < secondBoxMax.z && firstBoxMin.z > secondBoxMin.z) 
						|| (firstBoxMax.z < secondBoxMax.z && firstBoxMax.z > secondBoxMin.z)
						|| (firstBoxMin.z < secondBoxMax.z && firstBoxMax.z > secondBoxMin.z))
					{
						color[i] = color[j] = MERED;
					}
				}
			}
		}
	}
}