#include "stdafx.h"
#include "GSEGame.h"
#include "GSEObject.h"

GSEGame::GSEGame(GSEVec2 size)
{
	// Initialize Renderer
	m_Renderer = new Renderer((int)size.x, (int)size.y);
	m_objectMgr = new GSEObjectMgr();
	if (!m_Renderer->IsInitialized())
	{
		std::cout << "Renderer could not be initialized.. \n";
	}

	//test
	for (int i{}; i < MAX_OBJECT_COUNT; ++i) {
		GSEVec3 pos = { 0, 0, 0 };
		GSEVec3 tsize = { 100, 10, 10 };
		pos.x = ((float)std::rand() / (float)RAND_MAX - 0.5f) * 2.f * 250.f;
		pos.y = ((float)std::rand() / (float)RAND_MAX - 0.5f) * 2.f * 250.f;
		tsize.x = ((float)std::rand() / (float)RAND_MAX) * 5.f;
		m_objectMgr->AddObject(pos, tsize);
	}
}

GSEGame::~GSEGame()
{
	if(m_Renderer)
		delete m_Renderer;
	if(m_objectMgr)
		delete m_objectMgr;
}

void GSEGame::RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.3f, 0.3f, 1.0f);

	// Renderer Test
	for (int i{}; i < MAX_OBJECT_COUNT; ++i) {
		GSEObject temp = m_objectMgr->GetObject(i);
		auto pos = temp.GetPos();
		auto size = temp.GetSize();
		if (size.x > 0.00001f) {
			m_Renderer->DrawSolidRect(pos.x, pos.y, pos.z, size.x, 1, 0, 1, 1);
		}
	}
}
