#include "AssemblyMainWindow.h"
#include "ui_AssemblyMainWindow.h"

#include <QFileDialog>
#include <QPixmap>
#include <QString>
#include <QCloseEvent>
#include <QtXml/QDomDocument>

#include <cmath>
#include <exception>

AssemblyMainWindow::AssemblyMainWindow(QWidget *parent) :
	QMainWindow( parent )
,	mpTrack( 0 )
,	mCurrentTerrainBrush( 0 )
,	ui( new Ui::AssemblyMainWindow )
,	trackView( new TrackView() )
{	
	trackView->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
	trackView->setMinimumSize( 100, 100 );

	ui->setupUi(this);
	QGridLayout* container_layout = new QGridLayout( ui->viewContainer );
	//((QGridLayout*) ui->CentralFrame->layout())->addWidget( trackView, 1, 1 );

	container_layout->setContentsMargins( 0, 0, 0, 0 );
	container_layout->setSpacing( 0 );

	container_layout->addWidget( trackView, 0, 0 );

	// konfiguruje suwaki:
	ui->horizontalScrollBar->setSingleStep( 5 );
	ui->horizontalScrollBar->setPageStep( 20 );
	ui->horizontalScrollBar->setMinimum( 0 );


	ui->verticalScrollBar->setSingleStep( 5 );
	ui->verticalScrollBar->setPageStep( 20 );
	ui->verticalScrollBar->setMinimum( 0 );

	// status bar:
	connect( trackView, SIGNAL(showTip(QString)), ui->statusBar, SLOT(showMessage(QString)) );
	connect( trackView, SIGNAL(removeTip()), ui->statusBar, SLOT(clearMessage()) );

	// zakresy:
	connect( trackView, SIGNAL(horizontalSliderRangeChanged(int)), this, SLOT(setHorizontalSliderMaximum(int)) );
	connect( trackView, SIGNAL(verticalSliderRangeChanged(int)), this, SLOT(setVerticalSliderMaximum(int)) );

	// wlaczanie i wylaczanie scrollbarow:
	connect( trackView, SIGNAL(horizontalSliderEnabled(bool)), ui->horizontalScrollBar, SLOT(setEnabled(bool)));
	connect( trackView, SIGNAL(verticalSliderEnabled(bool)), ui->verticalScrollBar, SLOT(setEnabled(bool)));

	// sprzezenie zwrotne scrollbarow do TrackView:
	connect( ui->horizontalScrollBar, SIGNAL(valueChanged(int)), trackView, SLOT(setHorizontalPosition(int)) );
	connect( ui->verticalScrollBar, SIGNAL(valueChanged(int)), trackView, SLOT(setVerticalPosition(int)) );

	// guzik wczytywania obrazkow:
	connect( ui->loadImageButton, SIGNAL(clicked()), this, SLOT(loadImage()) );

	// taby:
	connect( ui->tabWidget, SIGNAL(currentChanged(int)), trackView, SLOT(setMode(int)) );
	connect( ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(prepareTab(int)) );

	// siatka:
	connect( ui->gridSlider, SIGNAL(valueChanged(int)), this, SLOT(setGridDensity(int)) );

	// menu:
	connect( ui->actionNewTrack, SIGNAL(triggered()), this, SLOT(newTrack()) );
	connect( ui->actionOpen, SIGNAL(triggered()), this, SLOT(openTrack()) );
	connect( ui->actionSave, SIGNAL(triggered()), this, SLOT(saveTrack()) );
	connect( ui->actionSaveAs, SIGNAL(triggered()), this, SLOT(saveTrackAs()) );
	connect( ui->actionClose, SIGNAL(triggered()), this, SLOT(close()) );
	connect( ui->actionDrawGrid, SIGNAL(toggled(bool)), trackView, SLOT(drawGrid(bool)) );

	// zakladka terenu:
	connect( ui->comboTerrainBrushType, SIGNAL(currentIndexChanged(int)), this, SLOT(terrainChangeBrush(int)) );
	connect( ui->comboTerrainBrushType, SIGNAL(currentIndexChanged(int)), trackView, SLOT(setBrushId(int)) );
	connect( ui->sliderTerrainBrushSize, SIGNAL(valueChanged(int)), trackView, SLOT(setBrushSize(int)) );
	connect( ui->sliderTerrainIntensity, SIGNAL(valueChanged(int)), trackView, SLOT(setBrushIntensity(int)) );
	connect( ui->sliderTerrainBrushSize, SIGNAL(valueChanged(int)),ui->lineTerrainBrushSize,SLOT(setValue(int)) );
	connect( ui->sliderTerrainIntensity, SIGNAL(valueChanged(int)),ui->lineTerrainIntensity,SLOT(setValue(int)) );

	// koncowa inicjalizacja...
	disableMenus();
}

AssemblyMainWindow::~AssemblyMainWindow()
{
	if( mpTrack != 0 ) delete mpTrack;
    delete ui;
}

void AssemblyMainWindow::prepareTab( int tabId ) {
	switch( tabId ) {
	case 0: // ustawienia
		break;
	case 1: // teren
		trackView->setBrushId( mCurrentTerrainBrush );
		trackView->setBrushIntensity( mpTerrainBrushes[mCurrentTerrainBrush].intensity );
		trackView->setBrushIntensity( mpTerrainBrushes[mCurrentTerrainBrush].size );
		break;
	case 2: // efekty
		break;
	}
}

