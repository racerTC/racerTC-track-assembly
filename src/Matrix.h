#ifndef MATRIX_H
#define MATRIX_H

template< typename HeldType >
class Matrix {
	HeldType* mpArray;
	int mWidth;
public:
	Matrix( int width, int height ):
		mpArray( new HeldType[ width * height ] )
	,	mWidth( width )
	{}

	~Matrix() {
		delete [] mpArray;
	}

	const HeldType& get( int x, int y ) {
		return mpArray[ x + (y*mWidth) ];
	}

	void set( int x, int y, const HeldType& value ) {
		mpArray[ x + (y*mWidth) ] = value;
	}
};

#endif // MATRIX_H
