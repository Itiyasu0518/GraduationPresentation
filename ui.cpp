//=============================================================================
//
// ���[�U�[�C���^�[�t�F�[�X���� [ui.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "ui.h"
#include "manager.h"
#include "renderer.h"
#include "scene2D.h"
#include "write.h"

//==============================================================================
// �R���X�g���N�^
//==============================================================================
CUi::CUi()
{
	m_pos = D3DXVECTOR3_ZERO;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CUi::~CUi()
{

}

//==============================================================================
// ����������
//==============================================================================
HRESULT CUi::Init(void)
{
	return S_OK;
}

//=============================================================================
// �J������
//=============================================================================
void CUi::Uninit(void)
{
	for (unsigned int nCount = 0; nCount < m_Asset.size(); nCount++)
	{
		if (m_Asset[nCount] == NULL) continue;				// �t�@�C����������Ύ���

		// �J������
		m_Asset[nCount]->Release();
		m_Asset[nCount] = NULL;
	}

	// �z��̗v�f���폜
	m_Asset.clear();
}

//=============================================================================
// �X�V����
//=============================================================================
void CUi::Update(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void CUi::Draw(void)
{

}

//==============================================================================
// ��������
//==============================================================================
CUi *CUi::Create(void)
{
	CUi *pUi;		// �w�i�̃|�C���^

	pUi = new CUi;		// �w�i�̐���

	if (pUi == NULL) { return NULL; }
	pUi->Init();							// �w�i�̏�����

	return pUi;
}

//==============================================================================
// �A�Z�b�g�̐�������
//==============================================================================
HRESULT CUi::Load(void)
{
	//�e�N�X�`���̓ǂݍ���
	return S_OK;
}

//==============================================================================
// �X�N���v�g�f�[�^���[�h����
//==============================================================================
bool CUi::LoadScript(const std::string &add)
{
	FILE *pFile;
	char cReadText[128];		//����
	char cHeadText[128];		//��r
	char cDie[128];
	int nCntPointer = 0;		//�|�C���^�[�̐��l

	char sAdd[64];				//���f���̃A�h���X
	std::string Add;

	int nCntMotion = 0;			//�Q�Ƃ���|�C���^�̒l��������
	int nCntKey = 0;

	int nMaxModel = 0;

	//�e�L�X�g�f�[�^���[�h
	pFile = fopen(add.c_str(), "r");

	if (pFile != NULL)
	{
		//�X�N���v�g������܂Ń��[�v
		while (strcmp(cHeadText, "SCRIPT") != 0)
		{
			fgets(cReadText, sizeof(cReadText), pFile);
			sscanf(cReadText, "%s", &cHeadText);
		}

		//�X�N���v�g��������
		if (strcmp(cHeadText, "SCRIPT") == 0)
		{
			//�G���h�X�N���v�g������܂�
			while (strcmp(cHeadText, "END_SCRIPT") != 0)
			{
				fgets(cReadText, sizeof(cReadText), pFile);
				sscanf(cReadText, "%s", &cHeadText);

				//���s
				if (strcmp(cReadText, "\n") == 0)
				{
					continue;
				}
				else if (strcmp(cHeadText, "UISET") == 0)
				{//�L�����N�^�[�̏������̂Ƃ�
					// �쐬
					CScene2D *scene = CScene2D::Create(CScene2D::PRIORITY_UI);

					scene->SetCenter(CScene2D::TEXTUREVTX_CENTER);		// ���S �̐ݒ�
					scene->SetSize(D3DXVECTOR3(100.0f, 70.0f, 0.0f));	// �傫�� �̐ݒ�
					scene->SetRotation(D3DXVECTOR3(-0.79f, -0.79f, 0.0f));
					scene->SetPosition(D3DXVECTOR3(320.0f, 150.0f, 0.0f));	// �ʒu �̐ݒ�
					scene->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));	// �F �̐ݒ�
					scene->SetTransform();		// ���_��� �̐ݒ�

					if (scene == NULL)
					{// �����Ɏ��s�����Ƃ�
						return false;
					}

					// �z��̍Ō���ɓ����
					m_Asset.push_back(scene);

					while (strcmp(cHeadText, "END_UISET") != 0)
					{
						fgets(cReadText, sizeof(cReadText), pFile);
						sscanf(cReadText, "%s", &cHeadText);

						if (strcmp(cHeadText, "TEXTURE_FILENAME") == 0)
						{// �A�h���X���̂Ƃ�
							sscanf(cReadText, "%s %s %s", &cDie, &cDie, &sAdd[0]);						//�A�h���X�̎擾
							Add = sAdd;
							scene->BindTexture(Add);
						}
						else if (strcmp(cHeadText, "POS") == 0)
						{//�ʒu
							D3DXVECTOR3 pos;
							sscanf(cReadText, "%s %s %f %f %f", &cDie, &cDie,
								&pos.x,
								&pos.y,
								&pos.z);

							// �ʒu����ݒ肷��
							scene->SetPosition(pos);
							scene->SetTransform();
						}
						else if (strcmp(cHeadText, "ROT") == 0)
						{//��]
							D3DXVECTOR3 rot;
							sscanf(cReadText, "%s %s %f %f %f", &cDie, &cDie,
								&rot.x,
								&rot.y,
								&rot.z);

							// ��]����ݒ肷��
							scene->SetRotation(rot);
							scene->SetTransform();
						}
						else if (strcmp(cHeadText, "SIZE") == 0)
						{//��]
							D3DXVECTOR3 size;
							sscanf(cReadText, "%s %s %f %f %f", &cDie, &cDie,
								&size.x,
								&size.y,
								&size.z);

							// ��]����ݒ肷��
							scene->SetSize(size);
							scene->SetTransform();
						}
					}
				}
			}
		}

		//�t�@�C����
		fclose(pFile);
	}
	else
	{

	}

	return false;
}

