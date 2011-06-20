#include "TrackDataContainer.h"

#include <cmath>

TrackDataContainer::TrackDataContainer( int width, int height, int cellSize ):
	mCellSize( cellSize ), mCellSizeLogarithm( Log2(cellSize) )
,	mWidth( width ), mHeight( height )
{}

TrackDataContainer::~TrackDataContainer() {}

bool TrackDataContainer::IsPowerOf2( int number ) {
	// obliczamy, do jakiej potegi nalezy podniesc 2, aby otrzymac n
	double x = log( static_cast<double>(number) ) / log( 2.0 );
	// jezeli czesc ulamkowa x == 0 -> n jest potega dwojki
	if( x - floor(x) == 0.0 )
		return true;
	else
		return false;
}

int TrackDataContainer::Log2( int number ) {
	double x = log( static_cast<double>( number ) ) / log( 2.0 );
	if( x - floor(x) != 0.0 )
		throw TrackContainerException();
	return static_cast<int>(x);
}
