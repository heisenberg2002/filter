int CalLargestBlock(int id_ignore) {
    int max_block_size = 0; // To store the largest block size
    int cur_size = 0;       // To track the current block size
    int previous = -1;      // To track the previous product ID

    for (int i = 0; i < N; i++) {
        if (ID[i] == id_ignore) {
            // Skip products of type id_ignore
            continue;
        }

        if (ID[i] == previous) {
            // If the current product is the same as the previous one, increment the block size
            cur_size++;
        } else {
            // Otherwise, start a new block
            cur_size = 1;
            previous = ID[i];
        }

        // Update the maximum block size if necessary
        max_block_size = max(max_block_size, cur_size);
    }

    return max_block_size;
}
