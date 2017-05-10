#include "AppClass.h"
void AppClass::ProcessKeyboard(void)
{
	bool bModifier = false;
	float fSpeed = 0.01f;

	int nMesh = m_pMeshMngr->IdentifyInstance(m_sSelectedObject);

#pragma region ON PRESS/RELEASE DEFINITION
	static bool	bLastF1 = false, bLastF2 = false, bLastF3 = false, bLastF4 = false, bLastF5 = false,
		bLastF6 = false, bLastF7 = false, bLastF8 = false, bLastF9 = false, bLastF10 = false,
		bLastEscape = false, bLastF = false;
#define ON_KEY_PRESS_RELEASE(key, pressed_action, released_action){  \
			bool pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::key);			\
			if(pressed){											\
				if(!bLast##key) pressed_action;}/*Just pressed? */\
			else if(bLast##key) released_action;/*Just released?*/\
			bLast##key = pressed; } //remember the state
#pragma endregion

#pragma region Modifiers
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
		bModifier = true;
#pragma endregion

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
	{
		ResetBoard();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		if (m_v3Rotation.x <= MAX_TURN)
			m_v3Rotation.x += ROT;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		if (m_v3Rotation.x >= -1 * MAX_TURN)
			m_v3Rotation.x -= ROT;
	}
	else
	{
		m_v3Rotation.x *= (1-RECOIL);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		if (m_v3Rotation.z >= -1 * MAX_TURN)
			m_v3Rotation.z -= ROT;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		if (m_v3Rotation.z <= MAX_TURN)
			m_v3Rotation.z += ROT;
	}
	else
	{
		m_v3Rotation.z *= (1-RECOIL);
	}
	//Model States
#pragma region Model States
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad1) || sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
	{
		ball->material = BallMat::rubber;
		ball->name = "Ball1";
		m_pMeshMngr->SetModelMatrix(DEAD, "Ball2");
		m_pMeshMngr->SetModelMatrix(DEAD, "Ball3");
		m_pMeshMngr->SetModelMatrix(ball->GetMatrix(), ball->name);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad2) || sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
	{
		ball->material = BallMat::fuzzy;
		ball->name = "Ball2";
		m_pMeshMngr->SetModelMatrix(DEAD, "Ball1");
		m_pMeshMngr->SetModelMatrix(DEAD, "Ball3");
		m_pMeshMngr->SetModelMatrix(ball->GetMatrix(), ball->name);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad3) || sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
	{
		ball->material = BallMat::lead;
		ball->name = "Ball3";
		m_pMeshMngr->SetModelMatrix(DEAD, "Ball1");
		m_pMeshMngr->SetModelMatrix(DEAD, "Ball2");
		m_pMeshMngr->SetModelMatrix(ball->GetMatrix(), ball->name);
	}
#pragma endregion

#pragma region Model Loading
	//Model Loading
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F5))
	{
		FileReaderClass pFile;
#ifdef FBXSDK_SHARED
		String sFileName = pFile.RetriveFileNameFromExplorer(L"FBX Files (*.fbx)\0*.fbx\0BTO Files (*.bto)\0*.bto\0ATO Files (*.ato)\0*.ato\0OBJ Files (*.obj)\0*.obj\0", L"fbx");
#else
		String sFileName = pFile.RetriveFileNameFromExplorer(L"BTO Files (*.bto)\0*.bto\0ATO Files (*.ato)\0*.ato\0OBJ Files (*.obj)\0*.obj\0", L"bto");
#endif

		if (sFileName != "")
		{
			bool bThreaded = m_pSystem->GetThreaded();
			m_pSystem->SetThreaded(false);

			m_pMeshMngr->m_pModelMngr->DeleteInstance();
			m_sSelectedObject = pFile.GetFileName(sFileName);
			m_pMeshMngr->LoadModel(sFileName, m_sSelectedObject, true);

			bool bTest = m_pMeshMngr->IsInstanceCreated(m_sSelectedObject);
			if (bTest)
			{
			}

			m_pSystem->SetThreaded(bThreaded);
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F6))
	{
		if (m_sSelectedObject != "")
		{
			bool bThreaded = m_pSystem->GetThreaded();
			m_pSystem->SetThreaded(false);
			FileReaderClass pFile;
			String sFileName = pFile.IndicateFileNameOnExplorer(L"BTO Files (*.bto)\0*.bto\0ATO Files (*.ato)\0*.ato\0All Files (*.*)\0*.*\0", L"bto");
			String sExtension = FileReaderClass::GetExtension(sFileName);
			if (sFileName != "")
			{
				if (sExtension == "bto")
					m_pMeshMngr->SaveModel(sFileName, BTO, m_sSelectedObject, true);
				else
					m_pMeshMngr->SaveModel(sFileName, ATO, m_sSelectedObject, true);
			}

			m_pSystem->SetThreaded(bThreaded);
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F9))
	{
		m_pMeshMngr->m_pModelMngr->DeleteInstance(m_sSelectedObject);
		m_sSelectedObject = "";
	}
#pragma endregion
}
void AppClass::ProcessMouse(void)
{

}
