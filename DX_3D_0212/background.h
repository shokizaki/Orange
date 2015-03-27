#ifndef _BACKGROUND_H_
#define _BACKGROUND_H_

#define BACKGROUND_VERTEX ( 4 )

typedef struct{
	LPDIRECT3DTEXTURE9		m_pTexture;			//�t�B�[���h�̃e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			//�|���S���̒��_���ւ̃|�C���^
	D3DXVECTOR3				m_pos;				//�ʒu
	D3DXVECTOR3				m_rot;				//����
	D3DXVECTOR3				m_scl;				//�傫��
	D3DXMATRIX				m_mtxworld;			//���[���h�}�g���b�N�X
}BACKGROUND;

int InitBackGround(void);
void UninitBackGround(void);
void UpdateBackGround(void);
void DrawBackGround(void);
#endif