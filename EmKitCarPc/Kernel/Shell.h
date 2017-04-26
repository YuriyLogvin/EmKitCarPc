/*
 * Shell.h
 *
 *  Created on: Apr 26, 2017
 *      Author: banz
 */

#ifndef SHELL_H_
#define SHELL_H_

class Shell {
public:
	static void Init();
	static void ProcessCommand(char* command);
};

#endif /* SHELL_H_ */
