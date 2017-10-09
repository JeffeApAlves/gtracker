/*
 * Cmd.h
 *
 *  Created on: Aug 30, 2017
 *      Author: Jefferson
 */

#ifndef INCLUDES_CMD_H_
#define INCLUDES_CMD_H_

/*
 *
 *Retorno das callbacks
 */
typedef enum {

	CMD_RESULT_EXEC_UNSUCCESS	= -3,
	CMD_RESULT_INVALID_CMD		= -2,
	CMD_RESULT_INVALID_PARAM	= -1,
	CMD_RESULT_EXEC_SUCCESS		= 0,

}ResultExec;

/*
 *
 * Id dos recursos
 */
typedef enum {

	CMD_NONE,
	CMD_LED,
	CMD_ANALOG,
	CMD_PWM,
	CMD_TOUCH,
	CMD_ACC,
	CMD_TLM,
	CMD_LOCK,
	CMD_LCD

}Resource;

#define SIZE_LIST_CMD	9


#endif /* INCLUDES_CMD_H_ */
