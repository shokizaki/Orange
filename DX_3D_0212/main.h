//===================================================================
//  ���e�F�C���N���[�h�t�@�C����}�N����`�⃉�C�u�����̃����N
//  Fail:main.h
//  ����ҁF�؍�@��
//===================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

//===================================================================
//
//  �C���N���[�h�t�@�C��
//
//===================================================================
#include <windows.h>
#include "d3dx9.h"		// �`�揈���ɕK�v
#define DIRECTINPUT_VERSION (0x0800)
#include "dinput.h"		// ���͏����ɕK�v
#include "xaudio2.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "debugproc.h"
#include "fade.h"
#include "input.h"
#include "mode.h"
#include "collision.h"
#include "sound.h"
#include "inputPad.h"

//===================================================================
//
// ���C�u�����̃����N
//
//===================================================================
#pragma comment (lib, "d3d9.lib")		// �`�揈���ɕK�v
#pragma comment (lib, "d3dx9.lib")		// ��L�̊g�����C�u����
#pragma comment (lib, "dxguid.lib")		// DirectX�R���|�[�l���g�g�p�ɕK�v
#pragma comment (lib, "winmm.lib")		// 
#pragma comment (lib, "dinput8.lib")	// ���͏����ɕK�v

//===================================================================
//
//  �}�N����`
//
//==================================================================
#define SCREEN_WIDTH  (800)			// ��ʂ̕�
#define SCREEN_HEIGHT (600)			// ��ʂ̍���
#define DEGTOR(degree) (degree * (3.1415 / 180))		// �p�x�����W�A���ɕς���v�Z

// ���_�t�H�[�}�b�g
#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)
// ��L�t�H�[�}�b�g�ɍ��킹���\����
typedef struct
{
	D3DXVECTOR3 pos;		// ���_���W
	D3DXVECTOR3 nor;		// �@��
	D3DCOLOR col;			// ���_�J���[
	D3DXVECTOR2 tex;		// �e�N�X�`�����W
}VERTEX_3D;

// ���_�t�H�[�}�b�g�i2D�j
//-------------------------------------
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
 // ��L�t�H�[�}�b�g�ɍ��킹���\����
typedef struct
{
	D3DXVECTOR3 pos;		// ���_���W
	float rhw;				// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	D3DCOLOR col;			// ���_�J���[
	D3DXVECTOR2 tex;		// �e�N�X�`�����W
}VERTEX_2D;


//===================================================================
//
// �v���g�^�C�v�錾
//
//===================================================================
LPDIRECT3DDEVICE9 GetDevice(void);		// �f�o�C�X�̎擾


#endif