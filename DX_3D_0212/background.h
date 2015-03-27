#ifndef _BACKGROUND_H_
#define _BACKGROUND_H_

#define BACKGROUND_VERTEX ( 4 )

typedef struct{
	LPDIRECT3DTEXTURE9		m_pTexture;			//フィールドのテクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			//ポリゴンの頂点情報へのポインタ
	D3DXVECTOR3				m_pos;				//位置
	D3DXVECTOR3				m_rot;				//向き
	D3DXVECTOR3				m_scl;				//大きさ
	D3DXMATRIX				m_mtxworld;			//ワールドマトリックス
}BACKGROUND;

int InitBackGround(void);
void UninitBackGround(void);
void UpdateBackGround(void);
void DrawBackGround(void);
#endif