//==============================================================================
// �e�N�X�`������
//==============================================================================
void CUi::CreateTexture(std::string Add)
{
	CScene2D *pScene2D = CScene2D::Create(CScene2D::PRIORITY_UI);

	if (pScene2D != NULL)
	{
		pScene2D->BindTexture(Add);			// �e�N�X�`���̐ݒ�
		if (pScene2D->GetActive())
		{
			pScene2D->SetActive(true);
		}
		m_Asset.push_back(pScene2D);								// �Ō���ɓ����
	}
}

//==============================================================================
// �e�N�X�`���폜
//==============================================================================
void CUi::DeleteTexture(int nIndex)
{
	if (m_Asset[nIndex] == NULL) { return; }			// ���g�����݂��Ă��Ȃ��Ƃ�

	m_Asset[nIndex]->Release();			// �폜�\��
	m_Asset[nIndex] = NULL;				// NULL����
	m_Asset.erase(m_Asset.begin() + nIndex);	// �w��v�f���폜
}

//==============================================================================
// �f�o�b�O��������
//==============================================================================
void CUi::SceneDebug(void)
{
	for (unsigned int nCount = 0; nCount < m_Asset.size(); nCount++)
	{
		char aTag[16];
		memset(&aTag, 0, sizeof(aTag));
		sprintf(aTag, "TEXTURE [%d]", nCount);
		D3DXVECTOR3 pos, rot, size;
		bool bActive;

		if (ImGui::CollapsingHeader(aTag))
		{
			pos = m_Asset[nCount]->GetPosition();
			rot = m_Asset[nCount]->GetRotation();
			size = m_Asset[nCount]->GetSize();
			bActive = m_Asset[nCount]->GetActive();

			ImGui::DragFloat3("pos", (float*)&pos);
			ImGui::DragFloat3("rot", (float*)&rot);
			ImGui::DragFloat3("size", (float*)&size);

			m_Asset[nCount]->SetPosition(pos);
			m_Asset[nCount]->SetRotation(rot);
			m_Asset[nCount]->SetSize(size);

			if (bActive != m_Asset[nCount]->GetActive())
			{
				m_Asset[nCount]->SetActive(bActive);
			}

			m_Asset[nCount]->SetTransform();

			if (ImGui::Button("delete"))
			{
				DeleteTexture(nCount);
			}
		}
	}
}

//==============================================================================
// �X�N���v�g�̏����o������
//==============================================================================
void CUi::SaveScript(std::string Add)
{
	CWrite *pWrite = new CWrite;
	if (pWrite == NULL) return;

	//�ϐ��錾
	char text[64];				// �e�L�X�g
	char cEqual[8] = { "=" };	//�C�R�[���p
	CScene *pSceneNext = NULL;	//����A�b�v�f�[�g�Ώ�
	CScene *pSceneNow = NULL;

	std::string Full = "data/text/";
	Full += Add;
	Full += ".txt";

	//�J����
	if (pWrite->Open(Full))
	{
		strcpy(text, "# UI�f�[�^�X�N���v�g\n");
		strcat(text, "# Author : masayasu wakita\n");

		pWrite->TitleWrite(text);				// �^�C�g���̌`���ŏ�������
		pWrite->Write("SCRIPT\n");			// �X�N���v�g�̏I���錾
		pWrite->Write("\n");

		// ���f���̏�� //
		pWrite->IndexWrite("UI�̏��\n");

		D3DXVECTOR3 pos, rot, size;

		for (unsigned int nCount = 0; nCount < m_Asset.size(); nCount++)
		{
			pos = m_Asset[nCount]->GetPosition() - m_pos;
			rot = m_Asset[nCount]->GetRotation();
			size = m_Asset[nCount]->GetSize();

			pWrite->Write("UISET\n");					// ���_���̏������݊J�n�錾
			pWrite->Write("	TEXTURE_FILENAME = %s\n", m_Asset[nCount]->GetAdd().c_str());
			pWrite->Write("	POS = %.2f %.2f %.2f\n", pos.x, pos.y, pos.z);		// ���S�ʒu�̏�������
			pWrite->Write("	ROT = %.2f %.2f %.2f\n", rot.x, rot.y, rot.z);		// ���S�ʒu�̏�������
			pWrite->Write("	SIZE = %.2f %.2f %.2f\n", size.x, size.y, size.z);		// ���S�ʒu�̏�������
			pWrite->Write("END_UISET\n");				// ���_���̏������݊J�n�錾
			pWrite->Write("\n");							// ���s
		}

		pWrite->Write("END_SCRIPT\n");			// �X�N���v�g�̏I���錾
		pWrite->End();

		MessageBox(NULL, "�����蔻��̏������ݏI�����܂����I", "WARNING", MB_ICONWARNING);	// ���b�Z�[�W�{�b�N�X�̐���
	}
}

//==============================================================================
// UI�̈ʒu
//==============================================================================
void CUi::SetPosition(D3DXVECTOR3 pos)
{
	D3DXVECTOR3 assetPos = D3DXVECTOR3_ZERO;

	for (unsigned int nCount = 0; nCount < m_Asset.size(); nCount++)
	{
		assetPos = m_Asset[nCount]->GetPosition() - m_pos;				// �O�̈ʒu���擾
		m_Asset[nCount]->SetPosition(assetPos + pos);			// �V�����ʒu�ɒu��
		m_Asset[nCount]->SetTransform();
	}

	m_pos = pos;
}