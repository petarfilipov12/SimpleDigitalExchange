#ifndef TYPES_H
#define TYPES_H

typedef unsigned int Return_Type;

#define RET_OK      0u
#define RET_NOT_OK  1u

//Book
#define RET_ORDER_EXISTS        2u
#define RET_ORDER_NOT_EXISTS    3u
#define RET_BOOK_EMPTY          4u

//Event Bus
#define RET_RECEIVER_EXISTS     5u
#define RET_RECEIVER_NOT_EXISTS 6u
#define RET_EVENT_ID_INVALID    7u

#define RET_INVALID             255u

#endif
