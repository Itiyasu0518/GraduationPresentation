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

//==============================================================================
// �R���X�g���N�^
//==============================================================================
CUi::CUi()
{

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
	for (unsigned int nCount = 0; nCount < m_Scene.size(); nCount++)
	{
		if (m_Scene[nCount] == NULL) continue;				// �t�@�C����������Ύ���

		// �J������
		m_Scene[nCount]->Release();
		m_Scene[nCount] = NULL;
	}

	// �z��̗v�f���폜
	m_Scene.clear();
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
CUi *CUi::Create(const std::string &add)
{
	CUi *pUi;		// �w�i�̃|�C���^

	pUi = new CUi;		// �w�i�̐���
	pUi->Init();							// �w�i�̏�����
	pUi->LoadScript(add);					// �X�N���v�g�̓ǂݍ���
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
					m_Scene.push_back(scene);

					while (strcmp(cHeadText, "END_UISET") != 0)
					{
						fgets(cReadText, sizeof(cReadText), pFile);
						sscanf(cReadText, "%s", &cHeadText);

						if (strcmp(cHeadText, "TEXTURE_FILENAME") == 0)
						{// �A�h���X���̂Ƃ�
							sscanf(cReadText, "%s %s %s", &cDie, &cDie, &sAdd[0]);						//�A�h���X�̎擾
							Add = sAdd;
							scene->BindTexture(CManager::GetResource(Add));
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