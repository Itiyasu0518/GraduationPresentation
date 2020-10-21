//=============================================================================
//
// �I�u�W�F�N�g���� [object.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "object.h"
#include "manager.h"
#include "renderer.h"
#include "meshField.h"
#include "colliderBox.h"
#include "colliderSphere.h"
#include "camera.h"
#include "house.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define OBJECT_SCRIPT "data/text/object.txt"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CObject::CObject(CScene::PRIORITY obj = CScene::PRIORITY_MODEL) : CSceneX(obj)
{
	SetObjType(CScene::PRIORITY_MODEL);

	m_ColliderBox = NULL;
	m_ColliderSphere = NULL;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CObject::~CObject()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CObject::Init(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();	// �f�o�C�X�̎擾

	// ����������
	CSceneX::Init();

	D3DXVECTOR3 pos = D3DXVECTOR3(500.0f, 0.0f, 0.0f);

	SetPosition(pos);			// �ʒu�̐ݒ�
	return S_OK;
}

//=============================================================================
// �J������
//=============================================================================
void CObject::Uninit(void)
{
	if (m_ColliderBox != NULL)
	{// �{�b�N�X�R���C�_�[�����݂��Ă����Ƃ�
		m_ColliderBox->Release();			// �폜�\��
		m_ColliderBox = NULL;				// NULL����
	}

	// �J������
	CSceneX::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CObject::Update(void)
{
	D3DXVECTOR3 pos = GetPosition();				// �ʒu�̎擾

	if (m_ColliderBox != NULL)
	{// �{�b�N�X�R���C�_�[�����݂��Ă����Ƃ�
		m_ColliderBox->SetPosition(pos);			// �ʒu�̐ݒ�
	}
	if (m_ColliderSphere != NULL)
	{// �X�t�B�A�R���C�_�[�����݂��Ă����Ƃ�
		m_ColliderSphere->SetPosition(pos);			// �ʒu�̐ݒ�
	}

	SetPosition(pos);		// �ʒu�̐ݒ�
#ifdef _DEBUG
	Debug();				// �f�o�b�O����
#endif
}

//=============================================================================
// �`�揈��
//=============================================================================
void CObject::Draw(void)
{
	CSceneX::Draw();
}

//=============================================================================
// �N���G�C�g�֐�
//=============================================================================
CObject *CObject::Create(void)
{
	CObject *pObject;
	pObject = new CObject(CScene::PRIORITY_MODEL);

	if (pObject != NULL)
	{
		pObject->Init();
	}
	return pObject;
}

//=============================================================================
// ���[�h����
//=============================================================================
HRESULT CObject::Load(void)
{
	return S_OK;
}

//=============================================================================
// �e�N�X�`���̉���֐�
//=============================================================================
void CObject::Unload(void)
{
}

//=============================================================================
// �I�u�W�F�N�g�A�Z�b�g�̃��[�h����
//=============================================================================
void CObject::LoadScript(void)
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
	pFile = fopen(OBJECT_SCRIPT, "r");

	if (pFile != NULL)
	{
		//�|�C���^�[�̃��Z�b�g
		nCntPointer = 0;

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
				if (strcmp(cReadText, "\n") != 0)
				{
					if (strcmp(cHeadText, "MODEL_FILENAME") == 0)
					{//�p�[�c���f���̃A�h���X���̂Ƃ�
						sscanf(cReadText, "%s %s %s", &cDie, &cDie, &sAdd[0]);						//�A�h���X�̎擾
						Add = sAdd;
						CManager::LoadModel(Add);
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
}

//=============================================================================
// ���f���̓Ǎ�
//=============================================================================
void CObject::LoadModel(char *add)
{
	FILE *pFile = NULL;											// �t�@�C��
	char cReadText[128] = {};									// ����
	char cHeadText[128] = {};									// ��r
	CObject *pObject = NULL;

	pFile = fopen(add, "r");									// �t�@�C�����J���܂��͍��

	if (pFile != NULL)											// �t�@�C�����ǂݍ��߂��ꍇ
	{
		fgets(cReadText, sizeof(cReadText), pFile);				// �s���΂�
		fgets(cReadText, sizeof(cReadText), pFile);				// �s���΂�
		fgets(cReadText, sizeof(cReadText), pFile);				// �s���΂�

		while (strcmp(cHeadText, "End") != 0)
		{
			std::string Data = cReadText;
			std::vector<std::string> splitData = CManager::split(Data, ',');

			pObject = CObject::Create();
			pObject->BindModel(splitData[LOADTYPE_ADD]);
			pObject->SetPosition(D3DXVECTOR3((float)atof(splitData[LOADTYPE_POS_X].c_str()), (float)atof(splitData[LOADTYPE_POS_Y].c_str()), (float)atof(splitData[LOADTYPE_POS_Z].c_str())));			// �ʒu�̐ݒ�
			pObject->SetRotation(D3DXVECTOR3((float)atof(splitData[LOADTYPE_ROT_X].c_str()), (float)atof(splitData[LOADTYPE_ROT_Y].c_str()), (float)atof(splitData[LOADTYPE_ROT_Z].c_str())));			// ��]�l��ݒ�

			fgets(cReadText, sizeof(cReadText), pFile);									// �s���΂�
			sscanf(cReadText, "%s", &cHeadText);
		}

		fclose(pFile);					// �t�@�C�������
	}
	else
	{
		MessageBox(NULL, "���f�����̃A�N�Z�X���s�I", "WARNING", MB_ICONWARNING);	// ���b�Z�[�W�{�b�N�X�̐���
	}
}

//=============================================================================
// ���f���ǂݍ���(�e�X�g)
//=============================================================================
void CObject::LoadModelTest(char *add)
{
	FILE *pFile = NULL;																	// �t�@�C��
	char cReadText[128];															// ����
	char cHeadText[128];															// ��r
	char cDie[128];

	pFile = fopen(add, "r");				// �t�@�C�����J���܂��͍��

	if (pFile != NULL)						//�t�@�C�����ǂݍ��߂��ꍇ
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
				if (strcmp(cReadText, "\n") != 0)
				{
					if (strcmp(cHeadText, "MODELSET") == 0)
					{//�L�����N�^�[�̏������̂Ƃ�
						CObject *pObj;
						pObj = CObject::Create();										// ���̍쐬
						OutputDebugString("���f���쐬");

						if (pObj != NULL)
						{
							//�G���h�L�����N�^�[�Z�b�g������܂Ń��[�v
							while (strcmp(cHeadText, "END_MODELSET") != 0)
							{
								fgets(cReadText, sizeof(cReadText), pFile);
								sscanf(cReadText, "%s", &cHeadText);

								if (strcmp(cHeadText, "MODEL_FILENAME") == 0)
								{//�p�[�c�����̂Ƃ�
									std::string add;
									char aModelAdd[64];
									memset(&add, 0, sizeof(add));

									sscanf(cReadText, "%s %s %s", &cDie, &cDie,
										&aModelAdd);

									// char��string��
									add = aModelAdd;

									// �ʒu�̐ݒ�
									pObj->BindModel(add);
								}
								else if (strcmp(cHeadText, "POS") == 0)
								{//�p�[�c�����̂Ƃ�
									D3DXVECTOR3 pos;
									sscanf(cReadText, "%s %s %f %f %f", &cDie, &cDie,
										&pos.x,
										&pos.y,
										&pos.z);

									// �ʒu�̐ݒ�
									pObj->SetPosition(pos);
								}
								else if (strcmp(cHeadText, "ROT") == 0)
								{//�p�[�c�����̂Ƃ�
									D3DXVECTOR3 rot;
									sscanf(cReadText, "%s %s %f %f %f", &cDie, &cDie,
										&rot.x,
										&rot.y,
										&rot.z);

									// �ʒu�̐ݒ�
									pObj->SetRotation(rot);
								}
								else if (strcmp(cHeadText, "SIZE") == 0)
								{//�p�[�c�����̂Ƃ�
									D3DXVECTOR3 size;
									sscanf(cReadText, "%s %s %f %f %f", &cDie, &cDie,
										&size.x,
										&size.y,
										&size.z);

									// �ʒu�̐ݒ�
									pObj->SetSize(size);
								}
							}
						}
					}
				}
			}
		}
		fclose(pFile);				// �t�@�C�������
	}
	else
	{
		MessageBox(NULL, "�n�ʏ��̃A�N�Z�X���s�I", "WARNING", MB_ICONWARNING);	// ���b�Z�[�W�{�b�N�X�̐���
	}
}

//=============================================================================
// ���f�����̓Ǎ�
//=============================================================================
void CObject::BindModel(std::string add)
{
	m_Add = add;

	// ���f���̎擾
	CManager::GetModelResource(add, m_pBuffMat, m_nNumMat, m_pMesh);

	// ���f���̏����Z�b�g
	CSceneX::BindModel(m_pMesh, m_nNumMat, m_pBuffMat);
}

//=============================================================================
// �C���X�y�N�^�[��\��
//=============================================================================
void CObject::ShowInspector(void)
{
	CSceneX::ShowInspector();
}

#ifdef _DEBUG
//=============================================================================
// �f�o�b�O����
//=============================================================================
void CObject::Debug(void)
{

}
#endif