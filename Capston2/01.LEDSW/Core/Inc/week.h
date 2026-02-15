#include "stdint.h"

enum WEEK{
	SUN = 0,
	MON,
	TUE,
	WED,
	THU,
	FRI,
	SAT
};

enum MONTH{
	Jan = 1,
	Feb,
	Mar,
	Apr,
	May,
	Jun,
	Jul,
	Aug,
	Sep,
	Oct,
	Nov,
	Dec
};

struct _DayOfMonth{
	char name[5];	
	uint16_t lengthOfDay;
}DayOfMonth, PDayOfMonth;
