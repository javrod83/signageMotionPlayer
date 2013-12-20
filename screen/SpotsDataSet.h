#pragma once

#include "ofMain.h"
#include "Spot.h"

class SpotsDataSet{
	
	public:
		vector<Spot *> spots;

		SpotsDataSet(ofxJSONElement json){
			cout << "[DataSet::loadSpots]begin: " << endl;
			int spotCount = json["spots"].size();
			for (int spotIndex = 0 ; spotIndex< spotCount;spotIndex++)
				{
					Spot * aSpot = new Spot();
					aSpot->setCurrentFrame(0);
					//aSpot->loadPoints(json["spots"][spotIndex]["points"],json["spots"][spotIndex]["number"].asInt());
					aSpot->loadBackPoints(json["spots"][spotIndex]["points_back"],json["spots"][spotIndex]["number"].asInt());
					aSpot->loadFrontPoints(json["spots"][spotIndex]["points_front"],json["spots"][spotIndex]["number"].asInt());
					spots.push_back(aSpot);
				}

			cout << "[DataSet::loadSpots]end: " << endl;
		}
		
};


