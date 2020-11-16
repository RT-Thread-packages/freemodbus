# Freemodbus

[中文页](README_ZH.md) | English

## Introduction

This is the Freemodbus protocol stack ported by armink. Support the functions of the master and slave at the same time.

FreeModbus is an open source Modbus protocol stack, but only the slave is open source, and the host source code is **charged**. At the same time, no better open source Modbus host protocol stack was found on the Internet, so this FreeModbus protocol stack supporting host mode was developed. This version of FreeModbus has been changed to V1.6 and features are as follows:

- The newly added host source code is consistent with the style and interface of the original slave;
- Support the host and slave to run in the same protocol stack;
- Support real-time operating system and bare metal transplantation;
- Provide a variety of request modes for applications, users can choose blocking or non-blocking mode, custom timeout time, etc., to facilitate flexible calls at the application layer;
- Support all common Modbus methods.

### File structure

| Source File | Description |
| --------------------------------------------- | ----------------------------------------------- |
| FreeModbus\modbus\mb.c | Provides Modbus slave settings and polling related interfaces for the application layer |
| FreeModbus\modbus\mb_m.c | Provides Modbus host settings and polling related interfaces for the application layer |
| FreeModbus\modbus\ascii\mbascii.c | ASCII mode settings and state machine |
| FreeModbus\modbus\functions\mbfunccoils.c | Slave coil related functions |
| FreeModbus\modbus\functions\mbfunccoils_m.c | Host coil related functions |
| FreeModbus\modbus\functions\mbfuncdisc.c | Slave Discrete Input Related Functions |
| FreeModbus\modbus\functions\mbfuncdisc_m.c | Discrete input related functions of the host |
| FreeModbus\modbus\functions\mbfuncholding.c | Slave holding register related functions |
| FreeModbus\modbus\functions\mbfuncholding_m.c | Host holding register related functions |
| FreeModbus\modbus\functions\mbfuncinput.c | Slave input register related functions |
| FreeModbus\modbus\functions\mbfuncinput_m.c | Host input register related functions |
| FreeModbus\modbus\functions\mbfuncother.c | Other Modbus functions |
| FreeModbus\modbus\functions\mbutils.c | Some small tools that need to be used in the protocol stack |
| FreeModbus\modbus\rtu\mbcrc.c | CRC check function |
| FreeModbus\modbus\rtu\mbrtu.c | Slave RTU mode settings and state machine |
| FreeModbus\modbus\rtu\mbrtu_m.c | Host RTU mode settings and state machine |
| FreeModbus\modbus\tcp\mbtcp.c | TCP mode settings and state machine |
| FreeModbus\port\port.c | Implement hardware porting part of the interface |
| FreeModbus\port\portevent.c | Implement slave event porting interface |
| FreeModbus\port\portevent_m.c | Implement host event and error handling porting interface |
| FreeModbus\port\portserial.c | Slave port porting |
| FreeModbus\port\portserial_m.c | Host serial port porting |
| FreeModbus\port\porttimer.c | Slave timer porting |
| FreeModbus\port\porttimer_m.c | Host timer porting |
| FreeModbus\port\user_mb_app.c | Define slave data buffer, realize the callback interface of slave Modbus function |
| FreeModbus\port\user_mb_app_m.c | Define host data buffer, realize the callback interface of host Modbus function |
| FreeModbus\samples\sample_mb_master.c | Sample code for host use |
| FreeModbus\samples\sample_mb_slave.c | Slave use sample code |
| FreeModbus\samples\README.md | Sample code description document |

> Note: All files with the _m suffix are the files that must be used in the master mode. If the slave mode is used, these files are not required.

### License

The Freemodbus software package complies with the BSD license, see the `LICENSE` file for details.

### Dependence

- RT_Thread UART device

## method of obtaining

To use the Freemodbus software package, you need to select it in the RT-Thread package manager. The specific path is as follows:

```
RT-Thread online packages
    IoT-internet of things --->
        [*] FreeModbus: Modbus master and slave stack --->
             [*] Master mode --->
             [*] Slave mode --->
```

Finally, let RT-Thread's package manager automatically update, or use the `pkgs --update` command to update the package to the BSP.

## Instructions

### Data buffer

The location defined by the data buffer is at the top of the `FreeModbus\port\user_mb_app_m.c` file, with a total of **4** data types. By default, FreeModbus slaves use **one-dimensional array** as the data structure of the buffer area. The host can store the data of all slaves in the network, so the host uses **two-dimensional array** to store all slave node data. The column number of the two-dimensional array represents the register, coil and discrete address, and the row number represents the slave node ID, but it needs to be reduced by one. For example, `usMRegHoldBuf[2][1]` means the slave ID is 3, and the register address is maintained The slave data is 1.

### Modbus data processing callback interface

Modbus has 4 different data types in total, and all Modbus functions operate around these data types. Since different user data buffer structures may be different, the corresponding Modbus data processing methods are also different, so users need to customize the operations corresponding to each data type according to their own data buffer structure. All Modbus data processing callback interfaces are as follows:

