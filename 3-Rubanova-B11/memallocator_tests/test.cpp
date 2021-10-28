#include "pch.h"
#include "memallocator.h"
#include "my_mem_func.h"
#include <string.h>
#define TEST_MEMORY_TEXT_BLOCK_SIZE 10

TEST(memgetminimumsize, min_size_expected20) {
	int a = memgetminimumsize();
	EXPECT_EQ(a, 20);
}

TEST(memgetblocksize, block_size_expected20) {
	int a = memgetblocksize();
	EXPECT_EQ(a, 20);
}

TEST(meminit, meminit_expectedValidVal) {
	const int TEST_MEMORY_SIZE_INIT = memgetminimumsize() + 1;
	void* ptr = malloc(TEST_MEMORY_SIZE_INIT);
	ASSERT_TRUE(ptr);
	int bytes_init = meminit(ptr, TEST_MEMORY_SIZE_INIT);
	if (!bytes_init) {
		free(ptr);
		ASSERT_TRUE(bytes_init);
	}
	int* desc = (int*)ptr;
	ASSERT_EQ(bytes_init, 1);
	ASSERT_EQ(*desc, TEST_MEMORY_SIZE_INIT);
	ASSERT_EQ(*nextOf(desc), (void*)NULL);
	ASSERT_EQ(*prevOf(desc), (void*)NULL);
	ASSERT_EQ(*isFree(desc), 1);
	ASSERT_EQ(*sizeCopy(desc), TEST_MEMORY_SIZE_INIT);
	free(ptr);
}


TEST(meminit_Test, meminit_meminitTooLittleBytes_expectZeroBytesInit) {
	const int TEST_MEMORY_SIZE_NOT_ENOUGH_TO_INIT = memgetminimumsize();
	void* ptr = malloc(TEST_MEMORY_SIZE_NOT_ENOUGH_TO_INIT);
	ASSERT_TRUE(ptr);
	int bytes_init = meminit(ptr, TEST_MEMORY_SIZE_NOT_ENOUGH_TO_INIT);
	EXPECT_EQ(bytes_init, 0);
	free(ptr);
}


TEST(memalloc_Test, memalloc_memallocAllInitMemory_expectedFlagTrue) {
	const int TEST_MEMORY_SIZE_INIT = memgetminimumsize() + TEST_MEMORY_TEXT_BLOCK_SIZE;
	void* ptr = malloc(TEST_MEMORY_SIZE_INIT);
	ASSERT_TRUE(ptr);
	int bytes_init = meminit(ptr, TEST_MEMORY_SIZE_INIT);
	if (!bytes_init) {
		free(ptr);
		ASSERT_TRUE(bytes_init);
	}
	int* desc = (int*)ptr;
	char* a = (char*)memalloc(TEST_MEMORY_TEXT_BLOCK_SIZE);
	EXPECT_EQ(bytes_init, 1);
	EXPECT_EQ(*desc, TEST_MEMORY_SIZE_INIT);
	EXPECT_EQ(*nextOf(desc), (void*)NULL);
	EXPECT_EQ(*prevOf(desc), (void*)NULL);
	EXPECT_EQ(*sizeCopy(desc), TEST_MEMORY_SIZE_INIT);
	EXPECT_EQ(*isFree(desc), 0);
	free(ptr);
}

TEST(memalloc_Test, memalloc_memallocAllInitMemoryWriteSmth_expectBlockInfoNotCorrupted) {
	const int TEST_MEMORY_SIZE_INIT = memgetminimumsize() + TEST_MEMORY_TEXT_BLOCK_SIZE;
	const char sometext[TEST_MEMORY_TEXT_BLOCK_SIZE] = "abccbbfff";
	void* ptr = malloc(TEST_MEMORY_SIZE_INIT);
	ASSERT_TRUE(ptr);
	int bytes_init = meminit(ptr, TEST_MEMORY_SIZE_INIT);
	if (!bytes_init) {
		free(ptr);
		ASSERT_TRUE(bytes_init);
	}
	int* desc = (int*)ptr;
	char* a = (char*)memalloc(TEST_MEMORY_TEXT_BLOCK_SIZE);
	// copying smth to *a not more than TEST_MEMORY_TEXT_BLOCK_SIZE so if smth goes wrong block info will be damaged
	strcpy(a, sometext);
	EXPECT_EQ(bytes_init, 1);
	EXPECT_EQ(*desc, TEST_MEMORY_SIZE_INIT);
	EXPECT_EQ(*nextOf(desc), (void*)NULL);
	EXPECT_EQ(*prevOf(desc), (void*)NULL);
	EXPECT_EQ(*isFree(desc), 0);
	EXPECT_EQ(*sizeCopy(desc), TEST_MEMORY_SIZE_INIT);
	EXPECT_TRUE(!strcmp(a, sometext));
	free(ptr);
}


