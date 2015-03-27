//=======================================================================================
//	�N���X�FAT11C272 30623 13
//	����ҁF�؍�@��
//	���e�F�t�F�[�h�C���E�A�E�g
//
//=======================================================================================

#ifndef _FADE_H_
#define _FADE_H_

//-----------------------------------------------
//
//  �C���N���[�h�t�@�C��
//
//-----------------------------------------------
#include "main.h"
#include "mode.h"

//-----------------------------------------------
//
//  �萔��` 
//
//-----------------------------------------------
#define FADE_STRAT_R (0)
#define FADE_STRAT_G (0)
#define FADE_STRAT_B (0)

// �񋓌^��`
typedef enum
{
	FADE_NONE = 0,
	FADE_IN,
	FADE_OUT,
	FADE_MAX
}FADE;

//-----------------------------------------------
//
//   �֐��v���g�^�C�v�錾
//
//-----------------------------------------------

//-----------------------------------------------
//
//�@�t�F�[�h������
//�@
//-----------------------------------------------
void InitFade(void);

//-----------------------------------------------
//
//�@�t�F�[�h�Z�b�g�A�b�v
//�@
//-----------------------------------------------
void SetupFade(void);

//-----------------------------------------------
//
//�@�t�F�[�h�I������
//�@
//-----------------------------------------------
void UninitFade(void);

//-----------------------------------------------
//
//�@�t�F�[�h�C���֐�
//�@�t�F�[�h�C�����w��F�ŃX�^�[�g����
//  r,g,b�@�E�E�E�@�t�F�[�h�F�@
//�@frame�@�E�E�E�@�t�F�[�h���ԁi�t���[�����j
//�@
//-----------------------------------------------
void FadeIn( float r, float g, float b, int frame );

//-----------------------------------------------
//
//�@�t�F�[�h�A�E�g�֐�
//�@�t�F�[�h�A�E�g���w��F�ŃX�^�[�g����
//  r,g,b�@�E�E�E�@�t�F�[�h�F�@
//�@frame�@�E�E�E�@�t�F�[�h���ԁi�t���[�����j
//�@
//-----------------------------------------------
void FadeOut( float r, float g, float b, int frame );

//-----------------------------------------------
//
//�@�t�F�[�h�X�V�����֐�
//�@
//-----------------------------------------------
void UpdateFade(void);

//-----------------------------------------------
//
//�@�t�F�[�h�`�揈���֐�
//�@
//-----------------------------------------------
void DrawFade(void);

//-----------------------------------------------
//
//�@���݂̃t�F�[�h�擾
//�@
//-----------------------------------------------
FADE GetFade( void );

//-----------------------------------------------
//
//�@�t�F�[�h�̃Z�b�g
//�@
//-----------------------------------------------
void SetFade( FADE fadeSet, int frame );

//-----------------------------------------------
//
//�@�t�F�[�h�̈ʒu�Z�b�g
//�@
//-----------------------------------------------
void SetFadePos( D3DXVECTOR3 pos, D3DXVECTOR3 rot );

#endif