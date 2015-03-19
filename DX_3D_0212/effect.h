//-----------------------------------------------
//  ���e�F�o���b�g����
//  Fail:bullet.h
//  ����ҁF�؍�@��
//-----------------------------------------------
//------------------------------------
// ��d�C���N���[�h�h�~
//------------------------------------
#ifndef _EFFECT_H_
#define _EFFECT_H_

//-----------------------------------------------
//  �C���N���[�h�t�@�C��
//-----------------------------------------------
#include "main.h"

//------ �񋓌^��` ------
typedef enum
{
	EFFECT_000 = 0,
	EFFECT_001,
	EFFECT_002,
	EFFECT_MAX,
}EFFECT_TEX;

//-----------------------------------------------
// �\���̒�`
//-----------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;
	int nLife;
	float fWidth;
	float fHeight;
	float r;
	float g;
	float b;
	float a;
	bool bUse;
	bool bCol;

	EFFECT_TEX effectTex;
}EFFECT;

// �}�N��
#define EFFECTEX_MAX (2000)		// �e�̍ő吔


//-----------------------------------------------
//  �֐��v���g�^�C�v�錾
//-----------------------------------------------
void InitEffectEx(void);		// �o���b�g�Ɋւ��鏉����
void UninitEffectEx(void);	// �o���b�g�Ɋւ���I������
void UpdateEffectEx(void);	// �o���b�g�Ɋւ���X�V����
void DrawEffectEx(void);		// �o���b�g�Ɋւ���`�揈��
void SetEffectEx( D3DXVECTOR3 pos, int nLife, float fWidth, float fHeight, EFFECT_TEX tex );	// �o���b�g�̃Z�b�g
void SetupEffectEx(void);	// �o���b�g�̒��_���Z�b�g
EFFECT *GetEffectEx();

#endif