TEST(memalloc_Test, memalloc_memallocAllInitMemoryNextMemallocFail_expectSecondMemallocReturnNull) {
	const int TEST_MEMORY_SIZE_INIT = memgetminimumsize() + TEST_MEMORY_TEXT_BLOCK_SIZE;
	void* ptr = malloc(TEST_MEMORY_SIZE_INIT);
	ASSERT_TRUE(ptr);
	int bytes_init = meminit(ptr, TEST_MEMORY_SIZE_INIT);
	if (!bytes_init) {
		free(ptr);
		ASSERT_TRUE(bytes_init);
	}
	int* desc = (int*)ptr;
	char* a = (char*)memalloc(TEST_MEMORY_TEXT_BLOCK_SIZE);
	char* b = (char*)memalloc(TEST_MEMORY_TEXT_BLOCK_SIZE);
	EXPECT_EQ(b, (void*)NULL);
	EXPECT_EQ(bytes_init, 1);
	EXPECT_EQ(*desc, TEST_MEMORY_SIZE_INIT);
	EXPECT_EQ(*nextOf(desc), (void*)NULL);
	EXPECT_EQ(*prevOf(desc), (void*)NULL);
	EXPECT_EQ(*isFree(desc), 0);
	EXPECT_EQ(*sizeCopy(desc), TEST_MEMORY_SIZE_INIT);
	free(ptr);
}

TEST(memalloc_Test, memalloc_memallocInitTextBlockAndCharBlock_expectMemoryAllocatedCorrect) {
	const int TEST_MEMORY_SIZE_INIT = memgetminimumsize() + TEST_MEMORY_TEXT_BLOCK_SIZE + memgetblocksize() + sizeof(char);
	void* ptr = malloc(TEST_MEMORY_SIZE_INIT);
	ASSERT_TRUE(ptr);
	int bytes_init = meminit(ptr, TEST_MEMORY_SIZE_INIT);
	if (!bytes_init) {
		free(ptr);
		ASSERT_TRUE(bytes_init);
	}
	char* a_text = (char*)memalloc(TEST_MEMORY_TEXT_BLOCK_SIZE);
	char* b_char = (char*)memalloc(sizeof(char));
	int* desc_a = (int*)((char*)a_text - 2 * sizeof(int) - 2 * sizeof(int*));
	int* desc_b = (int*)((char*)b_char - 2 * sizeof(int) - 2 * sizeof(int*));
	EXPECT_EQ(bytes_init, 1);
	EXPECT_EQ(*desc_a, TEST_MEMORY_TEXT_BLOCK_SIZE + memgetblocksize());
	EXPECT_EQ(*sizeCopy(desc_a), TEST_MEMORY_TEXT_BLOCK_SIZE + memgetblocksize());
	EXPECT_EQ(*nextOf(desc_a), (void*)NULL);
	EXPECT_EQ(*prevOf(desc_a), (void*)NULL);
	EXPECT_EQ(*isFree(desc_a), 0);
	EXPECT_EQ(*desc_a, TEST_MEMORY_TEXT_BLOCK_SIZE + memgetblocksize());
	EXPECT_EQ(*desc_b, (int)((char*)sizeof(char) + memgetblocksize()));
	EXPECT_EQ(*nextOf(desc_b), (void*)NULL);
	EXPECT_EQ(*prevOf(desc_b), (void*)NULL);
	EXPECT_EQ(*isFree(desc_b), 0);
	EXPECT_EQ(*sizeCopy(desc_b), (int)((char*)sizeof(char) + memgetblocksize()));
	free(ptr);
}

