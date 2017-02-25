/*
 * Hal.h
 *
 *  Created on: Feb 25, 2017
 *      Author: banz
 */

#ifndef HAL_H_
#define HAL_H_

class Hal {
public:
	static void Init();
	static void Tick();

	static bool LedRed();
	static void LedRed(bool);
	static bool LedBlue();
	static void LedBlue(bool);
};

#endif /* HAL_H_ */
