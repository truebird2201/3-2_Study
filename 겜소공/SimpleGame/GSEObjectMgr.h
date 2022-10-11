#pragma once
#include "Global.h"
#include "GSEObject.h"

class GSEObjectMgr
{
	GSEObject* m_objects[MAX_OBJECT_COUNT];

public:
	GSEObjectMgr();
	~GSEObjectMgr();

	bool AddObject(GSEVec3 pos, GSEVec3 size);
	GSEObject GetObject(int index);
	bool DeleteObject(int index);
	int FindEmptySlot();
};

