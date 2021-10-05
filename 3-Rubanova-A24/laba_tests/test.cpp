#include "pch.h"
#include "laba_header.h"

class TestMemory : public ::testing::Test {
protected:
	_CrtMemState s1, s2, s3;

	void SetUp() {
		_CrtMemCheckpoint(&s1);
	}

	void TearDown() {
		_CrtMemCheckpoint(&s2);
		if (_CrtMemDifference(&s3, &s1, &s2)) {
			_CrtMemDumpStatistics(&s3);
			FAIL();
		}
	}
};

class StringToStructureMemory : public TestMemory {};


TEST(charToInt, char7toInt7_expectedInt7) {
	char a = '7';
	int b = charToInt(a);
	EXPECT_EQ(b, 7);
}

TEST(charToInt, charAtoIntA_expectedNegative1) {
	char a = 'A';
	int b = charToInt(a);
	EXPECT_EQ(b, -1);
}

TEST(makeNumOutOfDigits, char1234toInt1234_expectedInt1234) {
	char* a = "1234";
	file_line element = { "2020-12-09", "Ivanov", "Sasha" };
	makeNumOutOfDigits(&element, a);
	EXPECT_TRUE(element.hours == 1234);
}

TEST(makeNumOutOfDigits, char1toInt1_expectedInt1) {
	char* a = "1";
	file_line element = { "2020-12-09", "Ivanov", "Sasha" };
	makeNumOutOfDigits(&element, a);
	EXPECT_TRUE(element.hours == 1);
}

TEST_F(StringToStructureMemory, LineToStructure_ExpectedValidVal) {
	char* a = "2021-09-10,Ivanov,Ivan,8\n";
	file_line head = { "2020-12-09", "Ivanov", "Sasha", 8, NULL};
	file_line element = stringToStructure(a, &head);
	EXPECT_STREQ(element.date, "2021-09-10");
	EXPECT_STREQ(element.surname, "Ivanov");
	EXPECT_STREQ(element.name, "Ivan");
	EXPECT_EQ(element.hours, 8);
	free(element.date);
	free(element.surname);
	free(element.name);
}

TEST(compareStrings, equalStrings_expected1) {
	int a = compareStrings("Ivanova", "Ivanova");
	EXPECT_TRUE(a == 1);
}

TEST(compareStrings, aBeforeB_expected2) {
	char* A = "Abc";
	char* B = "Bbc";
	int a = compareStrings(A, B);
	EXPECT_TRUE(a == 2);
}

TEST(compareStrings, bBeforeA_expected0) {
	char* B = "Abc";
	char* A = "Bbc";
	int a = compareStrings(A, B);
	EXPECT_TRUE(a == 0);
}

TEST(oneBeforeOther, headHoursSmaller_expected1) {
	file_line head = { "2020-12-09", "Ivanov", "Ivan", 8 };
	file_line element = { "2020-12-09", "Ivanov", "Ivan", 12 };
	EXPECT_EQ(oneBeforeOther(&head, &element), 1);
}

TEST(oneBeforeOther, headHoursBigger_expected0) {
	file_line head = { "2020-12-09", "Ivanov", "Ivan", 4 };
	file_line element = { "2020-12-09", "Ivanov", "Ivan", 2 };
	EXPECT_EQ(oneBeforeOther(&head, &element), 0);
}

TEST(oneBeforeOther, equalHoursHeadSurnameBefore_expected0) {
	file_line head = { "2020-12-09", "Abakanov", "Ivan", 5 };
	file_line element = { "2020-12-09", "Bykov", "Ivan", 5 };
	EXPECT_EQ(oneBeforeOther(&head, &element), 0);
}

TEST(oneBeforeOther, EqualHoursAndSurnamesElementNameBefore_expected1) {
	file_line head = { "2020-12-09", "Ivanova", "Boris", 4 };
	file_line element = { "2020-12-09", "Ivanova", "Alla", 4 };
	EXPECT_EQ(oneBeforeOther(&head, &element), 1);
}

TEST(findElementPosition, elementCounterEquals0_expectedElementNextNULL) {
	file_line element = { "2020-08-21", "Rubanov", "Boris", 8};
	findElementPosition(0, &element, &element);
	EXPECT_TRUE(element.next == NULL);
}

TEST(findElementPosition, elementAfterHead_expectedElementNextNULLHeadNextElement) {
	file_line head = { "2021-03-12", "Ivanov", "Ivan", 15, NULL };
	file_line element = { "2021-03-12", "Ivanov", "Ivan", 3 };
	findElementPosition(1, &element, &head);
	EXPECT_TRUE(element.next == NULL);
	EXPECT_TRUE(head.next == &element);
}

