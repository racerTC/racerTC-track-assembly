#include "TrackData.h"

#include <climits>

const char TrackData::IMPASSABLE = CHAR_MAX;

TrackData::TrackData():
	friction( 0 )
,	gfx_flags( 0 )
{}
