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
// �~�Ɖ~�̓����蔻��
//-----------------------------------------------
bool ColCircle( COL_RECT *a, COL_RECT *b )
{
	// ��̍��W�ix,y�j�̋����i��Βl�j���v�Z
	//--------------------------------------------------
	float fLengthX = (a ->pos.x - b ->pos.x);	// X���W�̋���
	float fLengthY = (a ->pos.z - b ->pos.z);	// Y���W�̋���

	// �~�Ɖ~�̋������v�Z
	//------------------------------------
	float fLength  = sqrtf( ( fLengthX * fLengthX ) + ( fLengthY * fLengthY ) );

	float aRad = sqrtf( ( a ->harfSize.x * a ->harfSize.x ) + ( a ->harfSize.z * a ->harfSize.z ) );
	float bRad = sqrtf( ( b ->harfSize.x * b ->harfSize.x ) + ( b ->harfSize.z * b ->harfSize.z ) );

	// �����蔻��
	//------------------------------------
	if ( fLength <= aRad + bRad )
	{
		// �~�Ɖ~�̋������A��̉~�̔��a�𑫂������̂�菬���������瓖����
		//------------------------------------------------------------------
		return true;
	}
	else
	{
		// �������Ă��Ȃ�
		//-----------------------------------
		return false;	
	}
}

//-----------------------------------------------
// ��`�Ƌ�`�̓����蔻��
// �����F
// ���S���Wx,y * 2�A���P�A�����P�A���Q�A�����Q
//-----------------------------------------------
bool ColRect( COL_RECT *a, COL_RECT *b )
{
	// �����蔻��
	if (   (a ->pos.x - a->harfSize.x) <= (b ->pos.x + b ->harfSize.x) && (a ->pos.z - a ->harfSize.z) <= (b ->pos.z + b ->harfSize.z)
		&& (a ->pos.x + a->harfSize.x) >= (b ->pos.x - b ->harfSize.x) && (a ->pos.z - a ->harfSize.z) <= (b ->pos.z + b ->harfSize.z)  
		&& (a ->pos.x - a->harfSize.x) <= (b ->pos.x + b ->harfSize.x) && (a ->pos.z + a ->harfSize.z) >= (b ->pos.z - b ->harfSize.z)  
		&& (a ->pos.x + a->harfSize.x) >= (b ->pos.x - b ->harfSize.x) && (a ->pos.z + a ->harfSize.z) >= (b ->pos.z - b ->harfSize.z) )
	{
		return true;	//�������Ă�
	}
	//else
	{
		return false;	// �������ĂȂ�
	}
}

//-----------------------------------------------
// ��`�Ƌ�`�̓����蔻��( X���W )
// �����F
// ���S���Wx,y * 2�A���P�A�����P�A���Q�A�����Q
//-----------------------------------------------
bool ColRectX_Right( COL_RECT *a, COL_RECT *b )
{
	if ( ( a ->pos.x + a ->harfSize.x ) >= ( b ->pos.x - b ->harfSize.x ) )
	{
		return true;
	}

	return false;
}

//-----------------------------------------------
// ��`�Ƌ�`�̓����蔻��
// �����F
// ���S���Wx,y * 2�A���P�A�����P�A���Q�A�����Q
//-----------------------------------------------
bool ColRectX_Left( COL_RECT *a, COL_RECT *b )
{
	if ( ( a ->pos.x - a ->harfSize.x ) <= ( b ->pos.x + b ->harfSize.x ) )
	{
		return true;
	}

	return false;
}

//-----------------------------------------------
// ��`�Ƌ�`�̓����蔻��
// �����F
// ���S���Wx,y * 2�A���P�A�����P�A���Q�A�����Q
//-----------------------------------------------
bool ColRectZ_Depth( COL_RECT *a, COL_RECT *b )
{
	if ( ( a ->pos.z + a ->harfSize.z ) >= ( b ->pos.z - b ->harfSize.z ) )
	{
		return true;
	}

	return false;
}

//-----------------------------------------------
// ��`�Ƌ�`�̓����蔻��
// �����F
// ���S���Wx,y * 2�A���P�A�����P�A���Q�A�����Q
//-----------------------------------------------
bool ColRectZ_Front( COL_RECT *a, COL_RECT *b )
{
	if ( ( a ->pos.z - a ->harfSize.z ) <= ( b ->pos.z + b ->harfSize.z ) )
	{
		return true;
	}

	return false;
}

//-----------------------------------------------
// ��`�Ƌ�`�̓����蔻��( Z���W )
// �����F
// ���S���Wx,y * 2�A���P�A�����P�A���Q�A�����Q
//-----------------------------------------------
bool ColRectZ( COL_RECT *a, COL_RECT *b )
{
	if ( (a ->pos.z - a->harfSize.z) <= (b ->pos.z + b ->harfSize.z) && (a ->pos.z + a->harfSize.z) >= (b ->pos.z - b ->harfSize.z)
	&&	 ( ( a ->pos.z > b ->pos.z ) || ( a ->pos.z < b ->pos.z ) ) )
	{
		return true;
	}

	return false;
}