TEST(findElementPosition, elementAfterHeadBeforeOther_expectedElementNextEl2HeadNextElement) {
	file_line el3 = { "2021-03-12", "Ivanov", "Ivan", 3, NULL };
	file_line el2 = { "2021-03-12", "Ivanov", "Ivan", 4, &el3 };
	file_line el1 = { "2021-03-12", "Ivanov", "Ivan", 15, &el2 };
	file_line element = { "2021-03-12", "Ivanov", "Ivan", 7 };
	findElementPosition(3, &element, &el1);
	EXPECT_TRUE(el1.next == &element);
	EXPECT_TRUE(element.next == &el2);
	EXPECT_TRUE(el2.next == &el3);
	EXPECT_TRUE(el3.next == NULL);
}

TEST(readFile, nonExistingFile_expectedNULL) {
	char* path = "C:\\Git\\GitHub\\alg-dstruct\\3-Rubanova-A24\\ReadFileTest.txt";
	file_line* head = readFile(path);
	EXPECT_TRUE(head == NULL);
}

TEST(readFile, emptyFile_expectedNULL) {
	char* filename = "D:\\Git\\GitHub\\alg-dstruct\\3-Rubanova-A24\\EmptyFileTest.txt";
	file_line* head = readFile(filename);
	EXPECT_TRUE(head == NULL);
}

TEST(readFile, read4LinesinFile_expectedValidVal) {
	char* filename = "D:\\Git\\GitHub\\alg-dstruct\\3-Rubanova-A24\\ReadFileTest.txt";
	file_line* head = readFile(filename);
	ASSERT_TRUE(head != NULL);
	EXPECT_STREQ(head->date, "1988-09-15");
	EXPECT_STREQ(head->surname, "Smith");
	EXPECT_STREQ(head->name, "Adam");
	EXPECT_EQ(head->hours, 15);
	head = head->next;
	ASSERT_TRUE(head != NULL);
	EXPECT_STREQ(head->date, "2021-09-13");
	EXPECT_STREQ(head->surname, "Alexeeva");
	EXPECT_STREQ(head->name, "Alla");
	EXPECT_EQ(head->hours, 8);
	head = head->next;
	ASSERT_TRUE(head != NULL);
	EXPECT_STREQ(head->date, "2020-12-09");
	EXPECT_STREQ(head->surname, "Ivanov");
	EXPECT_STREQ(head->name, "Ivan");
	EXPECT_EQ(head->hours, 7);
	head = head->next;
	ASSERT_TRUE(head != NULL);
	EXPECT_STREQ(head->date, "2015-04-13");
	EXPECT_STREQ(head->surname, "Rubanova");
	EXPECT_STREQ(head->name, "Valeria");
	EXPECT_EQ(head->hours, 4);
	EXPECT_TRUE(head->next == NULL);
}

TEST(countSum, threestrings_expected22) {
	file_line el3 = { "2021-03-12", "Ivanov", "Ivan", 3, NULL };
	file_line el2 = { "2021-03-12", "Ivanov", "Ivan", 4, &el3 };
	file_line el1 = { "2021-03-12", "Ivanov", "Ivan", 15, &el2 };
	int sum = countSum(&el1, "Ivanov", "Ivan");
	EXPECT_TRUE(sum == 22);
}

TEST(isUnique, uniqueName_expected1) {
	full_name* arr = (full_name*)malloc(2 * sizeof(full_name));
	ASSERT_TRUE(arr != NULL);
	arr[0] = { "Petrov", "Sasha" };
	arr[1] = { "Rubanova", "Lera" };
	file_line el1 = { "2021-03-12", "Ivanov", "Ivan", 15, NULL };
	int j = sizeof(arr) / sizeof(arr[0]);
	int i = isUnique(arr, el1, &el1, j);
	EXPECT_EQ(i, 1);
}

TEST(isUnique, notUniqueName_expected0) {
	full_name* arr = (full_name*)malloc(2 * sizeof(full_name));
	ASSERT_TRUE(arr != NULL);
	arr[0] = { "Petrov", "Sasha" };
	arr[1] = { "Rubanova", "Lera" };
	file_line el1 = { "2021-03-12", "Rubanova", "Lera", 15, NULL };
	int j = 2;
	int i = isUnique(arr, el1, &el1, j);
	EXPECT_EQ(i, 0);
}


int main(int* argc, char** argv) {
	int argn = 1;
	testing::InitGoogleTest(&argn, argv);
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
	RUN_ALL_TESTS();
	//_CrtDumpMemoryLeaks();
	return 0;
}