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


# Print iterations progress
def progress_bar(iteration, total, prefix = '', suffix = '', decimals = 1, length = 100, fill = '*'):
    """
    Call in a loop to create terminal progress bar
    Usage example:
    # Set 0%
    progress_bar(0, l, prefix = 'Progress:', suffix = 'Complete', length = 50)
    while some():
        progress_bar(i + 1, l, prefix = 'Progress:', suffix = 'Complete', length = 50)

    @params:
        iteration   - Required  : current iteration (Int)
        total       - Required  : total iterations (Int)
        prefix      - Optional  : prefix string (Str)
        suffix      - Optional  : suffix string (Str)
        decimals    - Optional  : positive number of decimals in percent complete (Int)
        length      - Optional  : character length of bar (Int)
        fill        - Optional  : bar fill character (Str)
    """
    percent = ("{0:." + str(decimals) + "f}").format(100 * (iteration / float(total)))
    filledLength = int(length * iteration // total)
    bar = fill * filledLength + '-' * (length - filledLength)
    print('\r%s |%s| %s%% %s' % (prefix, bar, percent, suffix), end = '\r')
    # Print New Line on Complete
    if iteration == total: 
        print()


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
        percent_counter = 0
        while read_byte != b"":
            # Do stuff with byte
            value = struct.unpack('B', read_byte)[0]
            bytes_list[value] += 1
            counter += 1
            if counter > percent_size:
                counter = 0
                percent_counter += 1
                print('{0} % read'.format(percent_counter))
            read_byte = f.read(1)

    # divide by file size
    frequency_list = [float(elem)/file_size for elem in bytes_list]

    entropy = shennon_entropy(frequency_list)
    print('Shannon entropy (min bits per byte-character): {0}'.format(entropy))
    print('Min possible file size assuming max theoretical compression efficiency:')
    print('{0} in bits'.format(entropy * file_size))
    print('{0} in bytes'.format((entropy * file_size) / 8))
