#include "stdafx.h"
#include "GSEObjectMgr.h"

GSEObjectMgr::GSEObjectMgr()
{
	//init object list
	for (int i{}; i < MAX_OBJECT_COUNT; ++i) {
		m_objects[i] = NULL;
	}
}

GSEObjectMgr::~GSEObjectMgr()
{
	for (int i{}; i < MAX_OBJECT_COUNT; ++i) {
		DeleteObject(i);
	}
}

bool GSEObjectMgr::AddObject(GSEVec3 pos, GSEVec3 size)
{
	//find empty slot
	int index{ FindEmptySlot() };

	if (index < 0)
		return false;

	m_objects[index] = new GSEObject(pos, size);
	return true;
}

GSEObject GSEObjectMgr::GetObject(int index)
{
	GSEVec3 pos{ 0, 0, 0 };
	GSEVec3 size{ 0, 0, 0 };
	GSEObject temp = GSEObject(pos, size);
	if (m_objects[index] != NULL)
		memcpy(&temp, m_objects[index], sizeof(GSEObject));
	else {
		//log
	}
	return temp;
}

bool GSEObjectMgr::DeleteObject(int index)
{
	if (m_objects[index]) {
		delete m_objects[index];
		m_objects[index] = NULL;
		return true;
	}
	return false;
}

int GSEObjectMgr::FindEmptySlot()
{
	for (int i{}; i < MAX_OBJECT_COUNT; ++i) {
		if (m_objects[i] == NULL) 
			return i;
	}

	//Fail to find empty slot
	return -1;
}
