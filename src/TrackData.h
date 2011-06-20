#ifndef TRACKDATA_H
#define TRACKDATA_H

struct TrackData
{
	static const char IMPASSABLE;

    TrackData();
	/**
	 * Wspolczynnik tarcia.
	 * <ul>Interpretacja:
	 * <li> 127 - nieskonczonosc - teren nieprzejezdny
	 * <li> 1..126 - opor ruchu - im wieksza wartosc, tym opor wiekszy
	 * <li> 0 - brak oporow
	 * <li> -126..-1 - brak tarcia
	 * <li> -127 - trudno powiedziec :P
	 * </ul>
	 */
	char friction;


	char gfx_flags;
};

#endif // TRACKDATA_H
