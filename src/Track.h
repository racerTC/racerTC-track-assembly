#ifndef TRACK_H
#define TRACK_H

#include <QPixmap>
#include <QtXml/QDomDocument>

#include "TrackDataContainer.h"

/**
 * Przechowuje trase.
 * @author Piotr Balut
 */
class Track
{
	int mWidth, mHeight;
	bool mSaved;
	QPixmap* mpPixmap;
	TrackDataContainer* mpTrackData;

	bool mHasName;
	QString mName;

	/** Konstruktor kopiujacy - prywatny, bo niezaimplementowany. */
	Track( const Track& ) {}
protected:
	void reset();
public:
	Track();

	/**
	 * Tworzy trase na podstawie zadanego obrazu. Przejmuje wlasnosc nad obrazem.
	 */
	Track( QPixmap*, int cellSize );

	/**
	 * Jaki destruktor jest, kazdy widzi.
	 */
	~Track();

	const QString& load( const QDomDocument& doc );

	void store( const QDomDocument& doc );

	void setDataResolution( int size );

	int getCellSize() {
		return mpTrackData->getCellSize();
	}

	int getCellSizeLogarithm() {
		return mpTrackData->getCellSizeLogarithm();
	}

	bool hasName() {
		return mHasName;
	}

	void setName( const QString& name ) {
		mName = name;
		mHasName = !name.isEmpty();
	}

	const QString& getName() {
		return mName;
	}

	bool isSaved() {
		return mSaved;
	}

	void setSaved( bool value ) {
		mSaved = value;
	}

	/**
	 * @return Szerokosc trasy w pikselach.
	 */
	int getWidth() {
		return mWidth;
	}

	/**
	 * @return Wysokosc trasy w pikselach.
	 */
	int getHeight() {
		return mHeight;
	}

	const TrackData& getPoint( int x, int y ) {
		return mpTrackData->pickData( x, y );
	}

	const TrackData& getCell( int x, int y ) {
		return mpTrackData->pickCell( x, y );
	}

	void setPoint( int x, int y, const TrackData& data ) {
		mpTrackData->setData( x, y, data );
		if( mSaved ) mSaved = false;
	}

	void setCell( int x, int y, const TrackData& data ) {
		mpTrackData->setCell( x, y, data );
		if( mSaved ) mSaved = false;
	}

	QPixmap& getImage() {
		return *mpPixmap;
	}

	void reloadImage( QPixmap*, int cellSize );
};

#endif // TRACK_H
