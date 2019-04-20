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


def shannon_entropy(frequency_array):
    """
    :param frequency_array: list of 255 elements, every of each is a frequency
    of a byte with that number has been met in a measured sample. For example,
    frequency_array[0] = 0.5 means 50% of bytes in the file are 0x00
    :return: entropy from 0.0 to 8.0, where 0.0 is order, 8.0 is chaos
    """
    ent = 0.0
    for freq in frequency_array:
        if freq > 0:
            ent = ent + freq * math.log(freq, 2)
    return -ent


# For printing iterations progress
class ProgressBar:
    """
    Create command-line-style ProgressBar
    """
    def __init__(self, total, prefix='', suffix='', length=100, fill='*'):
        """
        Init ProgressBar
        Start position is always 0, end position is total.
        Usage example:
        progress_bar = ProgressBar(0, l, prefix='Progress:', suffix='Complete', length=50)
        while some():
            progress_bar.iterate(i + 1)
        :param total: Required, total iterations (Int)
        :param prefix: Optional, prefix string (Str)
        :param suffix: Optional, suffix string (Str)
        :param length: Optional, character length of bar (Int)
        :param fill: Optional, bar fill character (Str)
        """
        self.counter = 0
        self.prev_counter = 0
        self.total = total
        self.percent_size = total/100
        self.prefix = prefix
        self.suffix = suffix
        self.length = length
        self.fill = fill

    def iterate(self, iteration):
        """
        Call in a loop to create terminal progress bar
        @param: iteration: Required, current iteration (Int)
        """
        self.prev_counter = int(iteration/self.percent_size)
        filled_length = int((self.prev_counter/100) * self.length)
        bar = self.fill * filled_length + '-' * (self.length - filled_length)
        if self.prev_counter != self.counter:
            print('\r%s |%s| %s%% %s' % (self.prefix, bar, self.prev_counter, self.suffix), end='\r')
        self.counter = self.prev_counter
        # Print New Line on Complete
        if iteration == self.total:
            print()


# Shannon entropy
if __name__ == '__main__':

    if len(sys.argv) != 2:
        print("Usage: file_entropy.py [path filename]")
        sys.exit()

    file_name = sys.argv[1]

    file_size = os.stat(file_name).st_size
    print('File size in bytes: {0}'.format(file_size))

    progress_bar = ProgressBar(file_size, prefix='Progress:', suffix='Complete', length=50)

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
            # Check if percentage changed not to re-print on every iteration, print() is heavy operation
            progress_bar.iterate(counter)
            read_byte = f.read(1)

    # divide by file size
    frequency_list = [float(elem)/file_size for elem in bytes_list]

    entropy = shannon_entropy(frequency_list)
    print('Shannon entropy (min bits per byte-character): {0}'.format(entropy))
    print('Min possible file size assuming max theoretical compression efficiency:')
    print('{0} in bits'.format(entropy * file_size))
    print('{0} in bytes'.format((entropy * file_size) / 8))
