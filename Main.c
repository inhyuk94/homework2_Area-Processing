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
		printf("1. ���� ���͸� \n2. ���� ������ ���� \n3. ����\n\n");
		printf("���� �� ���μ����� �����Ͻÿ� : ");
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
			printf("\n�ٽ� �Է� �ϼ���!!");
			_getch();
		}
		system("cls");	
	}
}