//===============================================
//
//  内容   ： モーションに関する処理
//  File   ： motion.cpp
//  制作者 ： 木崎　翔
//
//===============================================

//===============================================
// 
//  インクルードファイル
//
//===============================================
#include "motion.h"
#include "camera.h"

//===============================================
//
// モデルのモーションに関する初期化
//
//===============================================
void InitModelMotionEx( MODEL_INFO *pModel, char *pFileName )
{
	// ファイルのオープン
	FILE *fp = NULL;
	fp = fopen( pFileName, "rt" );
	if (fp == NULL)
	{
		MessageBox(NULL, "モーションテキストのオープンに失敗", "エラー", MB_OK);
		return;
	}
	
	// 使用するワーク
	char strWork[ 100 ];		// 読み込んだ文字列を一時保管する配列
	float x = 0.0f;				// 一時保管用変数
	float y = 0.0f;				// 一時保管用変数
	float z = 0.0f;				// 一時保管用変数
	int key = 0;				// キーのインデックス
	int parts = 0;				// パーツのインデックス
	int nNumModel = 0;			// モデル数
	int nFileName = 0;			// ファイル数
	int nNumMotion = 0;			// モーション番号
	int workInt = 0;			// ワーク
	float workFloat = 0.0f;		// ワーク
	bool bFrameCheck = false;	// 仕方がなく使用する判定用変数

	// とりあえず読み込む
	fscanf( fp, "%s" , strWork );

	// ファイルの読み込み開始
	while ( strcmp( strWork, "END_SCRIPT" ) != 0 )
	{
		// 一行読み込む
		if ( nNumModel == 0 )
		{
			fscanf( fp, "%s = %d" , strWork, &nNumModel );
		}
		else
		{
			fscanf( fp, "%s = %d" , strWork, &workInt );
		}

		// ファイル名の読み込み
		// モデル数が決定していたら
		if ( nNumModel != nFileName )
		{
			while ( nNumModel != nFileName )
			{
				fscanf( fp, "%s = %s" , strWork, &pModel ->modelFileName[ nFileName ][ 0 ] );
				if ( strcmp( strWork, "MODEL_FILENAME" ) == 0 )
				{
					nFileName++;
				}
			}
		}

		// キャラクターの情報を読み込む
		if ( strcmp( strWork, "CHARACTERSET" ) == 0 )
		{
			while ( strcmp( strWork, "END_CHARACTERSET" ) != 0 )
			{
				// 一行読み込む
				fscanf( fp, "%s = %f" , strWork, &workFloat );

				// 移動量
				if ( strcmp( strWork, "MOVE" ) == 0 )
				{
					pModel ->fMove = workFloat;
				}

				// ジャンプ量
				if ( strcmp( strWork, "JUMP" ) == 0 )
				{
					pModel ->fJumpVal = workFloat;
				}

				// パーツ数
				if ( strcmp( strWork, "NUM_PARTS" ) == 0 )
				{
					pModel ->nNumParts = ( int )workFloat;
				}

				// パーツの情報を読み込む
				if ( strcmp( strWork, "PARTSSET" ) == 0 )
				{
					// パーツの設定が終わるまで
					while( strcmp( strWork, "END_PARTSSET" ) != 0 )
					{
						// 一行読み込む
						fscanf( fp, "%s = %f %f %f" , strWork, &x, &y, &z );

						// インデックス（配列番号）
						if ( strcmp( strWork, "INDEX" ) == 0 )
						{
							pModel ->nIndex[ parts ] = ( int )x;
						}

						// 親のインデックス
						if ( strcmp( strWork, "PARENT" ) == 0 )
						{
							pModel ->nParent[ parts ] = ( int )x;
						}

						// 位置
						if ( strcmp( strWork, "POS" ) == 0 )
						{
							pModel ->posParts[ parts ].x = x;
							pModel ->posParts[ parts ].y = y;
							pModel ->posParts[ parts ].z = z;
						}

						// 角度
						if ( strcmp( strWork, "ROT" ) == 0 )
						{
							pModel ->rotParts[ parts ].x = x;
							pModel ->rotParts[ parts ].y = y;
							pModel ->rotParts[ parts ].z = z;
						}
					}

					parts++;	// パーツのインデックスを増やす
				}
			}
		}

		// モーションの読み込み
		if ( strcmp( strWork, "MOTIONSET" ) == 0 )
		{
			key = 0;	// キーのインデックスの初期化

			// モーションの設定が終わるまで
			while ( strcmp( strWork, "END_MOTIONSET" ) != 0 )
			{
				// 一行読み込む
				fscanf( fp, "%s = %f" , strWork, &workFloat );

				// ループ
				if ( strcmp( strWork, "LOOP" ) == 0 )
				{
					if ( ( int )workFloat == 1 )
					{
						pModel ->aMotion[ nNumMotion ].bLoop = true;
					}
					else
					{
						pModel ->aMotion[ nNumMotion ].bLoop = false;
					}
				}

				// キーの総数
				if ( strcmp( strWork, "NUM_KEY" ) == 0 )
				{
					pModel ->aMotion[ nNumMotion ].nNumKey = ( int )workFloat;
				}

				// キーの情報
				if ( strcmp( strWork, "KEYSET" ) == 0 )
				{
					parts = 0;				// パーツ数の初期化
					bFrameCheck = false;	// [ FRAME ]をチェックした判定

					// １モーションの全てのキーの設定が終わるまで
					while ( strcmp( strWork, "END_KEYSET" ) != 0 )
					{
						// 一行読み込む
						fscanf( fp, "%s = %f %f %f" , strWork, &x, &y, &z );	

						// フレーム数
						if ( strcmp( strWork, "FRAME" ) == 0 )
						{
							pModel ->aMotion[ nNumMotion ].nFrame[ key ] = ( int )x;
							bFrameCheck = true;
						}

						// フレームをチェックしていたら（仕方なく行う部分）
						if ( bFrameCheck == true )
						{
							// キーの設定
							if ( strcmp( strWork, "KEY" ) == 0 )
							{
								// キーの設定が終わるまで
								while ( strcmp( strWork, "END_KEY" ) != 0 )
								{
									// 一行読み込む
									fscanf( fp, "%s = %f %f %f" , strWork, &x, &y, &z );

									// 位置
									if ( strcmp( strWork, "POS" ) == 0 )
									{
										pModel ->aKeyInfo[ nNumMotion ].pos[ parts ][ key ].x = x;
										pModel ->aKeyInfo[ nNumMotion ].pos[ parts ][ key ].y = y;
										pModel ->aKeyInfo[ nNumMotion ].pos[ parts ][ key ].z = z;
									}

									// 角度
									if ( strcmp( strWork, "ROT" ) == 0 )
									{
										pModel ->aKeyInfo[ nNumMotion ].rot[ parts ][ key ].x = x;
										pModel ->aKeyInfo[ nNumMotion ].rot[ parts ][ key ].y = y;
										pModel ->aKeyInfo[ nNumMotion ].rot[ parts ][ key ].z = z;
									}
								} //[ END_KEY ]終わり

								parts++;	// キーのインデックスを増やす

							} //[ KEY ]終わり
						} // [ FRAMEチェック ]の終わり					
					} //[ END_KEYSET ]終わり
					
					key++;	// キーのインデックスを増やす

				} //[ KEYSET ]終わり
			} //[ END_MOTIONSET ]終わり

			nNumMotion++;	// モーションのインデックスを増やす
		}//[ MOTIONSET ]終わり
	}
}

