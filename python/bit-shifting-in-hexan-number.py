'''
just a little bit fun with bit shifting
'''

>>> n = 0x12345678

>>> for i in range(1, 8 + 1): hex(n << i*4) # shift left by digit-order in a hexan number
'0x123456780'
'0x1234567800'
'0x12345678000'
'0x123456780000'
'0x1234567800000'
'0x12345678000000'
'0x123456780000000'
'0x1234567800000000'

>>> for i in range(1, 8 + 1): hex(n >> i*4) # shift right by dibit-order in a hexan number
'0x1234567'
'0x123456'
'0x12345'
'0x1234'
'0x123'
'0x12'
'0x1'
'0x0'

>>> hex(n >> 2*4) # remove 2 last digits in a hexan number
'0x123456'

>>> hex(n >> 6*4) # extract 2 first digits in a hexan number
'0x12'

>>> hex(n >> 4*4 & 0x00FF) # extract 2 middle digits in a hexan number
'0x34'