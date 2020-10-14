//=============================================================================
//
// �X�N���v�g�������ݏ��� [write.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _WRITE_H_
#define _WRITE_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"

//=============================================================================
// �N���X��`
//=============================================================================
class CWrite
{
public:
	static bool Open(const std::string &add);
	static bool Write(const char* frm, ...);
	static bool TitleWrite(const char* frm, ...);
	static bool IndexWrite(const char* frm, ...);
	static bool End(void);

private:
	static FILE *pFile;					// �t�@�C���̃|�C���^
};
#endif