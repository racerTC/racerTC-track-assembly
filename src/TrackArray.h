#ifndef TRACKARRAY_H
#define TRACKARRAY_H

#include "TrackData.h"
#include "Matrix.h"
#include "TrackDataContainer.h"

class TrackArray : public TrackDataContainer
{
public:
	typedef Matrix<TrackData> DataMatrix;
protected:
	DataMatrix* mpDataMatrix;

	/**
	 * Alokuje macierz, ktorej wymiary beda odpowiednie dla trasy o podanych parametrach. Wymiary
	 * trasy musza byc potegami dwojki.
	 * @param
	 * @param
	 * @param
	 * @param
	 * @throw
	 */
	DataMatrix* allocateArray( int width, int height, int cellSize, int& cellSizeLogarithm );

public:
	TrackArray( int width, int height, int cellSize );

	~TrackArray();

	void changeCellSize( int cellSize );

	const TrackData& pickCell( int x, int y );

	void setCell( int x, int y, const TrackData& data );

};

#endif // TRACKARRAY_H
