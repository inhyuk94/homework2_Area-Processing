#include "DeclaSec.h"

void Sort_FindMidVal( Double *Arr, Int Size, Int *MidVal );

void ImgFiltering( Buf *DB )
{
	FILE *fp;
	Int   i, Num;
	Int   MaskSize;
	Int   ImgOffset = 0;
	Char  MedianFlag;
	Char  OffsetFlag;

	fopen_s( &fp, "FilteredImage.raw", "wb" );

	while( 1 )
	{
		system("cls");
		printf("Median filtering? (y or n) : ");
		scanf_s(" %c", &MedianFlag);

		if( MedianFlag == 'y' || MedianFlag == 'n' )
			break;
		else
			printf("�ٽ� �Է��ؼ���.\n");
	}

	system("cls");
	if( MedianFlag == 'n' )
	{
		printf("** ������, ����, ������ **\n");
		printf("Filter mask size : ");
		scanf_s(" %d", &MaskSize);
		printf("\n");
		printf("���͸� �ݺ� Ƚ�� : ");
		scanf_s(" %d", &Num);
		printf("\n");

		DB->FilterMask = (Double*)calloc(MaskSize*MaskSize, sizeof(Double));
		DB->PaddingImg = (UChar*)calloc((ROW + MaskSize - 1)*(COL + MaskSize - 1), sizeof(UChar));
		
		for( i=0; i<MaskSize * MaskSize; i++ )
		{
			printf("Filter mask coeff[%d] : ", i);
			scanf_s(" %lf", &DB->FilterMask[i]);
		}

		for( i=0; i<Num; i++ )
		{
			ImgPadding( DB, MaskSize );
			Convolution( DB, MaskSize );

			memcpy( DB->Input, DB->Output, ROW*COL );
		}
	}
	else
	{
		printf("** �̵�� ���͸� **\n");
		printf("Filter mask size : ");
		scanf_s(" %d", &MaskSize);
		printf("\n");
		printf("���͸� �ݺ� Ƚ�� : ");
		scanf_s(" %d", &Num);
		printf("\n");

		DB->FilterMask = (Double*)calloc(MaskSize*MaskSize, sizeof(Double));
		DB->PaddingImg = (UChar*)calloc((ROW + MaskSize - 1)*(COL + MaskSize - 1), sizeof(UChar));
		
		for( i=0; i<Num; i++ )
		{
			ImgPadding( DB, MaskSize );
			Median( DB, MaskSize );

			memcpy( DB->Input, DB->Output, ROW*COL );
		}
	}

	while( 1 )
	{
		system("cls");
		printf("�߰����� Offset�� ���Ͻðڽ��ϱ�? (y or n) : ");
		scanf_s(" %c", &OffsetFlag );

		if( OffsetFlag == 'y' || OffsetFlag == 'n' )
			break;
		else
			printf("�ٽ� �Է��ؼ���.\n");
	}

	if( OffsetFlag == 'y' )
	{
		printf("Offset �� : ");
		scanf_s(" %d", &ImgOffset );

		for( i=0; i<ROW*COL; i++ )
			DB->Output[i] = (DB->Output[i] + ImgOffset) > 255 ? 255 : (DB->Output[i] + ImgOffset) < 0 ? 0 : (DB->Output[i] + ImgOffset);
	}

	fwrite( DB->Output, sizeof(UChar), ROW*COL, fp );

	free( DB->FilterMask );
	free( DB->PaddingImg );

	fclose( fp );
}

void Median( Buf *DB, Int MaskSize )
{
	int i, j, k, l;
	Int MidVal;
	Double *OriImg;

	OriImg = (Double*)calloc(MaskSize*MaskSize, sizeof(Double));

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
			
			Sort_FindMidVal( OriImg, MaskSize, &MidVal );
			
		
			DB->Output[j + (i*ROW)] = MidVal > 255 ? 255 : MidVal < 0 ? 0 : MidVal;
		}
	}

	free(OriImg);
}

void Sort_FindMidVal( Double *Arr, Int Size, Int *MidVal )
{
	int i, j;

	for( i=0; i<Size*Size-1; i++ ) 
	{
		for( j=1; j<Size*Size-i; j++ ) 
		{
			
			/*** �߰��� Ž�� �˰����� ���� ȭ�� ���� ***/
			if (Arr[i] > Arr[Size*Size - j])
			{
				
				double temp = Arr[i];
				Arr[i] = Arr[Size*Size - j];
				Arr[Size*Size - j] = temp;
				
			}
		}
	}

	*MidVal = (int)Arr[(Size * Size - 1) / 2];

}