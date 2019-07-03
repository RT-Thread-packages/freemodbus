#ifndef    USER_APP
#define USER_APP
/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mb_m.h"
#include "mbconfig.h"
#include "mbframe.h"
#include "mbutils.h"

/* -----------------------Slave Defines -------------------------------------*/
#define S_DISCRETE_INPUT_START                    RT_S_DISCRETE_INPUT_START
#define S_DISCRETE_INPUT_NDISCRETES               RT_S_DISCRETE_INPUT_NDISCRETES
#define S_COIL_START                              RT_S_COIL_START
#define S_COIL_NCOILS                             RT_S_COIL_NCOILS
#define S_REG_INPUT_START                         RT_S_REG_INPUT_START
#define S_REG_INPUT_NREGS                         RT_S_REG_INPUT_NREGS
#define S_REG_HOLDING_START                       RT_S_REG_HOLDING_START
#define S_REG_HOLDING_NREGS                       RT_S_REG_HOLDING_NREGS
/* salve mode: holding register's all address */
#define          S_HD_RESERVE                     RT_S_HD_RESERVE
/* salve mode: input register's all address */
#define          S_IN_RESERVE                     RT_S_IN_RESERVE
/* salve mode: coil's all address */
#define          S_CO_RESERVE                     RT_S_CO_RESERVE
/* salve mode: discrete's all address */
#define          S_DI_RESERVE                     RT_S_DI_RESERVE

/* -----------------------Master Defines -------------------------------------*/
#define M_DISCRETE_INPUT_START                    RT_M_DISCRETE_INPUT_START
#define M_DISCRETE_INPUT_NDISCRETES               RT_M_DISCRETE_INPUT_NDISCRETES
#define M_COIL_START                              RT_M_COIL_START
#define M_COIL_NCOILS                             RT_M_COIL_NCOILS
#define M_REG_INPUT_START                         RT_M_REG_INPUT_START
#define M_REG_INPUT_NREGS                         RT_M_REG_INPUT_NREGS
#define M_REG_HOLDING_START                       RT_M_REG_HOLDING_START
#define M_REG_HOLDING_NREGS                       RT_M_REG_HOLDING_NREGS
/* master mode: holding register's all address */
#define          M_HD_RESERVE                     RT_M_HD_RESERVE
/* master mode: input register's all address */
#define          M_IN_RESERVE                     RT_M_IN_RESERVE
/* master mode: coil's all address */
#define          M_CO_RESERVE                     RT_M_CO_RESERVE
/* master mode: discrete's all address */
#define          M_DI_RESERVE                     RT_M_DI_RESERVE

#endif
