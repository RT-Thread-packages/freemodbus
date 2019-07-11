Import('RTT_ROOT')
from building import *

src = Split("""
modbus/functions/mbfuncdiag.c
modbus/functions/mbutils.c
modbus/functions/mbfuncother.c
modbus/rtu/mbcrc.c
port/port.c
""")

master_rtu_src = Split("""
modbus/functions/mbfunccoils_m.c
modbus/functions/mbfuncdisc_m.c
modbus/functions/mbfuncholding_m.c
modbus/functions/mbfuncinput_m.c
modbus/rtu/mbrtu_m.c
modbus/mb_m.c
port/portevent_m.c
port/portserial_m.c
port/porttimer_m.c
port/user_mb_app_m.c
""")

slave_src = Split("""
modbus/functions/mbfunccoils.c
modbus/functions/mbfuncdisc.c
modbus/functions/mbfuncholding.c
modbus/functions/mbfuncinput.c
modbus/mb.c
port/portevent.c
port/portserial.c
port/porttcp.c
port/porttimer.c
port/user_mb_app.c
""")

# The set of source files associated with this SConscript file.
path = [GetCurrentDir() + '/modbus/include',
    GetCurrentDir() + '/modbus/rtu',
    GetCurrentDir() + '/modbus/ascii',
    GetCurrentDir() + '/modbus/tcp',
    GetCurrentDir() + '/port']

if GetDepend(['PKG_MODBUS_MASTER_RTU']):
    src += master_rtu_src

if GetDepend(['PKG_MODBUS_SLAVE']):
    src += slave_src

if GetDepend(['PKG_MODBUS_SLAVE_RTU']):
    src += ['modbus/rtu/mbrtu.c']

if GetDepend(['PKG_MODBUS_SLAVE_ASCII']):
    src += ['modbus/ascii/mbascii.c']
    src += ['modbus/rtu/mbrtu.c']

if GetDepend(['PKG_MODBUS_SLAVE_TCP']):
    src += ['modbus/tcp/mbtcp.c']

if GetDepend(['PKG_MODBUS_MASTER_SAMPLE']):
    src += ['samples/sample_mb_master.c']

if GetDepend(['PKG_MODBUS_SLAVE_SAMPLE']):
    src += ['samples/sample_mb_slave.c']

group = DefineGroup('FreeModbus', src, depend = ['PKG_USING_FREEMODBUS'], CPPPATH = path)

Return('group')
