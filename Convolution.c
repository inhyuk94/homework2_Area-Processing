#include "DeclaSec.h"

void Convolution( Buf *DB, Int MaskSize )
{
	Int i, j, k, l;
	Int ResultPix = 0;
	Double *OriImg;
	Double RoundPos = 0.5, RoundNeg = -0.5;

	OriImg = (Double*)calloc(MaskSize * MaskSize, sizeof(double));
	memset( DB->Output, 0, ROW*COL );

	for( i=0; i<COL; i++ )
	{
		for( j=0; j<ROW; j++ )
		{
			for( k=0; k<MaskSize; k++ )
			{
				for( l=0; l<MaskSize; l++ )
				{					
					OriImg[(MaskSize*k) + l] = DB->PaddingImg[  (i*(ROW + MaskSize - 1) +j)+(k*(ROW + MaskSize - 1) +l)];
				}
			}

			for( k=0; k<MaskSize * MaskSize; k++ )
			{
				ResultPix += ( (OriImg[k]*DB->FilterMask[k])-(int)(OriImg[k] * DB->FilterMask[k]) ) >= 0.5 ? (int)ceil(OriImg[k] * DB->FilterMask[k]): (int)floor(OriImg[k] * DB->FilterMask[k]);
			}

			DB->Output[ (i * ROW) + j ] = ResultPix > 255 ? 255 : ResultPix < 0 ? 0 : ResultPix;
			ResultPix = 0;
		}
	}

	free(OriImg);
}