| Interface | Function description |
| ---------------------- | ------------------ |
| eMBMasterRegInputCB | Input register callback interface |
| eMBMasterRegHoldingCB | Holding register callback interface |
| eMBMasterRegCoilsCB | Coil callback interface |
| eMBMasterRegDiscreteCB | Discrete input callback interface |

> For the data buffer structure in the form of an array, the source code has already been transplanted and can be used directly.

### Initial configuration process

All configuration parameters of this protocol stack are located in `FreeModbus\modbus\include\mbconfig.h`, currently the protocol stack supports two modes of master and slave, and supports **both modes to be turned on at the same time**. The slave supports Modbus RTU, Modbus ASCII and Modbus TCP 3 modes, and the master now only supports the commonly used **Modbus RTU** mode. In the process of using the master, the user needs to configure the broadcast conversion delay time, command response timeout time and the number of slaves. It should be noted that the current protocol stack only supports continuous slave addresses, and the starting address starts from 1**.

### Normal use process

1. Call the `eMBMasterInit` method to initialize the Modbus host protocol stack, and some hardware related to the host is initialized at this time
2. Call the `eMBMasterEnable` method to start the Modbus master
3. By calling the `eMBMasterPoll` method in thread or timer polling, the polling cycle determines the response time of the command.
4. Call the host to request the API method, set a certain request timeout period, and will not return until the method has a result. If the method is executed successfully and the command is a read command, you can obtain the latest slave data by viewing the data buffer of the Modbus master.

> For specific usage, please refer to the sample code in the `/samples` directory. Debugging the Modbus master-slave program can be debugged with Modbus Poll and Modbus slave software on the PC.

### Exception handling process

Exception handling mainly occurs during the normal use of the host. All the error codes of the host request API have been described at the beginning of Chapter 3. For these error codes, users need to complete different actions according to their own product features. It is recommended that users encapsulate and implement the retransmission mechanism of the host request method. This implementation is more flexible. Generally, retransmission is required when receiving frame data errors and command response timeout error codes. The number of retransmissions is automatically increased by one. If the set value is exceeded, the slave is considered to be offline, and all subsequent commands sent to this slave are intercepted in advance; if the second retransmission command response is successful, **automatically cleared** the slave retransmits frequency. All the above functions can be realized by using the host request method or using the callback interface in `FreeModbus\port\portevent_m.c`, and users can choose flexibly according to their needs.

## API detailed

The Modbus master is very different from the slave in the use process. The slave needs to passively wait for the request of the master, while the master actively sends out the request and receives and processes the response from the slave. When the host sends a broadcast request, the slave does not need to return a response, so the broadcast request is suitable for the master's write slave data command, not suitable for the read slave data command. The return value format of all methods in the host request API is the same, and the meaning of the return value is as follows.

| Return value | Description |
| ------------------ | ------------------------------------------- |
| MB_MRE_NO_ERR | Normal, no error |
| MB_MRE_NO_REG | Register, coil or discrete input address error |
| MB_MRE_ILL_ARG | Incorrect input parameter format |
| MB_MRE_REV_DATA | Receive data error |
| MB_MRE_TIMEDOUT | Response timed out. The host did not receive the response from the slave within the set time. |
| MB_MRE_MASTER_BUSY | The host is busy. The request was not sent within the set time. |
| MB_MRE_EXE_FUN | After the host receives the response, an error occurs when executing the Modbus method (function). |

> All host request methods are **thread safe** and **blocking mode**. During use, as long as the host resource is not obtained within the set timeout period, it will return that the host is busy; if the host resource is obtained within the set timeout period, it must wait for the request result before returning.

### Write a single holding register

Write data to a holding register of the slave

```
eMBMasterReqErrCode eMBMasterReqWriteHoldingRegister( UCHAR ucSndAddr,
                                                      USHORT usRegAddr,
                                                      USHORT usRegData,
                                                      LONG lTimeOut );
```

| Parameters | Description |
| --------- | ----------------------------------------------------------- |
| ucSndAddr | Requested slave address, 0 means broadcast. |
| usRegAddr | Write register address |
| usRegData | Write register data |
| lTimeOut | Request timeout time. To support permanent waiting, just use the permanent waiting parameter of the operating system. |

### Write multiple holding registers

Write data to multiple holding registers of the slave.

```
eMBMasterReqErrCode eMBMasterReqWriteMultipleHoldingRegister( UCHAR ucSndAddr,
                                                              USHORT usRegAddr,
                                                              USHORT usNRegs,
                                                              USHORT * pusDataBuffer,
                                                              LONG lTimeOut)
```

| Parameters | Description |
| ------------- | ----------------------------------------------------------- |
| ucSndAddr | Requested slave address, 0 means broadcast. |
| usRegAddr | Start address of write register |
| usNRegs | Total number of write registers |
| pusDataBuffer | Write register data |
| lTimeOut | Request timeout time. To support permanent waiting, just use the permanent waiting parameter of the operating system. |

### Read multiple holding registers

Read data in multiple holding registers

