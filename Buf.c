#include "DeclaSec.h"

void BufCon( Buf *DB, Int OrderFlag )
{
	if( OrderFlag )
	{
		FILE *fp;
		Int   BitSize = 1 << BITDEPTH;
		fopen_s( &fp, FILENAME, "rb" );	

		DB->Input  = (UChar*)calloc(ROW*COL, sizeof(UChar));
		DB->Output = (UChar*)calloc(ROW*COL, sizeof(UChar));
		DB->EdgeRowImg = (UChar*)calloc(ROW*COL, sizeof(UChar));
		DB->EdgeColImg = (UChar*)calloc(ROW*COL, sizeof(UChar));

		fread( DB->Input, sizeof(UChar), ROW*COL, fp );

		fclose(fp);
	}	
	else
	{
		free( DB->Input );
		free( DB->Output );
		free( DB->EdgeRowImg );
		free( DB->EdgeColImg );
	}
}

void ImgPadding( Buf *DB, Int MaskSize )
{
	Int line, i, j;

	for( line=0; line<(MaskSize / 2) ; line++ )
	{
		/*** 행버퍼 패딩(상하단) ***/
		for( i=0; i<ROW; i++ )
		{
			DB->PaddingImg[ i+1 +line*(ROW + MaskSize - 1)] = DB->Input[i];
			DB->PaddingImg[ i+1 -line*(ROW + MaskSize - 1) + (ROW + MaskSize-1 )*(COL + MaskSize - 1 -1)] = DB->Input[i + (ROW * (COL  - 1))];
		}

		/*** 열버퍼 패딩(좌우측) ***/
		for( i=0; i<COL; i++ ) 
		{
			DB->PaddingImg[ i*(ROW + MaskSize - 1) + line ] = DB->Input[i * ROW];
			DB->PaddingImg[ i*(ROW + MaskSize - 1) + (ROW + MaskSize - 1 -1) -line] = DB->Input[i * ROW + (ROW - 1)];
		}
	}

	for( line=0; line<4; line++ )
	{
		for( i=0; i<(MaskSize / 2); i++ )
		{
			for( j=0; j<(MaskSize / 2); j++ )
			{
				/*** 좌상단 패딩 ***/
				if( line == 0 )
				{
					DB->PaddingImg[ i*(ROW + MaskSize - 1)+j] = DB->Input[0];
				}
				/*** 우상단 패딩 ***/
				else if( line == 1 )
				{
					DB->PaddingImg[ i*(ROW + MaskSize - 1) + (ROW + MaskSize - 1 - 1) -j] = DB->Input[ROW - 1];
				}
				/*** 좌하단 패딩 ***/
				else if( line == 2 )
				{
					DB->PaddingImg[ - i*(ROW + MaskSize - 1) +j + (ROW + MaskSize - 1)*(COL + MaskSize - 1 - 1)] = DB->Input[ROW * (COL - 1)];
				}
				/*** 우하단 패딩 ***/
				else
				{
					DB->PaddingImg[ - i*(ROW + MaskSize - 1) - j + (ROW + MaskSize - 1 -1)  + (ROW + MaskSize - 1)*(COL + MaskSize - 1-1) ] = DB->Input[ROW * COL - 1];
				}
			}
		}
	}

	/*** 원본 버퍼를 패딩 버퍼의 가운데로 불러오기 ***/
	for( i=0; i<COL; i++ )
	{
		for( j=0; j<ROW; j++ )
		{
			DB->PaddingImg[(MaskSize/2)*(ROW + MaskSize - 1) + ( (MaskSize / 2) - 1) + i*(ROW + MaskSize - 1) +j  ] = DB->Input[j + (i * ROW)];
		}
	}
}