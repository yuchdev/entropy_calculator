# file_entropy.py
#
# Shannon Entropy of a file
# = minimum average number of bits per character
# required for encoding (compressing) the file
#
# So the theoretical limit (in bytes) for data compression:
# Shannon Entropy of the file * file size (in bytes) / 8
# (Assuming the file is a string of byte-size (UTF-8?) characters
# because if not then the Shannon Entropy value would be different.)
# FB - 201011291
import os
import sys
import math
import itertools
import struct


def shennon_entropy(frequency_list):
    ent = 0.0
    for freq in frequency_list:
        if freq > 0:
            ent = ent + freq * math.log(freq, 2)
    return -ent


# Shannon entropy
if __name__ == '__main__':

    if len(sys.argv) != 2:
        print("Usage: file_entropy.py [path filename]")
        sys.exit()

    file_name = sys.argv[1]

    file_size = os.stat(file_name).st_size
    print('File size in bytes: {0}'.format(file_size))
    percent_size = file_size / 100

    bytes_list = list(itertools.repeat(0, 256))
    with open(file_name, "rb") as f:
        read_byte = f.read(1)
        counter = 0
        mb_counter = 0
        while read_byte != b"":
            # Do stuff with byte
            value = struct.unpack('B', read_byte)[0]
            bytes_list[value] += 1
            counter += 1
            if counter == 1048576:
                counter = 0
                mb_counter += 1
                print('{0} MB read'.format(mb_counter))
            read_byte = f.read(1)

    print('Bytes list:')
    print(bytes_list)

    # divide by file size
    frequency_list = [float(elem)/file_size for elem in bytes_list]

    print('Frequency list:')
    print(frequency_list)

    entropy = shennon_entropy(frequency_list)
    print('Shannon entropy (min bits per byte-character): {0}'.format(entropy))
    print('Min possible file size assuming max theoretical compression efficiency:')
    print('{0} in bits'.format(entropy * file_size))
    print('{0} in bytes'.format((entropy * file_size) / 8))
