#include <stdint.h>
#include <math.h>
#include "math_question.h"
#include "doomstat.h"
#include "m_controls.h"
#include "sounds.h"

//note to self
//images are structed. or maybe the whole game is streched
//from 59 to 141.??
//just muiltply the height by 5/6 (.833333333)
//max width is 320
//max height is 168
//max line length is 43

char questionString[256];
uint8_t questionNum;

typedef struct {
	const char * question;
	const char * answers[4];
	const char answer;
	const char image[8];
} MultipleChoiceQuestion;

const MultipleChoiceQuestion questions[] = {
	{ "What are supplementary angles?",
	{ "an angle whose sum is greater than 180\ndegrees",
	   "either of two angles whose sum is 180\ndegrees",
	   "Angles that add up to 180 degrees",
	   "two angles that add up to be 360 degrees" },
	KEY_ANSWERC, "AMMNUM0" },

	{ "What are the corresponding angles in this\npicture?",
	{ "1&2, 3&4, 5&6, 7&8", "1&4, 2&3, 5&8, 6&7", "1&3, 2&4, 5&7, 6&8", "1&5, 3&7, 2&6, 4&8" },
	KEY_ANSWERD, "MATH_Q3" },

	{ "What are the alternate interior angles in\nthis picture?",
	{ "3&4, 5&6", "3&6, 4&5", "4&6, 3&5", "7&2, 8&1" },
	KEY_ANSWERB, "MATH_Q3" },

	{ "What other angles measure 110?",
	{ "<1, <4, <8", "<7, <8, <6", "<3, <4, <6", "<7, <3, <1" },
	KEY_ANSWERA, "MATH_Q3" },

	{ "find x and y",
	{ "x= 75 y= 55", "x= 55 y= 70", "x= 70 y= 55", "x= 55 y= 75"},
	KEY_ANSWERA, "MATH_Q5" },

	{ "find x and y",
	{ "x= 80, y= 80", "x= 100, y= 80", "x= 80, y= 100", "x= 100, y= 100" },
	KEY_ANSWERA, "MATH_Q6" },

	{ "Which of these are corresponding angles\n\n\n\n",
	{ "<1 and <7, <2 and <3", "<8 and <3, <1 and <7\n\n\n\n\n", "<5 and <6, <4 and <3", "<5 and <9, <4 and <7\n\n\n" },
	KEY_ANSWERB, "MATH_Q7" },

	{ "Which of these are alternate interior angles\n\n\n\n",
	{ "<1 and <4, <2 and <6", "<9 and <8, <4 and <7\n\n\n\n\n", "<4 and <7, <5 and <8", "<8 and <9, <4 and <1\n\n\n" },
	KEY_ANSWERC, "MATH_Q7" },

	{ "Which of these are Same-side interior angles\n\n\n\n",
	{ "<6 and <8", "<4 and <7\n\n\n\n\n", "<5 and <9", "<8 and <9\n\n\n" },
	KEY_ANSWERA, "MATH_Q7" },

	{ "what is the slope of line b?",
	{ "4/5", "-4/-5", "-4/5", "5/-4" },
	KEY_ANSWERC, "MATH_Q10" },

	{ "Determine the value of x for which a II b",
	{ "35", "56", "112", "53" },
	KEY_ANSWERD, "MATH_Q11" },

	{ "Find each missing angle measure.",
	{ "3y= -120, (y-15)= -25", "3y = 120, (y-15) = 25", "3y= 120, (y-15)= -25", "3y= -120, (y-15)= 25" },
	KEY_ANSWERB, "MATH_Q12" },

	{ "determine whether statement is\nalways, sometimes, or never.\nPerpendicular lines meet at the right angle.",
	{ "sometimes", "never", "always", "I don't know" },
	KEY_ANSWERC, "AMMNUM0" },

	{ "Which theorem or postulate is this?\nIf a transversal intersects two parallel\nlines, then the same-side interior angles are\nsupplementary.",
	{ "alternate interior angles theorem",
	  "corresponding angle theorem",
	  "same-side interior angles postulate",
	  "alternate exterior angles theorem" },
	KEY_ANSWERC, "AMMNUM0" },

	{ "Which theorem or postulate is this?\nif a transversal intersects two parallel\nlines, then corresponding angles are\ncongruent.",
	{ "alternate interior angles theorem",
	  "corresponding angle theorem",
	  "same-side interior angles postulate",
	  "alternate exterior angles theorem" },
	KEY_ANSWERB, "AMMNUM0" },

	{ "Which theorem or postulate is this?\nif a transversal intersects two parallel\nlines, then alternate interior angle are\ncongruent",
	{ "alternate interior angles theorem",
	  "corresponding angle theorem",
	  "same-side interior angles postulate",
	  "alternate exterior angles theorem" },
	KEY_ANSWERA, "AMMNUM0" },
};

const unsigned int numOfQuestion = sizeof(questions) / sizeof(MultipleChoiceQuestion);

mobj_t* storedTarget;
mobj_t*	storedInflictor;
mobj_t*	storedSource;
int 	storedDamage;

void
AnswerQuestion
(int key) {
	static int numOfRightAnswers = 0;
	int* keybind;
	switch (key) {
	case KEY_ANSWERA: keybind = &key_math_answerA; break;
	case KEY_ANSWERB: keybind = &key_math_answerB; break;
	case KEY_ANSWERC: keybind = &key_math_answerC; break;
	case KEY_ANSWERD: keybind = &key_math_answerD; break;
	default: return;
	}
	if (questions[questionNum].answer == *keybind) {
		//reset controls
		key_math_answerA = KEY_ANSWERA;
		key_math_answerB = KEY_ANSWERB;
		key_math_answerC = KEY_ANSWERC;
		key_math_answerD = KEY_ANSWERD;
		players[0].message = "good";
		if (numOfQuestion <= ++questionNum) {
			players[0].message = "You've answered all questions. good job";
			S_StartSound(players[0].mo, sfx_perf);
		}
		players[0].didAnswerQuestion = true;
		P_GiveBody(players, storedDamage);
		numOfRightAnswers = numOfRightAnswers < 0 ? 1 : numOfRightAnswers + 1;
		if (numOfRightAnswers == 5) S_StartSound(players[0].mo, sfx_hanzo);
	} else {
		*keybind = -1;
		players[0].message = "wrong";

		players[0].didAnswerQuestion = true;
		numOfRightAnswers = 0 < numOfRightAnswers ? -1 : numOfRightAnswers - 1;
		P_DamageMobj(storedTarget, storedInflictor, storedSource, (int)(storedDamage*pow(2.8, (numOfRightAnswers * -1) - 1)));
	}
	players[0].didAnswerQuestion = false;
}

void
askQuestion
( mobj_t*	target,
  mobj_t*	inflictor,
  mobj_t*	source,
  int 		damage)
{
	storedTarget = target;
	storedInflictor = inflictor;
	storedSource = source;
	storedDamage = damage;
	if (numOfQuestion <= questionNum) return;
	DEH_snprintf(questionString, sizeof(questionString), "%s\n\nQ. %s\nW. %s\nR. %s\nT. %s", 
		questions[questionNum].question, questions[questionNum].answers[0],
		questions[questionNum].answers[1], questions[questionNum].answers[2],
		questions[questionNum].answers[3]);
	M_StartMessage(questionString, AnswerQuestion, true);
}

const char * getQuestionImage
( const uint8_t quesitonNumber)
{
	return questionNum < numOfQuestion ? questions[questionNum].image : "AMMNUM0";
}

const uint8_t
getQuestionNum()
{
	return questionNum;
}