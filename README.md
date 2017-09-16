# fastest-sorter-in-the-west
External sort i.e. sorting datasets that do not fit into main memory 
Each chunk of the dataset that can fit into main memory is individually sorted using a hybrid of quick sort & insertion sort. This is followed by an external merge sort.

# SPECS
Contents of file = 4 byte ints  
Size of file = 64GB  
Main memory on single machine = 8GB  

# TESTS
## Implemented
1. Sort single chunk in memory using fread

## Yet to be Implemented
1. Sort single chunk in memory using mmap
2. Sort all the chunks in memory using fread
3. Sort all the chunks in memory using mmap
4. Distribute chunks across 4 machines on a 10 Gbit/s network, sort it and perform external merge sort on source machine

# RESULTS
