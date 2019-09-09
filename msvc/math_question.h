#pragma once
#include "p_mobj.h"
#define KEY_ANSWERA 'q'
#define KEY_ANSWERB 'w'
#define KEY_ANSWERC 'r'
#define KEY_ANSWERD 't'

//
//answer question
//
void
AnswerQuestion(
	int key
);

void
askQuestion
(   mobj_t*	target,
	mobj_t*	inflictor,
	mobj_t*	source,
	int 	damage
);

const char *
getQuestionImage
(
	const uint8_t questionNumber
);

const uint8_t
getQuestionNum
(
);