//===============================================
//
// モデルのモーションに関する終了
//
//===============================================
void UninitModelMotion( MODEL_INFO *pModel )
{
	for (int nCnt = 0; nCnt < pModel ->nNumParts; nCnt++)
	{
		// メッシュ情報の解放と初期化
		//------------------------------------
		if (pModel ->pMeshModel[ nCnt ] != NULL)
		{ 
			pModel ->pMeshModel[ nCnt ] ->Release();
			pModel ->pMeshModel[ nCnt ] = NULL;
		}
	
		// マテリアル情報の解放と初期化
		//------------------------------------
		if (pModel ->pBuffMatModel[ nCnt ] != NULL)
		{ 
			pModel ->pBuffMatModel[ nCnt ] ->Release();
			pModel ->pBuffMatModel[ nCnt ] = NULL;
		}
	}
}

//===============================================
//
// モデルのモーションに関する更新
//
//===============================================
void UpdateModelMotion( MODEL_INFO *pModel )
{
	// ループするモーションがループするかどうか、調べた判定
	if ( pModel ->aMotion[ pModel ->status ].bLoop == false && pModel ->bMotion == false )
	{
		pModel ->bMotion = true;
		pModel ->nFrameCount = 0;
		pModel ->nKey = 0;
	}

	if ( pModel ->bMotion == false )
	{
		if ( pModel ->status != pModel ->statusOld )
		{
			pModel ->nFrameCount = 0;
			pModel ->nKey = 0;
		}
	}

	for (int nCnt = 0; nCnt < pModel ->nNumParts; nCnt++)
	{
		if ( pModel ->nFrameCount == 0 )
		{
			pModel ->rotDevide[ nCnt ].x = ( pModel ->aKeyInfo[ pModel ->status ].rot[ nCnt ][ pModel ->nKey ].x - pModel ->targetRot[ nCnt ].x) / pModel ->aMotion[ pModel ->status ].nFrame[ pModel ->nKey ];
			pModel ->rotDevide[ nCnt ].y = ( pModel ->aKeyInfo[ pModel ->status ].rot[ nCnt ][ pModel ->nKey ].y - pModel ->targetRot[ nCnt ].y) / pModel ->aMotion[ pModel ->status ].nFrame[ pModel ->nKey ];
			pModel ->rotDevide[ nCnt ].z = ( pModel ->aKeyInfo[ pModel ->status ].rot[ nCnt ][ pModel ->nKey ].z - pModel ->targetRot[ nCnt ].z) / pModel ->aMotion[ pModel ->status ].nFrame[ pModel ->nKey ];
		
			pModel ->posDevide[ nCnt ].x = ( pModel ->aKeyInfo[ pModel ->status ].pos[ nCnt ][ pModel ->nKey ].x - pModel ->targetPos[ nCnt ].x) / pModel ->aMotion[ pModel ->status ].nFrame[ pModel ->nKey ];
			pModel ->posDevide[ nCnt ].y = ( pModel ->aKeyInfo[ pModel ->status ].pos[ nCnt ][ pModel ->nKey ].y - pModel ->targetPos[ nCnt ].y) / pModel ->aMotion[ pModel ->status ].nFrame[ pModel ->nKey ];
			pModel ->posDevide[ nCnt ].z = ( pModel ->aKeyInfo[ pModel ->status ].pos[ nCnt ][ pModel ->nKey ].z - pModel ->targetPos[ nCnt ].z) / pModel ->aMotion[ pModel ->status ].nFrame[ pModel ->nKey ];
		}

		pModel ->targetRot[ nCnt ].x += pModel ->rotDevide[ nCnt ].x;
		pModel ->targetRot[ nCnt ].y += pModel ->rotDevide[ nCnt ].y;
		pModel ->targetRot[ nCnt ].z += pModel ->rotDevide[ nCnt ].z;

		pModel ->targetPos[ nCnt ].x += pModel ->posDevide[ nCnt ].x;
		pModel ->targetPos[ nCnt ].y += pModel ->posDevide[ nCnt ].y;
		pModel ->targetPos[ nCnt ].z += pModel ->posDevide[ nCnt ].z;
	}

	// フレームカウント増加
	pModel ->nFrameCount++;

	// モーションのキー変更処理
	if (pModel ->aMotion[ pModel ->status ].nFrame[ pModel ->nKey ] == pModel ->nFrameCount)
	{ 
		pModel ->nKey++;			// キーを増やす
		pModel ->nFrameCount = 0;	// カウントを元にもどす

		// キーの総数に到達したら
		if (pModel ->aMotion[ pModel ->status ].nNumKey == pModel ->nKey)
		{
			// ループ判定
			if (pModel ->aMotion[ pModel ->status ].bLoop == true)
			{
				pModel ->nKey = 0;		// キーを最初にもどす
			}
			else
			{
				// キー番号を最後のキーで固定
				//pModel ->aMotion[ pModel ->status ].nKey = pModel ->aMotion[ pModel ->status ].nNumKey - 1;
				
				pModel ->bMotion = false;
				
				// ジャンプだったら
				if ( pModel ->status == JUMP )
				{
					pModel ->nKey = 0;			// キーを最初にもどす
					pModel ->nFrameCount = 0;	// カウントを元にもどす
					pModel ->status = NEUTRAL;
					//pModel ->status = RANDING;

					//pModel ->bMotion = true;
				}
				else
				{
					pModel ->nKey = 0;			// キーを最初にもどす
					pModel ->nFrameCount = 0;	// カウントを元にもどす
					pModel ->status = NEUTRAL;
				}
			}
		}
	}

	//デバッグ表示
	//PrintDebugProc("/-------------------------\n");
	//PrintDebugProc("MOTION : NEUTRAL\n");
	//PrintDebugProc("LOOP   : ON\n");
	//PrintDebugProc("KEY    : %d / %d\n", pModel ->nKey, pModel ->aMotion[ pModel ->status ].nNumKey);
	//PrintDebugProc("FRAME  : %d / %d\n", pModel ->nFrameCount, pModel ->aMotion[ pModel ->status ].nFrame[ pModel ->nKey ]);
	//PrintDebugProc("-------------------------/\n\n");
}

