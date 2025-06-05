#ifndef TYPES_H
#define TYPES_H

enum eReturnType_t
{
    RET_OK,
    RET_NOT_OK,

    //Book
    RET_ORDER_EXISTS,
    RET_ORDER_NOT_EXISTS,
    RET_BOOK_EMPTY,

    //Event Bus
    RET_RECEIVER_EXISTS,
    RET_RECEIVER_NOT_EXISTS,
    RET_EVENT_ID_INVALID,

    RET_INVALID
};

typedef enum eReturnType_t ReturnType;

#endif
