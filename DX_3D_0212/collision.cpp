//-----------------------------------------------
//  ���e�F�����蔻��
//  Fail:collision.cpp
//  ����ҁF�؍�@��
//-----------------------------------------------

//-----------------------------------------------
//  �C���N���[�h�t�@�C��
//-----------------------------------------------
#include "collision.h"

//-----------------------------------------------
// �I�u�W�F�N�g���m�̓����蔻��
// �����F
//  COL_RECT�^�̕ϐ� * 2
//-----------------------------------------------
bool ColRectXY( COL_RECT *a, COL_RECT *b )
{
	if ( a ->pos.x + a ->harfSize.x >= b ->pos.x - b ->harfSize.x
	&&	 a ->pos.x - a ->harfSize.x <= b ->pos.x + b ->harfSize.x
	&&	 a ->pos.y + a ->harfSize.y >= b ->pos.y - b ->harfSize.y
	&&	 a ->pos.y - a ->harfSize.y <= b ->pos.y + b ->harfSize.y )
	{
		return true;
	}

	return false;
}