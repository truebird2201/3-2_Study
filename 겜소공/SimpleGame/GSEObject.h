#pragma once
#include "Global.h"

class GSEObject
{
	GSEVec3 m_pos;
	GSEVec3 m_size;

public:
	GSEObject(GSEVec3 pos, GSEVec3 size);
	~GSEObject();

	GSEVec3 GetPos() { return m_pos; }
	GSEVec3 GetSize() { return m_size; }
};

