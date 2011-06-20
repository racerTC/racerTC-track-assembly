#include "TrackView.h"

#include <QResizeEvent>
#include <QPaintEvent>
#include <QPainter>

#include <cmath>

/**
 * Oblicza odleglosc punktu (x1,y1) od punktu (x2,y2).
 */
inline float Dist( int x1, int y1, int x2, int y2 ) {
	return sqrt( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) );
}

TrackView::TrackView(QWidget *parent) :
	QWidget(parent)
,	mWidth( 0 ), mHeight( 0 )
,	mHorizontalPos( 0 ), mVerticalPos( 0 )
,	mpTrack( 0 )
,	mViewMode( vmTrack )
,	mGridDensity( 1 ), mDrawGrid( true )
,	mMouseOverView( false ), mMousePressed( false )
,	mMouseX( 0 ), mMouseY( 0 )
,	mBrushId( 0 ), mBrushSize( 1 ), mBrushIntensity( 0 )
{
	setMouseTracking( true );
}

QBrush TrackView::getCurrentQBrush() {
	switch( mViewMode ) {
	case vmTerrain:
		switch( mBrushId ) {
		case 0: return makeResistantBrush( mBrushIntensity );
		case 1: return makeSlopyBrush( mBrushIntensity );
		case 2: return makeImpassableBrush();
		}
	default:
		return Qt::NoBrush;
	}
}

bool TrackView::cellUnderBrush( int cx, int cy ) {
	int x = mHorizontalPos + mMouseX;
	int y = mVerticalPos + mMouseY;
	int track_width = mpTrack->getWidth();
	int track_height = mpTrack->getHeight();
	if( x > track_width || y > track_height )
		return false; // jezeli myszka jest poza trasa, to pedzla nie ma
	int radius = mBrushSize - 1;

	// TODO: dzielenie przez shift'y
	int cellSize = mpTrack->getCellSize();
	int px = x / cellSize;
	int py = y / cellSize;

	if( static_cast<int>( Dist( px, py, cx, cy ) ) <= radius )
		return true;
	return false;
}

void TrackView::paintEvent( QPaintEvent *event ) {	
	if( mpTrack == 0 ) {
		QWidget::paintEvent( event );
		return;
	}

	QPainter p( this );

	int x = mHorizontalPos;
	int y = mVerticalPos;

	if( mpTrack->getWidth() <= mWidth )
		x = 0;
	if( mpTrack->getHeight() <= mHeight )
		y = 0;

	p.drawPixmap(
			0, 0,
			mpTrack->getImage(),
			x,
			y,
			mWidth,
			mHeight
	);

	int shift = mpTrack->getCellSizeLogarithm();

	// oblicza wspolrzedne komorki, od ktorej nalezy rozpoczac rysowanie:
	// sx, sy - wspolrzedne pierwszego widocznego kafelka
	int sx = x >> shift;
	int sy = y >> shift;
	// ox, oy - przesuniecie kafelkow
	int ox = x & shift;
	int oy = y & shift;
	int track_width = mpTrack->getWidth();
	int track_height = mpTrack->getHeight();
	// ilosc widocznych kafelkow
	int ax = ( ((track_width < mWidth) ? track_width : mWidth) >> shift) - 1;
	int ay = ( ((track_height < mHeight) ? track_height : mHeight) >> shift) - 1;

	p.setPen( Qt::NoPen );

	int cellSize = mpTrack->getCellSize();

	/* Rysuje overlay nad trasa: */
	switch( mViewMode ) {
	case vmTrack:
		// tak naprawde nie ma tu czego rysowac...
		break;
	case vmTerrain: // przejezdnosc terenu...
		for( int i = 0; i <= ax; ++i  ) {
			for( int j = 0; j <= ay; ++j ) {
				char f = mpTrack->getCell( sx+i, sy+j ).friction;
				// ustawia odpowiedni QBrush:
				if( !mMouseOverView || mMousePressed || !cellUnderBrush( sx+i, sy+j ) ) {
					if( f == TrackData::IMPASSABLE )
						p.setBrush( makeImpassableBrush() );
					else if( f > 0 )
						p.setBrush( makeResistantBrush( f ) );
					else if( f < 0 )
						p.setBrush( makeSlopyBrush( -f ) );
					else
						p.setBrush( Qt::NoBrush );
				} else
					p.setBrush( getCurrentQBrush() );
				// rysuje overlay nad tilem:
				p.drawRect(	(i << shift) - ox, (j << shift) - oy, cellSize, cellSize );
			}
		}
		break;
	case vmFX: // efekty:
		break;
	}

	p.setPen( QPen(Qt::white) );

	if( mDrawGrid && cellSize > 1 ) {
		int track_height = mpTrack->getWidth();
		// linie poziome
		for( int i = ax + 1; i>=0; --i ) {
			int x_pos = -ox + cellSize*i;
			p.drawLine(
					x_pos,
					0,
					x_pos,
					(track_height < mHeight) ? track_height : mHeight
			);
		}

		int track_width = mpTrack->getHeight();
		for( int j = ay + 1; j>=0; --j ) {
			int y_pos = -oy + cellSize*j;
			p.drawLine(
					0,
					y_pos,
					(track_width < mWidth) ? track_width : mWidth,
					y_pos
			);
		}
	}
}

void TrackView::setTrack( Track *track ) {
	if( track != 0 ) {
		mGridDensity = track->getCellSize();
		mpTrack = track;
		adjustSliders();
	} else {
		mpTrack = track;
	}
	update();
}