TEST(memalloc_Test, memalloc_memallocInitTextBlockAndCharBlock_expectNoBlocksCorrupted) {
	const int TEST_MEMORY_SIZE_INIT = memgetminimumsize() + TEST_MEMORY_TEXT_BLOCK_SIZE + memgetblocksize() + sizeof(char);
	const char sometext[TEST_MEMORY_TEXT_BLOCK_SIZE] = "abccbbfff";
	const char b = 'b';
	void* ptr = malloc(TEST_MEMORY_SIZE_INIT);
	ASSERT_TRUE(ptr);
	int bytes_init = meminit(ptr, TEST_MEMORY_SIZE_INIT);
	if (!bytes_init) {
		free(ptr);
		ASSERT_TRUE(bytes_init);
	}
	char* a_text = (char*)memalloc(TEST_MEMORY_TEXT_BLOCK_SIZE);
	char* b_char = (char*)memalloc(sizeof(char));
	int* desc_a = (int*)((char*)a_text - 2 * sizeof(int) - 2 * sizeof(void*));
	int* desc_b = (int*)((char*)b_char - 2 * sizeof(int) - 2 * sizeof(void*));
	strcpy(a_text, sometext);
	*b_char = b;
	EXPECT_EQ(bytes_init, 1);
	EXPECT_EQ(*desc_a, TEST_MEMORY_TEXT_BLOCK_SIZE + memgetblocksize());
	EXPECT_EQ(*nextOf(desc_a), (void*)NULL);
	EXPECT_EQ(*prevOf(desc_a), (void*)NULL);
	EXPECT_EQ(*isFree(desc_a), 0);
	EXPECT_EQ(*sizeCopy(desc_a), TEST_MEMORY_TEXT_BLOCK_SIZE + memgetblocksize());
	EXPECT_EQ(*desc_b, (int)sizeof(char) + memgetblocksize());
	EXPECT_EQ(*nextOf(desc_b), (void*)NULL);
	EXPECT_EQ(*prevOf(desc_b), (void*)NULL);
	EXPECT_EQ(*isFree(desc_b), 0);
	EXPECT_EQ(*sizeCopy(desc_b), (int)sizeof(char) + memgetblocksize());
	EXPECT_TRUE(!strcmp(a_text, sometext));
	EXPECT_EQ(*b_char, b);
	free(ptr);
}

TEST(memfree_Test, memfree_memfreeFreeOneBlock_expectSizeIsPositive) {
	const int TEST_MEMORY_SIZE_INIT = memgetminimumsize() + sizeof(char);
	void* ptr = malloc(TEST_MEMORY_SIZE_INIT);
	ASSERT_TRUE(ptr);
	int bytes_init = meminit(ptr, TEST_MEMORY_SIZE_INIT);
	if (!bytes_init) {
		free(ptr);
		ASSERT_TRUE(bytes_init);
	}
	// making all init memory allocated block
	int* all_init_memory_block_desc = (int*)ptr;
	*all_init_memory_block_desc = *all_init_memory_block_desc;
	*nextOf(all_init_memory_block_desc) = NULL;
	*prevOf(all_init_memory_block_desc) = NULL;
	*isFree(all_init_memory_block_desc) = 0;
	*sizeCopy(all_init_memory_block_desc) = *sizeCopy(all_init_memory_block_desc);
	list.ptr = NULL;
	// freeing this block
	void* user_ptr_to_block = (void*)((char*)all_init_memory_block_desc + 2 * sizeof(int) + 2 * sizeof(int*));
	memfree(user_ptr_to_block);
	EXPECT_EQ(*isFree(all_init_memory_block_desc), 1);
	EXPECT_EQ(list.ptr, ptr);
	free(ptr);
}

