#include "join.hpp"

#include <array>
#include <cstdint>
#include <vector>
#include <iostream>

JoinAlgorithm getJoinAlgorithm() {
  // TODO: Return your chosen algorithm.
  return JOIN_ALGORITHM_BNLJ;
  // return JOIN_ALGORITHM_SMJ;
  // return JOIN_ALGORITHM_HJ;
  throw std::runtime_error("not implemented: getJoinAlgorithm");
};

int join(File &file, int numPagesR, int numPagesS, char *buffer,
         int numFrames) {
  // TODO: Implement your chosen algorithm.
  // Currently, this function performs a nested loop join in memory. While it
  // will pass the correctness and I/O cost tests, it ignores the provided
  // buffer and instead reads table data into vectors. It will not satisfy the
  // constraints on peak heap memory usage for large input tables. You should
  // delete this code and replace it with your disk-based join algorithm
  // implementation.

  int pageIndexR = 0;
  int pageIndexS = pageIndexR + numPagesR;
  int sIdx = 1;
  int pageIndexOut = pageIndexS + numPagesS;
  // initialize the variable which will store how many tuples matched.
  int count = 0;

  // determine the block size
  int blockSize = numFrames - 2;
  bool maxReached = false;

  // tuples that satisfy the condition are put here

  std::vector<Tuple> tuplesOut; //TODO: Add back

  // Loop 1: Loop over the blocks of R and put them in buffer
  // a block consists of multiple pages as determined by the blockSize
  // page size = 4096 bytes and there are 512 tuples per page

  for (int rBlock = 0; rBlock < numPagesR; rBlock += blockSize) {

    // end of this block or the end of the page
    int rBlockEnd = std::min(rBlock + blockSize, numPagesR);
    if (rBlockEnd == numPagesR) {
        maxReached = true;
    }
    int currBlockSize = rBlockEnd - rBlock;
    // read the tuples in that block from the file and place into the buffer
    file.read(buffer, rBlock, currBlockSize);

      for (int sPage = 0; sPage < numPagesS; sPage += 1) {
        file.read(buffer + (currBlockSize * 4096), pageIndexS + sPage, 1);

        for (int sTup = 0; sTup < 512; sTup += 1) {
            Tuple sAA = *reinterpret_cast<Tuple *>(buffer + (currBlockSize * 4096) + (sTup * sizeof(uint32_t) * 2));

            for (int rTuple = 0; rTuple < currBlockSize * 512; rTuple += 1) {   //4096
                Tuple rA = *reinterpret_cast<Tuple *>(buffer + (rTuple * sizeof(uint32_t) * 2));
                if (rA.first == sAA.first) {
                    tuplesOut.emplace_back(rA.second, sAA.second);
                    count++;
                    if (tuplesOut.size() == 512) {
                        file.write(tuplesOut.data(), pageIndexOut, 1);
                        pageIndexOut++;
                        tuplesOut.clear();
                    }
                }
            }
        }
      }

      if (maxReached) {
          break;
      }
  }

    int numTuplesOut = tuplesOut.size();
    int numPagesOut = numTuplesOut / 512 + (numTuplesOut % 512 != 0);
    file.write(tuplesOut.data(), pageIndexOut, numPagesOut);
//  int numTuplesOut = count;
//  int numPagesOut = numTuplesOut / 512 + (numTuplesOut % 512 != 0);
//  file.write(tuplesOut.data(), pageIndexOut, numPagesOut);

  return count;
}
