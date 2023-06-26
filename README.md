# Join Algorithms

The join is a fundamental operation in relational data processing that finds matching rows between two tables. In this project, you will implement, test, and benchmark a disk-based join algorithm. Your goal is to efficiently use memory and disk resources to return the answer to the following query.

```sql
SELECT R.b, S.b
FROM R, S
WHERE R.a = S.a;
```

## Algorithms

You will implement disk-based join algorithms: block nested loop join (BNLJ). Sketch of the algorithm is provided below.

### Sketches

#### Block nested loop join (BNLJ)

- For each block $R_i$ of $B - 2$ pages in $R$:
	- For each tuple $s$ in $S$:
		- For each tuple $r$ in $R_i$:
			- If $r.a = s.a$ then output $(r.b, s.b)$.

### Modifications

To an extent, you are free to make minor modifications to the algorithm to improve performance. However, your algorithm must be clearly recognizable as a variant of the original. For BNLJ, your algorithm must compute the join with a block nested loop structure.

## Implementation

You will implement the following function in `src/join.cpp`.

```cpp
int join(File &file,
         int numPagesR,
         int numPagesS,
         char *buffer,
         int numFrames);
```

The function parameters are explained below.

- **`file`**: The database file. Refer to the documentation of the `File` class for more details.
- **`numPagesR`**: The number of pages in input table `R`.
- **`numPagesS`**: The number of pages in input table `S`.
- **`buffer`**: A contiguous memory region of buffer frames.
- **`numFrames`**: The number of frames in the buffer.

The function must return the number of tuples in the result.

### Input

Your implementation will be run on randomly-generated databases. Each database will consist of two tables, `R` and `S`, each with two integer columns, `a` and `b`. You will evaluate the SQL query above.

Given $P_R$ pages in `R`, $P_S$ pages in `S`, and $B$ buffer frames, you may make the following assumptions about the input to your algorithm.

- The page size is 4096 bytes. The frame size is the same as the page size.
- $0 < P_R \le P_S$. The file consists of $P_R$ pages of table `R`, followed immediately by $P_S$ pages of table `S`, followed immediately by $P_R$ of empty output pages.
- $B \ge 2 + \sqrt{P_R + P_S}$. The number of frames in the buffer is large enough to achieve a two-pass algorithm for SMJ and HJ, given a reasonable hash function. The buffer is a contiguous memory region of $4096B$ bytes.
- The tuple size is 8 bytes. The first 4 bytes represent the value of column `a`. The last 4 bytes represent the value of column `b`. Values should be interpreted as unsigned 32-bit integers. There are 512 tuples per page. Each input table page is completely full. There are no empty slots.
- `R.a`, `S.a` $> 0$. There are no duplicate values in `R.a`. There are no duplicate values in `S.a`.


### Output

You should write your output to the pages that immediately follow table `S`. Your output table should be packed. If there is a page in the output table with fewer than 512 tuples, it should be the last page in the table. You may write the tuples in any order.

Any remaining pages in the file after your output table can be created as needed by your algorithm, and they will be ignored by the caller. You may not create additional files or perform I/O outside of using the `File` class.

### Constraints

#### I/O cost

We will measure the I/O cost of your implementation. Given $P_R$ pages in `R`, $P_S$ pages in `S`, and $B$ buffer frames, the number of I/Os your implementation incurs must not exceed the following limits. 

| Algorithm | Maximum number of reads                                | Maximum number of writes |
| --------- | ------------------------------------------------------ | ------------------------ |
| BNLJ      | $P_R + P_S \left\lceil \frac{P_R}{B - 2} \right\rceil$ | $P_R$                    |
| SMJ       | $2(P_R + P_S)$                                         | $2P_R + P_S$             |
| HJ        | $2(P_R + P_S)$                                         | $2P_R + P_S$             |

#### Memory usage

We will also measure the peak heap memory usage of your implementation. You may use only the provided buffer to read in table data, and you should aim to allocate minimal heap memory beyond the provided buffer. However, you will likely need some additional memory for data structures and bookkeeping. We will allow an additional 100 KiB plus 1 KiB for each page in the buffer. Thus, if your peak heap memory usage is less than or equal to $2^{10}(100 + B)$ bytes (not including the size of the buffer), you will receive full credit for this portion. In practice, this requirement should be easily satisfied. Instructions for measuring peak heap memory usage are below.

## Building

Ensure you have the dependencies installed. To build the project, you only need CMake version 3.4 or newer and a C++ 17 enabled compiler. You can verify your CMake installation by running:

```bash
cmake --version
```

If CMake is installed on your system, you'll see output similar to `cmake version 3.x.x`. If you don't have CMake, follow these [installation instructions](https://cmake.org/install).

Clone the repository to your computer. If you're unfamiliar with GitHub, you can follow this [quick tutorial](https://docs.github.com/en/get-started/quickstart). Replace `URL` below with the GitHub URL of the repository.

```bash
git clone URL
```

Navigate into the project directory.

```bash
cd Join-Algorithms
```

Create and navigate into a build directory.

```bash
mkdir build && cd build
```

Generate the build files.

```bash
cmake ..
```

Finally, build the project.

```bash
cmake --build .
```

Optionally, run the tests. Some tests will fail if you have just started the project.

```bash
ctest .
```

To measure peak heap memory usage, run the provided script. This will only work on a Linux machine with Valgrind installed. We recommend using one of the CSL Linux machines.

```bash
./test_memory.sh
```

## Developing

You may not create, move, or delete any files or directories, with the exception of the build directory. The only files you may modify are `src/join.cpp` and `src/test/test_join.cpp`.

### Tests

Tests are located in the subdirectory `src/test`. Tests for correctness and I/O cost of your implementation can be found in `src/test/test_join.cpp`. You are free to add any additional tests to this file; however, it will be overwritten with the original when we grade your submission. Passing these tests will account for the majority of your project grade. You need not worry about `src/test/test_file.cpp`.

In the performance evaluation database, `R` and `S` will have 100,000 pages each. The buffer will have 1000 frames.