TEST(memfree_Test, memfree_memfreeFreeBlockWhereRightBlockIsFree_expectBlocksMerged) {
	const int TEST_MEMORY_SIZE_INIT = memgetminimumsize() + memgetblocksize() + 2 * sizeof(char);
	void* ptr = malloc(TEST_MEMORY_SIZE_INIT);
	ASSERT_TRUE(ptr);
	int bytes_init = meminit(ptr, TEST_MEMORY_SIZE_INIT);
	if (!bytes_init) {
		free(ptr);
		ASSERT_TRUE(bytes_init);
	}
	// first block is allocated
	int* allocated_char_block_desc = (int*)ptr;
	*allocated_char_block_desc = (int)(sizeof(char) + memgetblocksize());
	*nextOf(allocated_char_block_desc) = NULL;
	*prevOf(allocated_char_block_desc) = NULL;
	*isFree(allocated_char_block_desc) = 0;
	*sizeCopy(allocated_char_block_desc) = *allocated_char_block_desc;
	// second block is free
	int* free_char_block_desc = (int*)((char*)allocated_char_block_desc + *allocated_char_block_desc);
	list.ptr = free_char_block_desc;
	*free_char_block_desc = (int)(sizeof(char) + memgetblocksize());
	*nextOf(free_char_block_desc) = NULL;
	*prevOf(free_char_block_desc) = NULL;
	*isFree(free_char_block_desc) = 1;
	*sizeCopy(free_char_block_desc) = *free_char_block_desc;
	// now freeing and expect memory state similar to state after meminit
	int* user_ptr_to_allocated_block = (int*)((char*)allocated_char_block_desc + 2 * sizeof(int) + 2 * sizeof(int*));
	memfree(user_ptr_to_allocated_block);
	EXPECT_EQ(*allocated_char_block_desc, list.size);
	EXPECT_EQ(*sizeCopy(allocated_char_block_desc), list.size);
	EXPECT_EQ(list.ptr, ptr);
	free(ptr);
}

TEST(memfree_Test, memfree_memfreeFreeBlockWhereLeftBlockIsFree_expectBlocksMerged) {
	const int TEST_MEMORY_SIZE_INIT = memgetminimumsize() + memgetblocksize() + 2 * sizeof(char);
	void* ptr = malloc(TEST_MEMORY_SIZE_INIT);
	ASSERT_TRUE(ptr);
	int bytes_init = meminit(ptr, TEST_MEMORY_SIZE_INIT);
	if (!bytes_init) {
		free(ptr);
		ASSERT_TRUE(bytes_init);
	}
	// first block is free
	int* free_char_block_desc = (int*)ptr;
	list.ptr = free_char_block_desc;
	*free_char_block_desc = (int)sizeof(char) + memgetblocksize();
	*nextOf(free_char_block_desc) = NULL;
	*prevOf(free_char_block_desc) = NULL;
	*isFree(free_char_block_desc) = 1;
	*sizeCopy(free_char_block_desc) = *free_char_block_desc;
	// second block is allocated
	int* allocated_char_block_desc = (int*)((char*)free_char_block_desc + *free_char_block_desc);
	*allocated_char_block_desc = (int)sizeof(char) + memgetblocksize(); // block is allocated so size is negative
	*nextOf(allocated_char_block_desc) = NULL;
	*prevOf(allocated_char_block_desc) = NULL;
	*isFree(allocated_char_block_desc) = 0;
	*sizeCopy(allocated_char_block_desc) = *allocated_char_block_desc;
	// now freeing and expect memory state similar with state after meminit
	void* user_ptr_to_allocated_block = (void*)((char*)allocated_char_block_desc + 2 * sizeof(int) + 2 * sizeof(int*));
	memfree(user_ptr_to_allocated_block);
	EXPECT_EQ(*free_char_block_desc, list.size);
	EXPECT_EQ(*sizeCopy(free_char_block_desc), list.size);
	EXPECT_EQ(list.ptr, ptr);
	free(ptr);
}

