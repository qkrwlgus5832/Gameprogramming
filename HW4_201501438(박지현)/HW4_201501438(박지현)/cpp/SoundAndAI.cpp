#include "../header/hw3.h"
#include "../header/SoundManager.h"
#include <time.h>

using namespace D2D1;

BOOL DemoApp::addSound(HWND hWnd) {
	//SOUND
	soundManager = new CSoundManager;
	if (!soundManager->init(hWnd))
		return FALSE;

	// ���� ������ �߰��� (id=0���� ������)
	int id;

	if (!soundManager->add(L"./sound/background.wav", &id)) 
		return FALSE;
	if (!soundManager->add(L"./sound/�ֹ�Ҹ�1.wav", &id))
		return FALSE;
	if (!soundManager->add(L"./sound/�ֹ�Ҹ�2.wav", &id))
		return FALSE;
	if (!soundManager->add(L"./sound/�ĸ��Ҹ�.wav", &id))
		return FALSE;
	if (!soundManager->add(L"./sound/���� �Ҹ�.wav", &id))
		return FALSE;
	if (!soundManager->add(L"./sound/���������� �Ҹ�.wav", &id))
		return FALSE;
	if (!soundManager->add(L"./sound/��� �μ��� �Ҹ�.wav", &id)) 
		return FALSE;
	if (!soundManager->add(L"./sound/�� ������ �Ҹ�.wav", &id)) 
		return FALSE;
	if (!soundManager->add(L"./sound/���� �ִ� �Ҹ�.wav", &id)) 
		return FALSE;
	return true;
}


BOOL DemoApp::SetPan(LPDIRECTSOUNDBUFFER lpDSBuffer, LONG IPan) {
	if (lpDSBuffer->SetPan(IPan) != DS_OK)
		return FALSE;

	return TRUE;
}


BOOL DemoApp::SetVolume(LPDIRECTSOUNDBUFFER lpDSBuffer, LONG lVolume) {
	//lpDSBuffer->SetVolume(DSBVOLUME_MIN);
	if (!lpDSBuffer) return FALSE; //jong
	if (FAILED(lpDSBuffer->SetVolume(DSVOLUME_TO_DB(lVolume))))
		return FALSE;

	return TRUE;
}

void DemoApp::drawAICharacter(bool isAiShow) {
	m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

	D2D1::Matrix3x2F Trans = D2D1::Matrix3x2F::Translation(myCharacter->getDestPositon().x, myCharacter->getDestPositon().y);

	LPWSTR str = new TCHAR[100];
	if (Character::isAttacked == true) {
		if ((time(NULL) - Character::lastAttackedTime) <= 5) {
			m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Translation(1100, 100));
			//m_pRenderTarget->DrawTextW(str, wcslen(str), m_pTextFormat, D2D1::RectF(0, 0, 10, 20), m_pTextBrush);
			m_pRenderTarget->FillRectangle(&RectF(0, 0, 200, 200), mouseJailBitmapBrush);
		}
		else {
			Character::isAttacked = false;
			setRandomSkullPosition();
		}
	}

	/*else {*/
	memset(str, 0, sizeof(str));
	for (size_t i = 0; i < character.size(); i++)
	{
		Trans = D2D1::Matrix3x2F::Translation(character[i]->getPosition().x, character[i]->getPosition().y);
		m_pRenderTarget->SetTransform(Trans);
		switch (character[i]->getColor())
		{
		case 0://Character.Black:
			m_pSceneBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
			swprintf(str, L"Stop");
			break;
		case 1://Character.Red:
			m_pSceneBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
			swprintf(str, L"attacking");
			break;
		case 2://Character.Blue:
			m_pSceneBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Blue));
			swprintf(str, L"following");
			break;
		case 3: //Character.Green
			m_pSceneBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Green));
			swprintf(str, L"moving");
			break;
		default:
			m_pSceneBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
			break;
		}
		//Trans = D2D1::Matrix3x2F::Translation(character[i]->getPosition().x - 30, character[i]->getPosition().y - 15);
		//m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Translation(0,0));

		//m_pRenderTarget->DrawRectangle(D2D1::Rect(-5, -5, 5, 5), m_pSceneBrush);
		if (Character::isAttacked == false)
			m_pRenderTarget->DrawTextW(str, wcslen(str), m_pTextFormat, D2D1::RectF(-20, -25, 50, 20), m_pRedBrush);
		m_pRenderTarget->FillRectangle(&RectF(0, 0, 30, 20), skullBitmapBrush);
		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Translation(character[i]->getPosition().x + 8, character[i]->getPosition().y + 12));


		// �� �ٱ� �ֺ� �� �׸��� 
		if (isAiShow == true) {
			m_pSceneBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Blue));
			m_pRenderTarget->DrawGeometry(pViewRange[i], m_pSceneBrush, 0.3f);
			m_pSceneBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
			m_pRenderTarget->DrawGeometry(pAttackRange[i], m_pSceneBrush, 0.3f);
			m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
			swprintf(str, L"�ΰ����� ������ attack���ϸ� ������ Ǯ���� �������� 5�ʰ� ���콺�� ������� ���մϴ�.");
			m_pRenderTarget->DrawTextW(str, wcslen(str), m_pTextFormat, D2D1::RectF(300, 100, 1000, 200), m_pRedBrush);
		}

	}

	for (std::vector<Character*>::iterator iter = character.begin(); iter != character.end(); iter++)
	{
		Character* ch = *iter;
		ch->update(myCharacter, 0.01);
	}
	myCharacter->update(myCharacter, 0.01);
}