#ifndef TRACKDATACONTAINER_H
#define TRACKDATACONTAINER_H

#include "TrackData.h"

#include <exception>

class TrackContainerException : public std::exception {

public:

};

class TrackDataContainer
{
protected:
	int mCellSize;
	int mCellSizeLogarithm;
	int mWidth, mHeight;
public:

	/**
	 * Konstruuje drzewo QuadTree o zadanych wymiarach. Wymiary drzewa musza byc potega
	 * dwojki.
	 * @param width Szerokosc drzewa. Musi byc potega dwojki
	 * @param height Wysokosc drzewa. Musi byc potega dwojki.
	 * @param cellSize Wiekosc najmniejszego obszaru opisywanego przez QuadTree.
	 * @throw TrackContainerException jezeli wymiary drzewa sa niepoprawne.
	 */
	TrackDataContainer( int width, int height, int cellSize );

	virtual ~TrackDataContainer();

	virtual void changeCellSize( int cellSize ) = 0;

	int getCellSize() {
		return mCellSize;
	}

	int getCellSizeLogarithm() {
		return mCellSizeLogarithm;
	}

	virtual const TrackData& pickCell( int x, int y ) = 0;

	virtual void setCell( int x, int y, const TrackData& data ) = 0;

	const TrackData& pickData( int x, int y ) {
		int mx = x >> mCellSizeLogarithm;
		int my = y >> mCellSizeLogarithm;
		return pickCell( mx, my );
	}

	void setData( int x, int y, const TrackData& data ) {
		int mx = x >> mCellSizeLogarithm;
		int my = y >> mCellSizeLogarithm;
		setCell( mx, my, data );
	}

	/**
	 * Pozwala sprawdzic, czy zadana liczba jest potega dwojki.
	 * @return <b>true</b>, jezeli istnieje taki x, ze 2^x=number.
	 */
	static bool IsPowerOf2( int n );

	static int Log2( int number );

};

#endif // TRACKDATACONTAINER_H