TEST(memfree_Test, memfree_memfreeFreeBlockBetweenTwoFreeBlocks_expectBlocksMerged) {
	const int TEST_MEMORY_SIZE_INIT = memgetminimumsize() + 2 * memgetblocksize() + 3 * sizeof(char);
	void* ptr = malloc(TEST_MEMORY_SIZE_INIT);
	ASSERT_TRUE(ptr);
	int bytes_init = meminit(ptr, TEST_MEMORY_SIZE_INIT);
	if (!bytes_init) {
		free(ptr);
		ASSERT_TRUE(bytes_init);
	}
	// first block is free
	int* first_free_char_block_desc = (int*)ptr;
	list.ptr = first_free_char_block_desc;
	*first_free_char_block_desc = (int)sizeof(char) + memgetblocksize();
	*nextOf(first_free_char_block_desc) = NULL;
	*prevOf(first_free_char_block_desc) = NULL;
	*isFree(first_free_char_block_desc) = 1;
	*sizeCopy(first_free_char_block_desc) = *first_free_char_block_desc;
	// second block is allocated
	int* allocated_char_block_desc = (int*)((char*)first_free_char_block_desc + *first_free_char_block_desc);
	*allocated_char_block_desc = (int)sizeof(char) + memgetblocksize();
	*nextOf(allocated_char_block_desc) = NULL;
	*prevOf(allocated_char_block_desc) = NULL;
	*isFree(allocated_char_block_desc) = 0;
	*sizeCopy(allocated_char_block_desc) = *allocated_char_block_desc;
	// third block is free
	int* third_free_char_block_desc = (int*)((char*)allocated_char_block_desc + *allocated_char_block_desc);
	*third_free_char_block_desc = (int)sizeof(char) + memgetblocksize();
	*nextOf(third_free_char_block_desc) = NULL;
	*prevOf(third_free_char_block_desc) = first_free_char_block_desc;
	*isFree(third_free_char_block_desc) = 1;
	*sizeCopy(third_free_char_block_desc) = *third_free_char_block_desc;
	*nextOf(first_free_char_block_desc) = third_free_char_block_desc;
	// now freeing and expect memory state similar with state after meminit
	void* user_ptr_to_allocated_block = (void*)((char*)allocated_char_block_desc + 2 * sizeof(int) + 2 * sizeof(int*));
	memfree(user_ptr_to_allocated_block);
	EXPECT_EQ(*first_free_char_block_desc, list.size);
	EXPECT_EQ(*sizeCopy(first_free_char_block_desc), list.size);
	EXPECT_EQ(*nextOf(first_free_char_block_desc), (void*)NULL);
	EXPECT_EQ(*prevOf(first_free_char_block_desc), (void*)NULL);
	EXPECT_EQ(list.ptr, ptr);
	free(ptr);
}

TEST(memalloc_FuncTest, memalloc_listptrSizeLessThenAskedToMalloc_expectRightBlock) {
	const int TEST_MEMORY_SIZE_INIT = 3 * memgetblocksize() + 1 + 5 + 5;
	void* ptr = malloc(TEST_MEMORY_SIZE_INIT);
	ASSERT_TRUE(ptr);
	int bytes_init = meminit(ptr, TEST_MEMORY_SIZE_INIT);
	if (!bytes_init) {
		free(ptr);
		ASSERT_TRUE(bytes_init);
	}
	char* a = (char*)memalloc(5);
	char* b = (char*)memalloc(5);
	char* c = (char*)memalloc(1);
	EXPECT_TRUE(a);
	EXPECT_TRUE(b);
	EXPECT_TRUE(c);
	memfree(a);
	memfree(c);
	char* d = (char*)memalloc(5); // here we could get 1-byte block with memalloc error
	c = (char*)memalloc(1);
	EXPECT_TRUE(d);
	EXPECT_TRUE(c);
	EXPECT_TRUE(d == a);
	memfree(d);
	memfree(b);
	memfree(c);
	memdone();
	free(ptr);
}


#define TEST_BLOCKS_COUNT 10000

