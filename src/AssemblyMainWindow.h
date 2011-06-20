#ifndef ASSEMBLYMAINWINDOW_H
#define ASSEMBLYMAINWINDOW_H

#include <QMainWindow>
#include "TrackView.h"

namespace Ui {
    class AssemblyMainWindow;
}

/**
 * Glowne okno aplikacji. Zarzadza wszystkimi znajdujacymi sie w nim elementami.
 * @author Piotr Balut
 */
class AssemblyMainWindow : public QMainWindow
{
    Q_OBJECT
private:
	Track* mpTrack;

	struct BrushData {
		int size;
		int intensity;
		BrushData(): size(1), intensity(0) {}
	};

	BrushData mpTerrainBrushes[3];
	int mCurrentTerrainBrush;

	void save( const QString& name );

public:
    explicit AssemblyMainWindow(QWidget *parent = 0);
    ~AssemblyMainWindow();

	void loadImage( const QString& imageName );

protected:
	void closeEvent ( QCloseEvent * event );

signals:

	void imageLoaded();

public slots:

	void newTrack();

	void openTrack();

	void saveTrack();

	void saveTrackAs();

	void loadImage();

	void enableMenus();

	void disableMenus();

	void setHorizontalSliderMaximum( int );
	void setVerticalSliderMaximum( int );

	void setGridDensity( int );

	void terrainChangeBrush( int );

	void prepareTab( int );

private:
    Ui::AssemblyMainWindow *ui;
	TrackView *trackView;
};

#endif // ASSEMBLYMAINWINDOW_H
