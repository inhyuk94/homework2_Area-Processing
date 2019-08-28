#include "DeclaSec.h"

void main()
{		
	Buf InOut;	

	BufCon( &InOut, TRUE );
	
	SubMain( &InOut );

	BufCon( &InOut, FALSE );
}

void SubMain( Buf *DB )
{
	Int Num;

	system("cls");	
	while( 1 )
	{
		printf("1. 영상 필터링 \n2. 영상 윤곽선 검출 \n3. 종료\n\n");
		printf("수행 할 프로세스를 선택하시오 : ");
		scanf_s(" %d", &Num );

		if( Num == 1 )
			ImgFiltering( DB );
		else if( Num == 2 )
			ImgEdge( DB );
		else if( Num == 3 )
		{
			printf("\n");
			_exit(0);
		}
		else
		{
			printf("\n다시 입력 하세요!!");
			_getch();
		}
		system("cls");	
	}
}