//===============================================
//
// モデルの描画
//
//===============================================
void DrawModelMotion( MODEL_INFO *pModel )
{
	// ローカル変数
	//------------------------------------
	LPDIRECT3DDEVICE9 pDevice;						// デバイスオブジェクト
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;		// 計算用
	D3DXMATRIX pMtxWorld;
	D3DXMATERIAL *pMat;								// 
	D3DMATERIAL9 matDef;							// 元のマテリアル情報
	char strDest[256] = "data/TEXTURE/";

	// デバイスの取得
	//------------------------------------
	pDevice = GetDevice();

	SetCamera();

	for (int nCnt = 0; nCnt < pModel ->nNumParts; nCnt++)
	{
		// ワールドマトリックスの設定
		//------------------------------------
		D3DXMatrixIdentity( &pModel ->mtxWorld[ nCnt ] );			// フォーマットの初期化
		D3DXMatrixIdentity( &mtxScl );
		D3DXMatrixIdentity( &mtxRot );
		D3DXMatrixIdentity( &mtxTranslate );

		// スケールを反映
		D3DXMatrixScaling(&mtxScl, pModel ->scl[ nCnt ].x, pModel ->scl[ nCnt ].y, pModel ->scl[ nCnt ].z);
		D3DXMatrixMultiply(&pModel ->mtxWorld[ nCnt ], &pModel ->mtxWorld[ nCnt ], &mtxScl);

		// 向きを反映（モーション用）
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pModel ->targetRot[ nCnt ].y, pModel ->targetRot[ nCnt ].x, pModel ->targetRot[ nCnt ].z);
		D3DXMatrixMultiply(&pModel ->mtxWorld[ nCnt ], &pModel ->mtxWorld[ nCnt ], &mtxRot);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pModel ->rotParts[ nCnt ].y, pModel ->rotParts[ nCnt ].x, pModel ->rotParts[ nCnt ].z);
		D3DXMatrixMultiply(&pModel ->mtxWorld[ nCnt ], &pModel ->mtxWorld[ nCnt ], &mtxRot);

		// 位置を反映（モーション用）
		D3DXMatrixTranslation(&mtxTranslate, pModel ->targetPos[ nCnt ].x, pModel ->targetPos[ nCnt ].y, pModel ->targetPos[ nCnt ].z);
		D3DXMatrixMultiply(&pModel ->mtxWorld[ nCnt ], &pModel ->mtxWorld[ nCnt ], &mtxTranslate);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTranslate, pModel ->posParts[ nCnt ].x, pModel ->posParts[ nCnt ].y, pModel ->posParts[ nCnt ].z);
		D3DXMatrixMultiply(&pModel ->mtxWorld[ nCnt ], &pModel ->mtxWorld[ nCnt ], &mtxTranslate);

		// パーツに親がいたら
		if ( pModel ->nParent[ nCnt ] != -1 )
		{
			// 親子設定
			D3DXMatrixMultiply(&pModel ->mtxWorld[ nCnt ], &pModel ->mtxWorld[ nCnt ], &pModel ->mtxWorld[ pModel ->nParent[ nCnt ] ]);
		}

		// 設定
		pDevice ->SetTransform(D3DTS_WORLD, &pModel ->mtxWorld[ nCnt ]);

		// 現在のマテリアル情報を保存
		//------------------------------------
		pDevice ->GetMaterial( &matDef );

		pMat = (D3DXMATERIAL*)pModel ->pBuffMatModel[ nCnt ] ->GetBufferPointer();

		/*strSrc = strrchr(pMat ->pTextureFilename, '\\') + 1;

		strcat(strDest, strSrc);

		D3DXCreateTextureFromFile(pDevice, strDest, &g_pTextureModel[1]);*/
		
		// マテリアルの数だけループ
		for (int nCntMat = 0; nCntMat < (int)pModel ->numMatModel[ nCnt ]; nCntMat++)
		{
			pDevice ->SetMaterial( &pMat[ nCntMat ].MatD3D );			// マテリアルの設定
			pDevice ->SetTexture( 0, NULL );								// テクスチャのセット
			pModel ->pMeshModel[ nCnt ] ->DrawSubset( nCntMat );	// 描画
		}
			
		// マテリアルを元に戻す
		//------------------------------------
		pDevice ->SetMaterial( &matDef );
	}
}