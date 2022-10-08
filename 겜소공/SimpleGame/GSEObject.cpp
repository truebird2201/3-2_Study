#include "stdafx.h"
#include "GSEObject.h"

GSEObject::GSEObject(GSEVec3 pos, GSEVec3 size)
{
	m_pos = pos;
	m_size = size;
}

GSEObject::~GSEObject()
{
}