```
eMBMasterReqErrCode eMBMasterReqReadHoldingRegister( UCHAR ucSndAddr,
                                                     USHORT usRegAddr,
                                                     USHORT usNRegs,
                                                     LONG lTimeOut );
```

| Parameters | Description |
| --------- | ----------------------------------------------------------- |
| ucSndAddr | Requested slave address, 0 means broadcast. |
| usRegAddr | Read register address |
| usRegData | Number of read registers |
| lTimeOut | Request timeout time. To support permanent waiting, just use the permanent waiting parameter of the operating system. |

### Read and write multiple holding registers

Read multiple registers first, and then write multiple registers.

```
eMBMasterReqErrCode eMBMasterReqReadWriteMultipleHoldingRegister( UCHAR ucSndAddr,
                                                                  USHORT usReadRegAddr,
                                                                  USHORT usNReadRegs,
                                                                  USHORT * pusDataBuffer,
                                                                  USHORT usWriteRegAddr,
                                                                  USHORT usNWriteRegs,
                                                                  LONG lTimeOut)
```

| Parameters | Description |
| -------------- | ----------------------------------------------------------- |
| ucSndAddr | Requested slave address, 0 means broadcast. |
| usReadRegAddr | Read register address |
| usNReadRegs | Number of read registers |
| pusDataBuffer | Write register data |
| usWriteRegAddr | Write register address |
| usNWriteRegs | Number of write registers |
| lTimeOut | Request timeout time. To support permanent waiting, just use the permanent waiting parameter of the operating system. |

### Read multiple input registers

Read data in multiple input registers

```
eMBMasterReqErrCode eMBMasterReqReadInputRegister( UCHAR ucSndAddr,
                                                   USHORT usRegAddr,
                                                   USHORT usNRegs,
                                                   LONG lTimeOut );
```

| Parameters | Description |
| --------- | ----------------------------------------------------------- |
| ucSndAddr | Requested slave address, 0 means broadcast. |
| usRegAddr | Read register address |
| usRegData | Number of read registers |
| lTimeOut | Request timeout time. To support permanent waiting, just use the permanent waiting parameter of the operating system. |

### Write a single coil

Write data to a coil of the slave

```
eMBMasterReqErrCode eMBMasterReqWriteCoil( UCHAR ucSndAddr,
                                           USHORT usCoilAddr,
                                           USHORT usCoilData,
                                           LONG lTimeOut)
```

| Parameters | Description |
| ---------- | ----------------------------------------------------------- |
| ucSndAddr | Requested slave address, 0 means broadcast. |
| usCoilAddr | Write the address of the coil |
| usCoilData | Number of write coils |
| lTimeOut | Request timeout time. To support permanent waiting, just use the permanent waiting parameter of the operating system. |

### Write multiple coils

Write data to multiple coils of the slave.

```
eMBMasterReqErrCode eMBMasterReqWriteMultipleCoils( UCHAR ucSndAddr,
                                                    USHORT usCoilAddr,
                                                    USHORT usNCoils,
                                                    UCHAR * pucDataBuffer,
                                                    LONG lTimeOut)
```

| Parameters | Description |
| ------------- | ----------------------------------------------------------- |
| ucSndAddr | Requested slave address, 0 means broadcast. |
| usCoilAddr | Write the start address of the coil |
| usNCoils | Total number of write coils |
| pucDataBuffer | Write coil data |
| lTimeOut | Request timeout time. To support permanent waiting, just use the permanent waiting parameter of the operating system. |

### Read multiple coils

Read data from multiple coils

```
eMBMasterReqErrCode eMBMasterReqReadCoils( UCHAR ucSndAddr,
                                           USHORT usCoilAddr,
                                           USHORT usNCoils,
                                           LONG lTimeOut)
```

| Parameters | Description |
| ---------- | ------------------------------------------------------------ |
| ucSndAddr | Requested slave address, 0 means broadcast. |
| usCoilAddr | Read the address of the coil |
| usNCoils | Number of reading coils |
| lTimeOut | Request timeout time. To support permanent waiting, just use the permanent waiting parameter of the operating system. |

### Read multiple discrete inputs

Read data from multiple discrete inputs

```
eMBMasterReqErrCode eMBMasterReqReadDiscreteInputs( UCHAR ucSndAddr,
                                                    USHORT usDiscreteAddr,
                                                    USHORT usNDiscreteIn,
                                                    LONG lTimeOut)
```

| Parameters | Description |
| -------------- | ------------------------------------------------------------ |
| ucSndAddr | Requested slave address, 0 means broadcast. |
| usDiscreteAddr | Read the address of discrete input |
| usNDiscreteIn | Read the number of discrete inputs |
| lTimeOut | Request timeout time. To support permanent waiting, just use the permanent waiting parameter of the operating system. |

## Precautions

- The slave supports Modbus RTU, Modbus ASCII and Modbus TCP 3 modes. The master now only supports the commonly used **Modbus RTU** mode.
- Currently the protocol stack only supports **continuous slave address**, and the starting address **starts from 1**.

## Contact information

- Maintenance: RT-Thread development team and community developers
- Homepage: <https://github.com/RT-Thread-packages/freemodbus>
