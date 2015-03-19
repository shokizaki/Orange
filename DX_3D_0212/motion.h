//===============================================
//
//  ���e   �F ���[�V��������
//  File   �F motion.h
//  ����� �F �؍�@��
//
//===============================================

//===============================================
// 
// ��d�C���N���[�h�h�~
// 
//===============================================
#ifndef _MOTION_H_
#define _MOTION_H_

//===============================================
//
//  �C���N���[�h�t�@�C��
//
//===============================================
#include "main.h"

//===============================================
//
// �}�N����`
//
//===============================================
#define PARTS_NUM	( 20 )
#define MOTION_NUM	( 5 )
#define KEY_NUM		( 16 )

//===============================================
//
// �\���̒�`
//
//===============================================
typedef enum
{
	NEUTRAL = 0,
	WALK,
	ACTION,
	JUMP,
	RANDING,
	MAX
}MOTION_STATUS;

typedef enum
{
	DIRECTION_DEPTH = 0,
	DIRECTION_FRONT,
	DIRECTION_RIGHT,
	DIRECTION_LEFT,
	DIRECTION_MAX
}DIRECTION_MODEL;

// ���[�V�������
typedef struct
{	
	int nFrame[ KEY_NUM ];	// �؂�ւ���t���[��
	int nNumKey;			// �L�[���i�ő吔�j
	bool bLoop;				// ���[�v���邩�ǂ���
}MOTIONINFO;

// �L�[���
typedef struct
{
	D3DXVECTOR3 pos[ PARTS_NUM ][ KEY_NUM ];		// �ڕW�ƂȂ鐔�l
	D3DXVECTOR3 rot[ PARTS_NUM ][ KEY_NUM ];		// �ڕW�ƂȂ鐔�l
}KEY;

// ���f���̏��i�P�L�����N�^�[�ɂ����ok�j
// �}�N����`�ōő�̃p�[�c���Ƃ��͕ς������B
// �g���l�ɂ���Ă͂���Ȃ����̂�����������Ă邯�ǂ����͂��C���ŁB
typedef struct
{
	// �L�����N�^�[ / �p�[�c�p
	char modelFileName[ PARTS_NUM ][ 256 ];	// �t�@�C����
	float fMove;							// �ړ���
	float fJumpVal;							// �W�����v��
	float fGravity;							// �d�͗�
	int nNumParts;							// �p�[�c��
	D3DXVECTOR3 posParts[ PARTS_NUM ];		// �ʒu�i��{�ƂȂ�ʒu�i���������́j�j
	D3DXVECTOR3 rotParts[ PARTS_NUM ];		// �p�x�i��{�ƂȂ�ʒu�i���������́j�j
	D3DXVECTOR3 scl[ PARTS_NUM ];			// �傫��
	D3DXVECTOR3 targetRotParts;				// �ڕW�p�x
	int nIndex[ PARTS_NUM ];				// �C���f�b�N�X
	int nParent[ PARTS_NUM ];				// �e�̃C���f�b�N�X
	int nKey;								// ���݂̃L�[���i���[�v�p�ϐ��j
	D3DXVECTOR3 posOld;						// �ʒu�ۑ��p
	D3DXVECTOR3 targetRot[ PARTS_NUM ];		// �����̖ڕW�l�i���[�V�����p�j
	D3DXVECTOR3 targetPos[ PARTS_NUM ];		// �����̖ڕW�l�i���[�V�����p�j
	D3DXVECTOR3 move;						// �ړ���

	D3DXMATRIX mtxWorld[ PARTS_NUM ];			// ���[���h�}�g���b�N�X
	LPD3DXMESH pMeshModel[ PARTS_NUM ];			// ���b�V�����ւ̃|�C���^
	LPD3DXBUFFER pBuffMatModel[ PARTS_NUM ];	// �}�e���A�����ւ̃|�C���^
	DWORD numMatModel[ PARTS_NUM ];				// �}�e���A���̐�

	MOTIONINFO aMotion[ MOTION_NUM ];			// ���[�V�����̏��
	KEY aKeyInfo[ MOTION_NUM ];					// �L�[�̏��

	int nFrameCount;						// �t���[���𐔂���ϐ�
	int nBillboardNum;						// �r���{�[�h�̃C���f�b�N�X
	int nShadowNum;							// �e�̃C���f�b�N�X
	int nMoveCnt;							// �ړ�����t���[���J�E���^�i�G�p�j

	D3DXVECTOR3 rotDevide[ PARTS_NUM ];		// �P�t���[���ɑ�������p�x�ʁi���[�V�����p�j
	D3DXVECTOR3 posDevide[ PARTS_NUM ];		// �P�t���[���ɑ�������ʒu�ʁi���[�V�����p�j

	MOTION_STATUS status;				// ���݂̃��[�V�����̔ԍ�
	MOTION_STATUS statusOld;			// �O��̃��[�V�����̔ԍ�

	DIRECTION_MODEL direction;			// �����Ă����
	DIRECTION_MODEL directionSecond;	// �����Ă����

	bool bMove;
	bool bAutoMove;
	bool bMotion;

}MODEL_INFO;

//===============================================
//
//  �֐��v���g�^�C�v�錾
//
//===============================================

//===============================================
//
// ���f���̃��[�V�����Ɋւ��鏉����
//
//===============================================
void InitModelMotionEx( MODEL_INFO *pModel, char *pFileName );	

//===============================================
//
// ���f���̃��[�V�����Ɋւ���I��
//
//===============================================
void UninitModelMotion( MODEL_INFO *pModel );

//===============================================
//
// ���f���̃��[�V�����Ɋւ���X�V
//
//===============================================
void UpdateModelMotion( MODEL_INFO *pModel );

//===============================================
//
// ���f���̕`��
//
//===============================================
void DrawModelMotion( MODEL_INFO *pModel );



#endif

