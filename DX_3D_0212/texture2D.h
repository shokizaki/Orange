//===============================================
//
//  ���e   �F ���[�U�[�C���^�[�t�F�[�X����
//  File   �F Texture2D.h
//  ����� �F �؍�@��
//
//===============================================

//===============================================
// 
// ��d�C���N���[�h�h�~
// 
//===============================================
#ifndef _TEXTURE2D_H_
#define _TEXTURE2D_H_

//------ �C���N���[�h�t�@�C�� ------
#include "main.h"

//------ �}�N����` ------
#define TEXTURE2D_MAX ( 10 )

//------ �񋓌^��` ------
typedef enum
{
	TEXTURE2DTYPE_000 = 0,
	TEXTURE2DTYPE_001,
	TEXTURE2DTYPE_HASTY2,
	TEXTURE2DTYPE_BOX,
	TEXTURE2DTYPE_BOXREVERS,
	TEXTURE2DTYPE_MAX
}TEXTURE2DTYPE;

//------ �\���̒�` ------
typedef struct
{
	D3DXVECTOR3 pos;
	D3DCOLOR color;

	float fWidth;
	float fHeight;
	int nFrame;							// �e�N�X�`���̐؂�ւ��X�s�[�h�ݒ�
	int nFrameCount;					// �؂�ւ��t���[���𐔂���
	int nTexDevideX;					// �e�N�X�`���̕�����X
	int nTexDevideY;					// �e�N�X�`���̕�����Y
	int nTexNumX;						// �e�N�X�`���̌��݂̈ʒu
	int nTexNumY;						// �e�N�X�`���̌��݂̈ʒu

	TEXTURE2DTYPE type;

	bool bUse;
	bool bLoop;			// ���[�v����
}TEXTURE2D;

//------ �֐��v���g�^�C�v�錾 ------

//===============================================
//
// Texture2D�Ɋւ��鏉����
//
//===============================================
void InitTexture2D( void );		

//===============================================
//
// Texture2D�Ɋւ���I������
//
//===============================================
void UninitTexture2D(void);	

//===============================================
//
// Texture2D�Ɋւ���X�V����
//
//===============================================
void UpdateTexture2D(void);

//===============================================
//
// Texture2D�Ɋւ���`�揈��
//
//===============================================
void DrawTexture2D(void);

//===============================================
//
// Texture2D�̒��_���Z�b�g
//
//===============================================
void SetupTexture2D(void);

//===============================================
//
// Texture2D�̃Z�b�g
//
//===============================================
int SetTexture2D( D3DXVECTOR3 pos, float fWdith, float fHeight, TEXTURE2DTYPE type );

//===============================================
//
// Texture2D�̎g�pOFF
//
//===============================================
void SetTexture2DUse( int num );


#endif