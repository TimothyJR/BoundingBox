#include "BoundingBoxClass.h"
// BoundingBoxClass
void BoundingBoxClass::Init()
{
	initialized = false;
	xLength = 0.0f;
	yLength = 0.0f;
	zLength = 0.0f;
	centroid = vector3(0.0f);
	name = "NULL";
}

void BoundingBoxClass::Swap(BoundingBoxClass& other)
{
	std::swap(initialized, other.initialized);
	std::swap(xLength, other.xLength);
	std::swap(yLength, other.yLength);
	std::swap(zLength, other.zLength);
	std::swap(centroid, other.centroid);
	std::swap(name, other.name);
}

void BoundingBoxClass::Release()
{
	//No pointers to release
}

BoundingBoxClass::BoundingBoxClass(void)
{
	Init();
}

BoundingBoxClass::BoundingBoxClass(BoundingBoxClass const& other)
{
	initialized = other.initialized;
	xLength = other.xLength;
	yLength = other.yLength;
	zLength = other.zLength;
	centroid = other.centroid;
	name = other.name;
}

BoundingBoxClass& BoundingBoxClass::operator=(BoundingBoxClass const& other)
{
	if(this != &other)
	{
		Release();
		Init();
		BoundingBoxClass temp(other);
		Swap(temp);
	}
	return *this;
}

BoundingBoxClass::~BoundingBoxClass(void)
{
	Release();
}

bool BoundingBoxClass::IsInitialized()
{
	return initialized;
}

float BoundingBoxClass::GetXLength()
{
	return xLength;
}

float BoundingBoxClass::GetYLength()
{
	return yLength;
}

float BoundingBoxClass::GetZLength()
{
	return zLength;
}

vector3 BoundingBoxClass::GetCentroid()
{
	return centroid;
}

String BoundingBoxClass:: GetName()
{
	return name;
}

void BoundingBoxClass::GenerateBoundingBox(String instanceName)
{
	// Do nothing if initialized
	if(initialized)
	{
		return;
	}
	MeshManagerSingleton* pMeshMngr = MeshManagerSingleton::GetInstance();
	if(pMeshMngr->IsInstanceCreated(instanceName))
	{
		name = instanceName;

		std::vector<vector3> meshVertices = pMeshMngr->GetVertices(name);
		unsigned int numVertices = meshVertices.size();
		centroid = meshVertices[0];
		vector3 vectorMax(meshVertices[0]);
		vector3 vectorMin(meshVertices[0]);

		// Get the max and min x, y, and z of the mesh
		for(unsigned int i = 1; i < numVertices; i++)
		{
			if(vectorMin.x > meshVertices[i].x)
			{
				vectorMin.x =  meshVertices[i].x;
			}
			else if(vectorMax.x < meshVertices[i].x)
			{
				vectorMax.x =  meshVertices[i].x;
			}

			if(vectorMin.y > meshVertices[i].y)
			{
				vectorMin.y =  meshVertices[i].y;
			}
			else if(vectorMax.y < meshVertices[i].y)
			{
				vectorMax.y =  meshVertices[i].y;
			}

			if(vectorMin.z > meshVertices[i].z)
			{
				vectorMin.z =  meshVertices[i].z;
			}
			else if(vectorMax.z < meshVertices[i].z)
			{
				vectorMax.z =  meshVertices[i].z;
			}
		}

		centroid = (vectorMin + vectorMax) / 2.0f;

		xLength = vectorMax.x - vectorMin.x;
		yLength = vectorMax.y - vectorMin.y;
		zLength = vectorMax.z - vectorMin.z;

		initialized = true;
	}
}

void BoundingBoxClass::AddBoxToRenderList(matrix4 modelToWorld, vector3 color, bool renderCentroid)
{
	if(!initialized)
	{
		return;
	}
	MeshManagerSingleton* pMeshMngr = MeshManagerSingleton::GetInstance();
	if(renderCentroid)
	{
		pMeshMngr->AddAxisToQueue(modelToWorld * glm::translate(centroid));
	}
	pMeshMngr->AddCubeToQueue(modelToWorld * glm::translate(centroid) * glm::scale(vector3(xLength, yLength, zLength)), color, MERENDER::WIRE);
}