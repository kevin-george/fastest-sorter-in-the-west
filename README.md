# fastest-sorter-in-the-west
Library to sort large(~64GB) data files with limited main memory

FILES
1. singlechunk_quicksort.c - Sorts a single chunk of 8 GB using standard quick sort and fread

2. singlechunk_hybrid.c - Sorts a single chunk of 8 GB using a hybrid of quick sort & insertion sort(on a nearly sorted array). Still uses fread.

3. singlechunk_hybrid_mmap.c - Sorts a single chunk of 8 GB using the hybrid sort plus mmap instead of fread. Fastest implementation yet.
