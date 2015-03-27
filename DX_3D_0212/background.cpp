/*******************************************************************************
*
*	File	:�w�i�\��	[ background.cpp ]
*	Author	:AT12A271 26 keita bannno
*
*******************************************************************************/

/*******************************************************************************
�C���N���[�h�t�@�C��
*******************************************************************************/
#include "main.h"
#include "background.h"
#include "camera.h"
#include "input.h"

/*******************************************************************************
�}�N���錾
*******************************************************************************/
#define TEXTURE_BACKGROUND		"data/TEXTURE/Background.jpg"		// �w�i�e�N�X�`��
#define BG_WIDTH ( 2000.0f )
#define BG_HEIGHT ( 700.0f )

/*******************************************************************************
�O���[�o���ϐ��錾
*******************************************************************************/
BACKGROUND BackGround;

/*******************************************************************************
�t�B�[���h������
*******************************************************************************/
int InitBackGround(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();

	BackGround.m_pos = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
	BackGround.m_scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	BackGround.m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//���_���̐ݒ�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * BACKGROUND_VERTEX,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &BackGround.m_pVtxBuff, NULL)))
	{
		return E_FAIL;
	}
		VERTEX_3D *pVtx;
		BackGround.m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);	//���_�f�[�^�����b�N

		//���_���W������
		pVtx[0].pos = D3DXVECTOR3( -BG_WIDTH, BG_HEIGHT, 0.0f );
		pVtx[1].pos = D3DXVECTOR3( BG_WIDTH, BG_HEIGHT, 0.0f );
		pVtx[2].pos = D3DXVECTOR3( -BG_WIDTH, -BG_HEIGHT, 0.0f );
		pVtx[3].pos = D3DXVECTOR3( BG_WIDTH, -BG_HEIGHT, 0.0f );

		//���_�x�N�g��������
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		//���_�J���[������
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//���_�e�N�X�`��������
		pVtx[0].tex = D3DXVECTOR2(0, 0);
		pVtx[1].tex = D3DXVECTOR2(1, 0);
		pVtx[2].tex = D3DXVECTOR2(0, 1);
		pVtx[3].tex = D3DXVECTOR2(1, 1);

		BackGround.m_pTexture = NULL;

		//�e�N�X�`���̓ǂݍ���
		if (FAILED(D3DXCreateTextureFromFile(pDevice, TEXTURE_BACKGROUND, &BackGround.m_pTexture)))
		{
			return E_FAIL;
		}

		BackGround.m_pVtxBuff->Unlock();		//���_�f�[�^���A�����b�N

	return 0;
}

/*******************************************************************************
�t�B�[���h�I������
*******************************************************************************/
void UninitBackGround(void)
{
	//���_�o�b�t�@�ւ̃|�C���^�̔p��
	if (BackGround.m_pVtxBuff != NULL)
	{
		BackGround.m_pVtxBuff->Release();
		BackGround.m_pVtxBuff = NULL;
	}
}

/*******************************************************************************
�t�B�[���h�X�V
*******************************************************************************/
void UpdateBackGround(void)
{
}

/*******************************************************************************
�t�B�[���h�`��
*******************************************************************************/
void DrawBackGround(void)
{
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	LPDIRECT3DDEVICE9 pDevice	= GetDevice();		//�@�f�o�C�X�|�C���^���擾		

	// �J�����̃Z�b�g
	SetCamera();
	CAMERA *pCamera				= GetCamera();					//�@�J���������擾

			//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&BackGround.m_mtxworld);

		//�t�s������߂�
		D3DXMatrixInverse(&BackGround.m_mtxworld, NULL, &pCamera->mtxView);

		BackGround.m_mtxworld._41 = 0.0f;
		BackGround.m_mtxworld._42 = 0.0f;
		BackGround.m_mtxworld._43 = 0.0f;



		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&BackGround.m_mtxworld);	//������
		D3DXMatrixScaling(&mtxScl, BackGround.m_scl.x, BackGround.m_scl.y, BackGround.m_scl.z);	//�X�P�[���𔽉f
		D3DXMatrixMultiply(&BackGround.m_mtxworld, &BackGround.m_mtxworld, &mtxScl);
	
		D3DXMatrixRotationYawPitchRoll(&mtxRot, BackGround.m_rot.y, BackGround.m_rot.x, BackGround.m_rot.z);	//�����𔽉f
		D3DXMatrixMultiply(&BackGround.m_mtxworld, &BackGround.m_mtxworld, &mtxRot);
		
		D3DXMatrixTranslation(&mtxTranslate, BackGround.m_pos.x, BackGround.m_pos.y, BackGround.m_pos.z);		//�ʒu�𔽉f
		D3DXMatrixMultiply(&BackGround.m_mtxworld, &BackGround.m_mtxworld, &mtxTranslate);
		
		pDevice->SetTransform(D3DTS_WORLD, &BackGround.m_mtxworld);	//�ݒ�


		//�|���S���̕`��ݒ�J�n
		pDevice->SetStreamSource(0, BackGround.m_pVtxBuff, 0, sizeof(VERTEX_3D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, BackGround.m_pTexture);

		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);		//�J�����O�̐ݒ�( ���ʂ��J�����O���� )


		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);		//�J�����O�̐ݒ�( ���ʂ��J�����O���� )
}