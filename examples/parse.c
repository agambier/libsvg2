#include <stdio.h>
#include "svg.h"

int main( int iargc, char **argv )
{
	svgDrawing *ptSvg;
	svgItem *ptItem;
	int iLevel,j;

	if( iargc<2 )
		return -1;

	//	Open the SVG file
	ptSvg = svgParseFile( argv[ 1 ] );
	if( ptSvg==NULL ) {
		printf( "ERROR(%d): %s.\n", svgGetLastError(), svgGetLastErrorDescription() );
		return -1;
	}

	//	Display the SVG item as a tree
	printf( "Nbr of items : %d\n", ptSvg->tItemList.ui32Count );

	iLevel = 0;
	ptItem = ptSvg->tItemList.ptItem;
	while( ptItem!=NULL )
	{
		printf( "|" );
		for( j=0; j<iLevel; j++ )
			printf( "  |" );
		printf( "-%s\n", ptItem->szId );

		//	walk through the tree
		if( ptItem->ptFirstChild!=NULL ) {
			ptItem = ptItem->ptFirstChild;
			iLevel ++;
		}
		else {
			if( ptItem->ptNextItem!=NULL )
				ptItem = ptItem->ptNextItem;
			else if( ptItem->ptParent!=NULL ) {
				ptItem = ptItem->ptParent->ptNextItem;
				if( iLevel > 0 )
					iLevel --;
			}
			else
				ptItem = NULL;
		}
	}


	svgFreeDrawing( ptSvg );

	return 0;
}
