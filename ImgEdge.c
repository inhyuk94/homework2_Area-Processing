#include "DeclaSec.h"

void Sort_FindMaxVal( Double *Arr, Int Size, Int *MaxVal );

void ImgEdge( Buf *DB )
{
	Int   Num;
	Int   ImgOffset = 0;

	system("cls");
	while( 1 )
	{
		printf("1. 유사 연산자 \n2. 차 연산자 \n3. 1차 미분 연산자(Sobel) \n4. 2차 미분 연산자(Dog 7x7) \n5. 전 단계로 돌아가기\n\n");
		printf("수행 할 프로세스를 선택하시오 : ");
		scanf_s(" %d", &Num );

		if( Num == 1 )
			HomoAndDiff( DB, 3, 1 );
		else if( Num == 2 )
			HomoAndDiff( DB, 3, 2 );
		else if( Num == 3 )
			One_Dim_Diff( DB );
		else if( Num == 4 )
			Two_Dim_Diff( DB );
		else if( Num == 5 )
			SubMain( DB );
		else
		{
			printf("\n다시 입력 하세요!!");
			_getch();
		}
		system("cls");	
	}
}

void HomoAndDiff( Buf *DB, Int MaskSize, Int FuncNum )
{
	FILE *fp;
	Int i, j, k, l;
	Int MaxVal;
	Char WFileName[100];
	Double *OriImg;
	Double *OriTmpImg;
	Double MidVal;

	if( FuncNum == 1 )			sprintf_s( WFileName, sizeof(Char)*100, "EdgeHomogeneityImg.raw");
	else if( FuncNum == 2 )		sprintf_s( WFileName, sizeof(Char)*100, "EdgeDifferenceImg.raw");
	else						assert(0);

	fopen_s( &fp, WFileName, "wb" );	

	OriImg = (Double*)calloc(MaskSize * MaskSize, sizeof(double));
	OriTmpImg = (Double*)calloc((MaskSize * MaskSize - 1) / 2, sizeof(double));
	DB->PaddingImg = (UChar*)calloc((ROW + MaskSize - 1)*(COL + MaskSize - 1), sizeof(UChar));
	ImgPadding( DB, MaskSize );

	for( i=0; i<COL; i++ )
	{
		for( j=0; j<ROW; j++ )
		{
			for( k=0; k<MaskSize; k++ )
			{
				for( l=0; l<MaskSize; l++ )
				{
					OriImg[(MaskSize*k) + l] = DB->PaddingImg[(i*(ROW + MaskSize - 1) + j) + (k*(ROW + MaskSize - 1) + l)];
				}
			}

			if( FuncNum == 1 )
			{
				MidVal = OriImg[(MaskSize * MaskSize - 1) / 2];
				for(k=0; k<MaskSize; k++)
				{
					for(l=0; l<MaskSize; l++)
					{  /*** 유사연산자 알고리즘 ***/					
							OriImg[(MaskSize * k) + l] = (double)abs(OriImg[(MaskSize * k) + l] - MidVal);
					}
				}
				Sort_FindMaxVal( OriImg, MaskSize * MaskSize, &MaxVal ); 
			}
			else if( FuncNum == 2 )
			{
				for(k=0; k<(MaskSize * MaskSize - 1) / 2; k++)
				{
					/*** 차연산자 알고리즘 ***/
					OriTmpImg[k] = (double)abs(OriImg[k] - OriImg[ ( MaskSize *(MaskSize)- 1 ) - k ]);

					
				}
				Sort_FindMaxVal( OriTmpImg, (MaskSize * MaskSize - 1) / 2, &MaxVal ); 
			}
			else
			{
				assert(0);
			}

			DB->Output[(i*ROW) + j] = (MaxVal + 60) > 255 ? 255 : (MaxVal + 60) < 0 ? 0 : (MaxVal + 60);
		}
	}

	fwrite(DB->Output, sizeof(unsigned char), ROW*COL, fp);

	free(OriImg);
	free(OriTmpImg);
	free(DB->PaddingImg);

	fclose(fp);
}

void One_Dim_Diff( Buf *DB )
{
	FILE *fp;
	Int   i, j;
	Int   MaskSize;

	fopen_s( &fp, "EdgeOneDimImg.raw", "wb" );

	system("cls");
	printf("Filter mask size : ");
	scanf_s(" %d", &MaskSize);
	printf("\n");

	DB->FilterMask = (Double*)calloc(MaskSize*MaskSize, sizeof(Double));
	DB->PaddingImg = (UChar*)calloc((ROW + MaskSize - 1)*(COL + MaskSize - 1), sizeof(UChar));
	ImgPadding( DB, MaskSize );

	for( i=0; i<2; i++ )
	{
		for( j=0; j<MaskSize * MaskSize; j++ )
		{
			if( i == 0 )	printf("Filter mask coeff[%d] of ROW dir : ", j);
			else			printf("Filter mask coeff[%d] of COL dir : ", j);
			scanf_s(" %lf", &DB->FilterMask[j]);
		}

		Convolution( DB, MaskSize );
		memcpy( i == 0 ? DB->EdgeRowImg : DB->EdgeColImg, DB->Output, ROW*COL );
		printf("\n");
	}

	for( i=0; i<ROW*COL; i++ )
	{
		DB->Output[i] = /*** 1차 미분 연산자 알고리즘 ***/
			(abs(DB->EdgeRowImg[i]) + abs(DB->EdgeColImg[i])) > 255 ? 255 : (abs(DB->EdgeRowImg[i]) + abs(DB->EdgeColImg[i])) < 0 ? 0 : (abs(DB->EdgeRowImg[i]) + abs(DB->EdgeColImg[i]));
		
	}

	fwrite( DB->Output, sizeof(UChar), ROW*COL, fp );

	free( DB->FilterMask );
	free( DB->PaddingImg );

	fclose( fp );
}

void Two_Dim_Diff( Buf *DB )
{
	FILE *fp;
	Int   i;
	Int   MaskSize;

	fopen_s( &fp, "EdgeTwoDimImg.raw", "wb" );

	system("cls");
	printf("Filter mask size : ");
	scanf_s(" %d", &MaskSize);
	printf("\n");

	DB->FilterMask = (Double*)calloc(MaskSize*MaskSize, sizeof(Double));
	DB->PaddingImg = (UChar*)calloc((ROW + MaskSize - 1)*(COL + MaskSize - 1), sizeof(UChar));
	ImgPadding( DB, MaskSize );

	for( i=0; i<MaskSize * MaskSize; i++ )
	{
		printf("Filter mask coeff[%d] : ", i);
		scanf_s(" %lf", &DB->FilterMask[i]);
	}

	Convolution( DB, MaskSize );

	fwrite( DB->Output, sizeof(UChar), ROW*COL, fp );

	free( DB->FilterMask );
	free( DB->PaddingImg );

	fclose( fp );
}

void Sort_FindMaxVal( Double *Arr, Int Size, Int *MaxVal )
{
	int i, j;
	
	for( i=0; i<Size-1; i++ ) 
	{
		for( j=1; j<Size-i; j++ ) 
		{
			if( Arr[j-1] > Arr[j] )
			{
				/*** 최대값 탐색 알고리즘을 위한 화소 정렬 ***/
				double temp = Arr[j-1];
				Arr[j-1] = Arr[j];
				Arr[j] = temp;

			}
		}
	}

	*MaxVal = (int)Arr[Size-1];
}