//=============================================================================
//
// ���̋��������߂鏈�� [distanceNext.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "distanceNext.h"
#include "manager.h"
#include "renderer.h"
#include "ui.h"
#include "number.h"

//==============================================================================
// �R���X�g���N�^
//==============================================================================
CDistanceNext::CDistanceNext()
{
	m_pUi = NULL;
	m_pos = D3DXVECTOR3_ZERO;
	m_distance = D3DXVECTOR3_ZERO;
	m_IntervalNum = D3DXVECTOR3_ZERO;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CDistanceNext::~CDistanceNext()
{

}

//==============================================================================
// ����������
//==============================================================================
HRESULT CDistanceNext::Init(void)
{
	// ������
	m_pUi = CUi::Create();

	if (m_pUi != NULL)
	{
		m_pUi->LoadScript("data/text/ui/TargetDistance.txt");
	}

	for (int nCount = 0; nCount < DISTANCE_MAXNUM; nCount++)
	{
		m_apNumber[nCount] = CNumber::Create();

		if (m_apNumber[nCount] != NULL)
		{
			m_apNumber[nCount]->SetSize(D3DXVECTOR3(50, 90, 0));						// �傫���ݒ�
			m_apNumber[nCount]->BindTexture("data/tex/number_rank.png");
		}
	}

	// �ʒu�֌W�̐ݒ�
	SetTransform();
	SetNumber(0);
	return S_OK;
}

//=============================================================================
// �J������
//=============================================================================
void CDistanceNext::Uninit(void)
{

}

//=============================================================================
// �X�V����
//=============================================================================
void CDistanceNext::Update(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void CDistanceNext::Draw(void)
{

}

//==============================================================================
// ��������
//==============================================================================
CDistanceNext *CDistanceNext::Create(void)
{
	CDistanceNext *pDistanceNext;		// �w�i�̃|�C���^

	pDistanceNext = new CDistanceNext();		// �w�i�̐���
	if (pDistanceNext == NULL) { return NULL; }	// NULL��������Ԃ�

	pDistanceNext->Init();				// �w�i�̏�����
	return pDistanceNext;
}

//==============================================================================
// �A�Z�b�g�̐�������
//==============================================================================
HRESULT CDistanceNext::Load(void)
{
	return S_OK;
}

//=============================================================================
// �ʒu�̐ݒ�
//=============================================================================
void CDistanceNext::SetPosition(D3DXVECTOR3 &pos)
{
	m_pos = pos;
	SetTransform();
}

//=============================================================================
// �����̐ݒ�
//=============================================================================
void CDistanceNext::SetDistance(D3DXVECTOR3 &distance)
{
	m_distance = distance;
	SetTransform();
}

//=============================================================================
// �����ǂ����̋���
//=============================================================================
void CDistanceNext::SetIntervalNum(D3DXVECTOR3 & interval)
{
	m_IntervalNum = interval;
	SetTransform();
}

//=============================================================================
// �ʒu�֌W�̍X�V
//=============================================================================
void CDistanceNext::SetTransform(void)
{
	if (m_pUi != NULL)
	{
		m_pUi->SetPosition(m_pos);
	}

	for (int nCount = 0; nCount < DISTANCE_MAXNUM; nCount++)
	{
		if (m_apNumber[nCount] != NULL)
		{
			m_apNumber[nCount]->SetPosition((m_distance + m_pos) + m_IntervalNum * (float)nCount);
			m_apNumber[nCount]->SetTransform();
		}
	}
}

//=============================================================================
// �^�C���̕ύX
//=============================================================================
void CDistanceNext::SetNumber(int nValue)
{
	int nNumber;
	int nLength = CManager::LengthCalculation(nValue);

	// �ő包���܂ŃJ�E���g
	for (int nCount = 0; nCount < DISTANCE_MAXNUM; nCount++)
	{
		// �\�����鐔���v�Z
		nNumber = nValue % (int)powf(10.0f, (float)DISTANCE_MAXNUM - nCount) / (int)powf(10.0f, DISTANCE_MAXNUM - 1.0f - nCount);

		// �����ݒ�
		m_apNumber[nCount]->SetNumber(nNumber);

		if (DISTANCE_MAXNUM - nCount <= nLength)
		{// �ő包����菬�������������Ƃ�
			if (!m_apNumber[nCount]->GetActive())
			{
				m_apNumber[nCount]->SetActive(true);
			}
		}
		else
		{// �ő包�����傫�����������Ƃ�
			if (m_apNumber[nCount]->GetActive())
			{
				m_apNumber[nCount]->SetActive(false);
			}
		}
	}
}