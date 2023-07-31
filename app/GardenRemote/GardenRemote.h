/*
 * GardenRemote.h
 *
 *  Created on: Jul 15, 2023
 *      Author: hgrav
 */

#ifndef GARDENREMOTE_H_
#define GARDENREMOTE_H_

struct dataFrameStruct {
	uint8_t		nodeID;
	float 		soilTempC;
	float 		ambiantHumidity;
	float 		ambiantTempC;
	uint32_t   	lumens;

};



#endif /* GARDENREMOTE_H_ */