void TrackView::adjustSliders() {
	if( mpTrack != 0 ) {
		int slider_resolution_horizontal = mpTrack->getWidth() - mWidth;
		int slider_resolution_vertical = mpTrack->getHeight() - mHeight;

		// wlacza lub wylacza slidery oraz emituje nowe zakresy:
		if( slider_resolution_horizontal <= 0 ) {
			//emit horizontalSliderRangeChanged( 0, 0 );
			emit horizontalSliderEnabled( false );
		} else {
			emit horizontalSliderRangeChanged( slider_resolution_horizontal );
			emit horizontalSliderEnabled( true );
		}
		if( slider_resolution_vertical <= 0 ) {
			//emit verticalSliderRangeChanged( 0, 0 );
			emit verticalSliderEnabled( false );
		} else {
			emit verticalSliderRangeChanged( slider_resolution_vertical );
			emit verticalSliderEnabled( true );
		}

	} else {
		horizontalSliderEnabled( false );
		verticalSliderEnabled( false );
	}
}

void TrackView::resizeEvent( QResizeEvent* event ) {
	const QSize& size = event->size();
	// zapisuje nowy rozmiar:
	mWidth = size.width();
	mHeight = size.height();
	// poprawia suwaki:
	adjustSliders();

	// odswierza widget:
	update();
}

void TrackView::setGridDensity( int density ) {
	mGridDensity = density;
	mpTrack->setDataResolution( density );
	update();
}

void TrackView::drawGrid( bool yes_no ) {
	mDrawGrid = yes_no;
	update();
}

void TrackView::brushCell( int x, int y ) {
	TrackData data( mpTrack->getCell(x,y) );
	switch( mViewMode ) {
	case vmTerrain:
		switch( mBrushId ) {
		case 0: data.friction = mBrushIntensity; break;
		case 1: data.friction = -mBrushIntensity; break;
		case 2: data.friction = TrackData::IMPASSABLE; break;
		}
		break;
	default:
		break;
	}
	mpTrack->setCell( x, y, data );
}

void TrackView::brushTrack() {
	// badamy, nad jakim segmentem trasy znajduje sie kursor
	int x = mHorizontalPos + mMouseX;
	int y = mVerticalPos + mMouseY;
	int track_width = mpTrack->getWidth();
	int track_height = mpTrack->getHeight();
	if( x > track_width || y > track_height )
		return; // jezeli myszka jest poza trasa, nic nie trzeba robic

	int shift = mpTrack->getCellSizeLogarithm();

	// TODO: dzielenie przez shift'y
	//int cellSize = mpTrack->getCellSize();
	//x = x / cellSize;
	//y = y / cellSize;

	x >>= shift;
	y >>= shift;
	track_width >>= shift;
	track_height >>= shift;

	// maluje segment wskazywany przez kursor
	int radius = mBrushSize - 1;
	for( int i = x-radius; i <= x+radius; ++i )
		for( int j = y-radius; j <= y+radius; ++j )
			if( (i >= 0) && (j >= 0) && (i < track_width) && (j < track_height) )
				if( static_cast<int>( Dist( i, j, x, y ) ) <= radius )
					brushCell( i, j );

}

void TrackView::mouseMoveEvent ( QMouseEvent* event ) {
	if( mpTrack == 0 ) return;
	if( mMouseOverView ) {
		mMouseX = event->pos().x();
		mMouseY = event->pos().y();
		if( mMousePressed )
			brushTrack();
		update();

		int x = mMouseX + mHorizontalPos;
		int y = mMouseY + mVerticalPos;
		if( x < mpTrack->getWidth() && y < mpTrack->getHeight() ) {
			const TrackData& data = mpTrack->getPoint( x, y );
			switch( mViewMode ) {

			case vmTrack: // widok trasy
				emit showTip( tr("Mysz nad punktem(") + QString::number(mMouseX) + tr(",") + QString::number(mMouseY) + tr(")") );
				break;

			case vmTerrain: // tryb edycji terenu
				if( data.friction == TrackData::IMPASSABLE )
					emit showTip( tr("Mysz nad punktem(") + QString::number(mMouseX) + tr(",") + QString::number(mMouseY) + tr(") - teren nieprzejezdny") );
				else if( data.friction >= 0 )
					emit showTip( tr("Mysz nad punktem(") + QString::number(mMouseX) + tr(",") + QString::number(mMouseY) + tr(") - spowolnienie ruchu: ") + QString::number( data.friction ) );
				else
					emit showTip( tr("Mysz nad punktem(") + QString::number(mMouseX) + tr(",") + QString::number(mMouseY) + tr(") - sliski teren: ") + QString::number( -data.friction ) );
				break;

			default:
				break;
			}
		} else
			emit removeTip();
	}
}

void TrackView::enterEvent( QEvent* event ) {
	Q_UNUSED( event );
	mMouseOverView = true;
}

void TrackView::leaveEvent( QEvent* event ) {
	Q_UNUSED( event );
	mMouseOverView = false;
	mMousePressed = false;
	emit removeTip();
}


void TrackView::mousePressEvent ( QMouseEvent* event ) {
	Q_UNUSED( event );
	if( mpTrack == 0 ) return;
	// zapisuje brusha do tracka...
	mMousePressed = true;
	brushTrack();
}

void TrackView::mouseReleaseEvent ( QMouseEvent* event ) {
	Q_UNUSED( event );
	mMousePressed = false;
}

void TrackView::setHorizontalPosition( int value ) {
	mHorizontalPos = value;
	update();
}

void TrackView::setVerticalPosition( int value ) {
	mVerticalPos = value;
	update();
}

void TrackView::setMode( int mode ) {
	mViewMode = ViewMode( mode );
	update();
}

void TrackView::setBrushId( int brushId ) {
	mBrushId = brushId;
}

void TrackView::setBrushSize( int brushSize  ) {
	mBrushSize = brushSize;
}

void TrackView::setBrushIntensity( int brushIntensity ) {
	mBrushIntensity = brushIntensity;
}
