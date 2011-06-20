#ifndef TRACKVIEW_H
#define TRACKVIEW_H

#include <QWidget>
#include "Track.h"

class TrackView : public QWidget
{
    Q_OBJECT
public:
	enum ViewMode {
		vmTrack,
		vmTerrain,
		vmFX
	};
private:
	/**
	 * Przechowuje aktualna szerokosc i wysokosc TrackView.
	 */
	int mWidth, mHeight;
	int mHorizontalPos, mVerticalPos;
	Track* mpTrack;
	ViewMode mViewMode;

	int mGridDensity;
	bool mDrawGrid;

	bool mMouseOverView;
	bool mMousePressed;
	int mMouseX, mMouseY;

	void adjustSliders();

	/** Ustawienia pedzla. */
	int mBrushId, mBrushSize, mBrushIntensity;

	QBrush makeImpassableBrush() {
		return QBrush( QColor( 0, 0, 0, 127 ) );
	}

	QBrush makeSlopyBrush( int intensity ) {
		if( intensity != 0 )
			return QBrush( QColor( 0, 0, 255, 50 + 126 * intensity/126 ) );
		else return Qt::NoBrush;
	}

	QBrush makeResistantBrush( int intensity ) {
		if( intensity != 0 )
			return QBrush( QColor( 255, 0, 0, 50 + 126 * intensity/126 ) );
		else return Qt::NoBrush;
	}

	QBrush getCurrentQBrush();

	void brushCell( int x, int y );
	void brushTrack();

	bool cellUnderBrush( int x, int y );

public:
	explicit TrackView( QWidget* parent = 0 );

	void setTrack( Track* track );

protected:
	void paintEvent( QPaintEvent* event );
	void resizeEvent( QResizeEvent* event );
	void mouseMoveEvent( QMouseEvent* event );
	void mousePressEvent( QMouseEvent* event );
	void mouseReleaseEvent( QMouseEvent* event );

	void enterEvent( QEvent* event );
	void leaveEvent( QEvent* event );

signals:
	void horizontalSliderRangeChanged( int max );
	void verticalSliderRangeChanged( int max );

	void verticalSliderEnabled( bool );
	void horizontalSliderEnabled( bool );

	void drawGridChangedTo( bool );

	void removeTip();
	void showTip( const QString& message );

public slots:

	void setMode( int mode );

	void setHorizontalPosition( int value );
	void setVerticalPosition( int value );

	void setGridDensity( int density );
	void drawGrid( bool );

	/**
	 * Ustawia numer aktualnie wybranego pedzla. Zachowanie pedzla obliczane jest na podstawie
	 * jego id oraz wybranego aktualnie trybu dzialania.
	 */
	void setBrushId( int );

	/**
	 * Ustawia rozmiar pedzla.
	 */
	void setBrushSize( int );

	/**
	 * Ustawia intensywnosc pedzla.
	 */
	void setBrushIntensity( int );

};

#endif // TRACKVIEW_H