void AssemblyMainWindow::terrainChangeBrush( int brushId ) {
	// pokazuje/chowa elementy zakladki:
	bool visible = true;
	if( brushId == 2 ) visible = false;
	ui->labelTerrainBrushDescription->setVisible( visible );
	ui->labelTerrainHigh->setVisible( visible );
	ui->labelTerrainNone->setVisible( visible );
	ui->lineTerrainIntensity->setVisible( visible );
	ui->sliderTerrainIntensity->setVisible( visible );

	// zapisuje ustawienia aktualnego brusha
	mpTerrainBrushes[mCurrentTerrainBrush].intensity = ui->sliderTerrainIntensity->value();
	mpTerrainBrushes[mCurrentTerrainBrush].size = ui->sliderTerrainBrushSize->value();

	// zapisuje numer aktualnego brusha terenu
	mCurrentTerrainBrush = brushId;

	// wczytuje ustawienia pedzla:
	ui->sliderTerrainIntensity->setValue( mpTerrainBrushes[brushId].intensity );
	ui->sliderTerrainBrushSize->setValue( mpTerrainBrushes[brushId].size );
}

void AssemblyMainWindow::setGridDensity( int density_exponent ) {
	double density = pow( 2.0, density_exponent );
	ui->labelGrid->setText( QString::number(density) );
	trackView->setGridDensity( static_cast<int>(density) );
}

void AssemblyMainWindow::setHorizontalSliderMaximum( int max ) {
	ui->horizontalScrollBar->setMaximum( max );
}

void AssemblyMainWindow::setVerticalSliderMaximum( int max ) {
	ui->verticalScrollBar->setMaximum( max );
}

void AssemblyMainWindow::closeEvent( QCloseEvent * event ) {

}

void AssemblyMainWindow::newTrack() {
	if( mpTrack != 0 ) {
		delete mpTrack;
		mpTrack = 0;
	}
	disableMenus();
}

void AssemblyMainWindow::openTrack() {
	QString name = QFileDialog::getOpenFileName(
			this, // parent
			tr("Wczytaj trasê..."),
			tr("./"),
			tr("Plik trasy (*.rtc)")
	);
	QDomDocument doc( name );
	QFile file( name );
	if( file.open( QIODevice::ReadOnly ) ) {
		if( doc.setContent( &file ) ) {
			if( mpTrack != 0 ) {

				delete mpTrack;
			}
			mpTrack = new Track();
			QString img = mpTrack->load( doc );
			loadImage( img );
		}
	}
	file.close();
}

void AssemblyMainWindow::save( const QString& name ) {
	mpTrack->setSaved( true );
	QDomDocument doc( name );
	mpTrack->store( doc );
	QFile file( name );
	if( file.open( QIODevice::WriteOnly ) ) {
		file.write( doc.toByteArray() );
	}
	file.close();
}

void AssemblyMainWindow::saveTrack() {
	if( !mpTrack->isSaved() ) {
		if( !mpTrack->hasName() )
			saveTrackAs();
		else {
			save( mpTrack->getName() );
		}
	}
}

void AssemblyMainWindow::saveTrackAs() {
	QString name = QFileDialog::getSaveFileName(
			this,
			tr("Zapisz trasê jako..."),
			tr("./"),
			tr("Plik trasy (*.rtc)")
	);
	if( !name.isNull() ) {
		mpTrack->setName( name );
		save( name );
	}
}

void AssemblyMainWindow::enableMenus() {
	ui->loadImageButton->setEnabled( false );

	// info boxy:
	ui->groupInfo->setVisible( true );
	ui->groupParams->setVisible( true );

	// taby:
	ui->tab_terrain->setEnabled( true );

}

void AssemblyMainWindow::disableMenus() {
	ui->loadImageButton->setEnabled( true );

	// info box'y:
	ui->groupInfo->setVisible( false );
	ui->groupParams->setVisible( false );

	// taby:
	ui->tab_terrain->setEnabled( false );
}

void AssemblyMainWindow::loadImage( const QString& imageName ) {
	QPixmap* img = new QPixmap();
	try {
		if( img->load(imageName) ) {
			// udalo sie zaladowac obrazek. Czy trasa jest juz utworzona?
			if( mpTrack != 0 ) {
				// aktualizujemy obrazek istniejacej trasy
				mpTrack->reloadImage( img, static_cast<int>( pow( 2, ui->gridSlider->value() ) ) );
			} else {
				// tworzy nowa trase
				mpTrack = new Track( img, static_cast<int>( pow( 2, ui->gridSlider->value() ) ) );
			}
			trackView->setTrack( mpTrack );
			ui->loadImageButton->setEnabled( false );
			ui->labelImageState->setText("Stan obrazka: <font color=\"green\">OK</font>");
			ui->labelResolution->setText( QString::number(mpTrack->getWidth()) + QString("x") + QString::number(mpTrack->getHeight()) );
			enableMenus();
			emit imageLoaded();
		} else {
			delete img;
		}
	} catch( std::exception& e ) {
		if( img != 0 ) delete img;
		if( mpTrack != 0 ) delete mpTrack;
		trackView->setTrack( 0 );
		ui->labelImageState->setText("Stan obrazka: <font color=\"red\">b³¹d</font>");
		ui->labelResolution->setText("Niepoprawny rozmiar.");
	}

}

void AssemblyMainWindow::loadImage() {
	QString imageName = QFileDialog::getOpenFileName(
			this, // parent
			tr("Otwórz plik obrazka..."),
			tr("./"),
			tr("Obrazy (*.png *.jpg *.bmp)")
	);
	if( !imageName.isNull() ) {
		loadImage( imageName );
	}
}