TEST(memallocator_StressTest, memalloc_manyBlocksAllocAndFree_expectMemoryStateSimilarToAfterInit) {
	const int TEST_BLOCK_SIZE = 16;
	const int TEST_MEMORY_SIZE = TEST_BLOCKS_COUNT * TEST_BLOCK_SIZE;
	const int TEST_MEMORY_SIZE_INIT = TEST_BLOCKS_COUNT * (TEST_BLOCK_SIZE + memgetblocksize());
	void* ptr = malloc(TEST_MEMORY_SIZE_INIT);
	ASSERT_TRUE(ptr);
	int bytes_init = meminit(ptr, TEST_MEMORY_SIZE_INIT);
	if (!bytes_init) {
		free(ptr);
		ASSERT_TRUE(bytes_init);
	}
	void** blocks = (void**)malloc(TEST_BLOCKS_COUNT * sizeof(void*));
	if (!blocks) {
		free(ptr);
		ASSERT_TRUE(blocks);
	}
	for (int i = 0; i < TEST_BLOCKS_COUNT; i++) {
		blocks[i] = (char*)memalloc(TEST_BLOCK_SIZE);
	}
	for (int i = 0; 2 * i < TEST_BLOCKS_COUNT; i++) {
		memfree(blocks[2 * i]);
	}
	for (int i = 0; 2 * i < TEST_BLOCKS_COUNT; i++) {
		blocks[2 * i] = (char*)memalloc(TEST_BLOCK_SIZE);
	}
	for (int i = 0; 2 * i + 1 < TEST_BLOCKS_COUNT; i++) {
		memfree(blocks[2 * i + 1]);
	}
	for (int i = 0; 2 * i < TEST_BLOCKS_COUNT; i++) {
		memfree(blocks[2 * i]);
	}
	EXPECT_EQ(*((int*)ptr), list.size);
	EXPECT_EQ(*sizeCopy((int*)ptr), list.size);
	EXPECT_EQ(*nextOf((int*)ptr), (void*)NULL);
	EXPECT_EQ(*prevOf((int*)ptr), (void*)NULL);
	EXPECT_EQ(*isFree((int*)ptr), 1);
	memdone();
	free(blocks);
	free(ptr);
}

TEST(memallocator_StressTest, memalloc_manyRandomBlocksAllocAndFree_expectMemoryStateSimilarToAfterInit) {
	const int TEST_BLOCK_SIZE = 16;
	const int TEST_MEMORY_SIZE = TEST_BLOCKS_COUNT * TEST_BLOCK_SIZE;
	const int TEST_MEMORY_SIZE_INIT = 2 * TEST_BLOCKS_COUNT * (TEST_BLOCK_SIZE + memgetblocksize()); //multiplied by 2 so fragmentation will not fail the test
	void* ptr = malloc(TEST_MEMORY_SIZE_INIT);
	ASSERT_TRUE(ptr);
	int bytes_init = meminit(ptr, TEST_MEMORY_SIZE_INIT);
	if (!bytes_init) {
		free(ptr);
		ASSERT_TRUE(bytes_init);
	}
	void** blocks = (void**)malloc(TEST_BLOCKS_COUNT * sizeof(void*));
	if (!blocks) {
		free(ptr);
		ASSERT_TRUE(blocks);
	}
	for (int i = 0; i < TEST_BLOCKS_COUNT; i++) {
		blocks[i] = (char*)memalloc(rand() % TEST_BLOCK_SIZE + 1);
	}
	for (int i = 0; 2 * i < TEST_BLOCKS_COUNT; i++) {
		memfree(blocks[2 * i]);
	}
	for (int i = 0; 2 * i < TEST_BLOCKS_COUNT; i++) {
		blocks[2 * i] = (char*)memalloc(TEST_BLOCK_SIZE);
	}
	for (int i = 0; 2 * i + 1 < TEST_BLOCKS_COUNT; i++) {
		memfree(blocks[2 * i + 1]);
	}
	for (int i = 0; 2 * i < TEST_BLOCKS_COUNT; i++) {
		memfree(blocks[2 * i]);
	}
	EXPECT_EQ(*((int*)ptr), list.size);
	EXPECT_EQ(*sizeCopy((int*)ptr), list.size);
	EXPECT_EQ(*nextOf((int*)ptr), (void*)NULL);
	EXPECT_EQ(*prevOf((int*)ptr), (void*)NULL);
	EXPECT_EQ(*isFree((int*)ptr), 1);
	memdone();
	free(blocks);
	free(ptr);
}
