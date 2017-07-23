import numpy as np
import sys

nums = np.random.random_integers(0, 2 ** 32, size=(1,10))
nums.tofile(sys.argv[1]+".bin")

sort_nums =  np.sort(nums)
sort_nums.tofile(sys.argv[1]+"_sorted.bin")
