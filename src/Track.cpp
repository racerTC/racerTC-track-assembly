#include "Track.h"

#include "TrackArray.h"

Track::Track():
	mSaved( false )
,	mpPixmap( 0 )
,	mpTrackData( 0 )
,	mHasName( false )
{}

Track::Track( QPixmap* img, int cellSize ):
	mSaved( false )
,	mpPixmap( 0 )
,	mpTrackData( 0 )
,	mHasName( false )
{
	reloadImage( img, cellSize );
}

Track::~Track() {
	reset();
}

void Track::reloadImage( QPixmap* img, int cellSize ) {
	reset();
	mpPixmap = img;
	QSize size = img->size();
	mWidth = size.width();
	mHeight = size.height();
	mpTrackData = new TrackArray( mWidth, mHeight, cellSize );
}

void Track::reset() {
	if( mpPixmap != 0 ) {
		delete mpPixmap;
		mpPixmap = 0;
	}
	if( mpTrackData != 0 ) {
		delete mpTrackData;
		mpTrackData = 0;
	}
	if( mSaved ) mSaved = false;
}

void Track::setDataResolution( int size ) {
	if( mSaved ) mSaved = false;
	mpTrackData->changeCellSize( size );
}

const QString& Track::load( const QDomDocument& doc ) {

}

void Track::store( const QDomDocument& doc ) {

}
