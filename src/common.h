#ifndef SOCKET_EXAMPLE_COMMON_H
#define SOCKET_EXAMPLE_COMMON_H

typedef enum
{
    FC_EXP = 1, FC_SIN, FC_COS
} func_type;
typedef struct _calc_data
{
    double x;
    func_type ft;
} calc_struct;

#endif
