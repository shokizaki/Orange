//-----------------------------------------------
//  ���e�F������`�悷�鏈��
//  File�Fnumber.h
//  ����ҁF�؍�@��
//-----------------------------------------------
//------------------------------------
// ��d�C���N���[�h�h�~
//------------------------------------
#ifndef _NUMBER_H_
#define _NUMBER_H_

//-----------------------------------------------
//  �C���N���[�h�t�@�C��
//-----------------------------------------------
#include "main.h"

//------ �\���̒�` ------
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuffer;

	D3DXVECTOR3 pos;	// ��ԑ傫�����̈ʒu

	int nDigit;			// ������`�悷�鐔
	int nNumber;		// �`�悷�鐔��

	float fWidth;		// ��̐����̕�
	float fHeight;		// ��̐����̍���
}NUMBER;

//-----------------------------------------------
//  �֐��v���g�^�C�v�錾
//-----------------------------------------------

// ������
void InitNumber( void );

// �I��
void UninitNumber( NUMBER *pNumber );

// �e�N�X�`���̉������
void UninitNumberTexture( void );

// �`��
void DrawNumber( NUMBER *pNumber, int nNumber );

// �Z�b�g
void SetNumber( NUMBER *pNumber, D3DXVECTOR3 pos, int nDig, float fWidth, float fHeight );


#endif

