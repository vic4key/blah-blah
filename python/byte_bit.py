# set a bit
>>> format(0b0000_0000 | (1 << 3), "08b")
'00001000'

# clear a bit
 >>> format(0b1111_1111 & ~(1 << 3), "08b")
'11110111'

# toggle a bit
>>> format(0b1111_1111 ^ (1 << 3), "08b")
'11110111'

# toggle bits
>>> mask = (1 << 1 | 1 << 3 | 1 << 5 | 1 << 7)
>>> format(0b1111_1111 ^ mask, "08b")
'01010101'

# check a bit is set or not
>>> (0b000_1000 & (1 << 3)) != 0
True

# check any bit is set or not (at least one)
>>> mask = (1 << 1 | 1 << 3 | 1 << 5 | 1 << 7)
>>> (0b0000_1000 & mask) != 0
True

# check bits are all set or not
>>> mask = (1 << 1 | 1 << 3 | 1 << 5 | 1 << 7)
>>> (0b1111_1111 & mask) == mask
True

# find first bit set
>>> n = 0b0101_0000
>>> (n & -n).bit_length() - 1
4

# find last bit set
>>> n = 0b0101_0000
>>> n.bit_length() - 1
6

# count number of bits are set
>>> n = 0b0101_0000
>>> bin(n).count('1')
2
>>> n.bit_count()
2


-----


# remove last byte(s)
>>> hex(0x12345678 >> 4*2)
'0x123456'

# add more last byte(s)
 >>> hex((n << 4*2) | 0xab)
'0x12345678ab'

# clear byte(s)
>>> mask = 0x00ff0000
>>> hex((0x12345678 & ~mask))
'0x12005678'

# replace by another byte(s)
>>> mask = 0x00ff0000
>>> hex((0x12345678 & ~mask) | 0x00ab0000)
'0x12ab5678'