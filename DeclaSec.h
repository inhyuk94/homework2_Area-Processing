#include "ImgInfo.h"

typedef struct _Buffer
{
	UChar  *Input;
	UChar  *Output;
	UChar  *PaddingImg;
	UChar  *EdgeRowImg;
	UChar  *EdgeColImg;
	Double *FilterMask;
}Buffer;

typedef Buffer Buf;

void SubMain( Buf *DB );

void BufCon( Buf *DB, Int OrderFlag );
void ImgPadding( Buf *DB, Int MaskSize );

void ImgFiltering( Buf *DB );
void ImgEdge( Buf *DB );
void Convolution( Buf *DB, Int MaskSize );

void Median( Buf *DB, Int MaskSize );

void HomoAndDiff( Buf *DB, Int MaskSize, Int FuncNum );
void One_Dim_Diff( Buf *DB );
void Two_Dim_Diff( Buf *DB );