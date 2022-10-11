#pragma once
#include "Global.h"
#include "Renderer.h"
#include "GSEObjectMgr.h"

class GSEGame
{

	Renderer* m_Renderer = NULL;
	GSEObjectMgr* m_objectMgr = NULL;
public:
	GSEGame(GSEVec2 size);
	virtual ~GSEGame();
	
	void RenderScene();

};