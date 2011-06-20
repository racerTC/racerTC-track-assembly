#include "TrackArray.h"

#include <cmath>

TrackArray::TrackArray( int width, int height, int cellSize ):
	TrackDataContainer( width, height, cellSize )
{
	// zapewnia, iz zarowno szerokosc, jak i wysokosc, sa potegami dwojki
	if( !IsPowerOf2(width) || !IsPowerOf2(height) )
		throw TrackContainerException();

	mpDataMatrix = allocateArray( width, height, cellSize, mCellSizeLogarithm );
}

TrackArray::~TrackArray() {
	delete mpDataMatrix;
}

TrackArray::DataMatrix* TrackArray::allocateArray( int width, int height, int cellSize, int& cellSizeLogarithm ) {
	//cell size powinno byc wielokrotnoscia dwojki...
	cellSizeLogarithm = Log2( cellSize );
	return new DataMatrix( width >> cellSizeLogarithm, height >> cellSizeLogarithm );
}

void TrackArray::changeCellSize( int cellSize ) {
	double ratio = mCellSize / cellSize;
	DataMatrix* data_matrix;

	if( ratio != 1.0 ) {
		data_matrix = allocateArray( mWidth, mHeight, cellSize, mCellSizeLogarithm );

		if( ratio > 1.0 ) {
			// przypadek zlozony: przepisujemy...

			/* TODO: poprawic segfaulta
			int r = static_cast<int>( ratio );

			for( int j = mHeight; j != 0; j -= mCellSize ) {
				for( int i = mWidth; i != 0; i -= mCellSize ) {
					// przepisujemy komorke (i,j) oryginalnej macierzy do
					// komorek (i*r,j*r) .. (i*r + r-1,j*r + r-1)
					int xo = i*r;
					int yo = j*r;
					for( int y = r; y != 0; --y )
						for( int x = r; x != 0; --x )
							data_matrix->set( xo+x, yo+y, mpDataMatrix->get(i,j) );
				}
			}
			*/

		} else if( ratio < 1.0 ) {
			// przypadek prosty: zmniejszamy rozdzieczosc; nic nie przepisujemy
			//TODO: to nie powinno byc takie proste
		}

		delete mpDataMatrix;
		mpDataMatrix = data_matrix;
		mCellSize = cellSize;

	}
}

const TrackData& TrackArray::pickCell( int x, int y ) {
	return mpDataMatrix->get( x, y );
}

void TrackArray::setCell( int x, int y, const TrackData &data ) {
	return mpDataMatrix->set( x, y, data );
}
