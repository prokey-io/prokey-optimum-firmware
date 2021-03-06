#!/usr/bin/env python
#from hashlib import sha256

fn = '../bootloader/bootloader.bin'

data = open(fn, 'rb').read()
if len(data) > 65536:
    raise Exception('bootloader has to be smaller than 65536 bytes')

data += b'\x00' * (65536 - len(data))

bl_data = ', '.join('0x%02x' % x for x in bytearray(data))

with open('bl_data.h', 'wt') as f:
    #f.write('static const uint8_t bl_hash[32] = {%s};\n' % bl_hash)
    f.write('static const uint8_t bl_data[65536] = {%s};\n' % bl_data)
