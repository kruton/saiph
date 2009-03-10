#ifndef TEST_H
#define TEST_H

#ifdef UNITTEST
#define ASSIST_UNIT_TEST(__class) friend class __class ## Test;
#else
#define ASSIST_UNIT_TEST(__class)
#endif

#endif
