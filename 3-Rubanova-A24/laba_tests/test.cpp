#include "pch.h"
#include "laba_header.h"


TEST(CharToInt, Char7toInt7_expectedInt7) {
	char a = '7';
	int b = CharToInt(a);
	EXPECT_EQ(b, 7);
}

TEST(CharToInt, CharAtoIntA_ExpectedNegative1) {
	char a = 'A';
	int b = CharToInt(a);
	EXPECT_EQ(b, -1);
}

TEST(MakeNumOutOfDigits, Char1234toInt1234_ExpectedInt1234) {
	char* a = "1234";
	file_line element = { "2020-12-09", "Ivanov", "Sasha" };
	MakeNumOutOfDigits(&element, a);
	EXPECT_TRUE(element.hours == 1234);
}

TEST(MakeNumOutOfDigits, Char1toInt1_ExpectedInt1) {
	char* a = "1";
	file_line element = { "2020-12-09", "Ivanov", "Sasha" };
	MakeNumOutOfDigits(&element, a);
	EXPECT_TRUE(element.hours == 1);
}

TEST(StringToStructure, LineToStructure_ExpectedValidVal) {
	char* a = "2021-09-10,Ivanov,Ivan,8\n";
	file_line element = StringToStructure(a);
	EXPECT_STREQ(element.date, "2021-09-10");
	EXPECT_STREQ(element.surname, "Ivanov");
	EXPECT_STREQ(element.name, "Ivan");
	EXPECT_EQ(element.hours, 8);
}

TEST(CompareStrings, EqualStrings_Expected1) {
	int a = CompareStrings("Ivanova", "Ivanova");
	EXPECT_TRUE(a == 1);
}

TEST(CompareStrings, ABeforeB_Expected2) {
	char* A = "Abc";
	char* B = "Bbc";
	int a = CompareStrings(A, B);
	EXPECT_TRUE(a == 2);
}

TEST(CompareStrings, BBeforeA_Expected0) {
	char* B = "Abc";
	char* A = "Bbc";
	int a = CompareStrings(A, B);
	EXPECT_TRUE(a == 0);
}

TEST(HeadSmaller, HeadHoursSmaller_Expected1) {
	file_line head = { "2020-12-09","Ivanov","Ivan",8 };
	file_line element = { "2020-12-09","Ivanov","Ivan",12 };
	EXPECT_EQ(HeadSmaller(&head, &element), 1);
}

TEST(HeadSmaller, HeadHoursBigger_Expected0) {
	file_line head = { "2020-12-09","Ivanov","Ivan",4 };
	file_line element = { "2020-12-09","Ivanov","Ivan",2 };
	EXPECT_EQ(HeadSmaller(&head, &element), 0);
}

TEST(HeadSmaller, EqualHoursHeadSurnameBefore_Expected0) {
	file_line head = { "2020-12-09","Abakanov","Ivan",5 };
	file_line element = { "2020-12-09","Bykov","Ivan",5 };
	EXPECT_EQ(HeadSmaller(&head, &element), 0);
}

TEST(HeadSmaller, EqualHoursAndSurnamesElementNameBefore_Expected1) {
	file_line head = { "2020-12-09","Ivanova","Boris",4 };
	file_line element = { "2020-12-09","Ivanova","Alla",4 };
	EXPECT_EQ(HeadSmaller(&head, &element), 1);
}

TEST(FindElementPosition, ElementCounterEquals0_ExpectedElementNextNULL) {
	file_line element = { "2020-08-21", "Rubanov" , "Boris", 8};
	FindElementPosition(0, &element, &element);
	EXPECT_TRUE(element.next == NULL);
}

TEST(FindElementPosition, ElementAfterHead_ExpectedElementNextNULLHeadNextElement) {
	file_line head = { "2021-03-12","Ivanov", "Ivan", 15,NULL };
	file_line element = { "2021-03-12","Ivanov", "Ivan", 3 };
	FindElementPosition(1, &element, &head);
	EXPECT_TRUE(element.next == NULL);
	EXPECT_TRUE(head.next == &element);
}

TEST(FindElementPosition, ElementAfterHeadBeforeOther_ExpectedElementNextEl2HeadNextElement) {
	file_line el3 = { "2021-03-12","Ivanov", "Ivan", 3,NULL };
	file_line el2 = { "2021-03-12","Ivanov", "Ivan", 4,&el3 };
	file_line el1 = { "2021-03-12","Ivanov", "Ivan", 15,&el2 };
	file_line element = { "2021-03-12","Ivanov", "Ivan", 7 };
	FindElementPosition(3, &element, &el1);
	EXPECT_TRUE(element.next == &el2);
	EXPECT_TRUE(el1.next == &element);
}

TEST(ReadFile, Read4LinesinFile_ExpectedValidVal) {
	char* filename = "D:\\Git\GitHub\alg-dstruct\3-Rubanova-A24\ReadFileTest.txt";
	file_line* head = ReadFile(filename);
	EXPECT_STREQ(head->date,"1988-09-15");
	EXPECT_STREQ(head->surname, "Smith");
	EXPECT_STREQ(head->name, "Adam");
	EXPECT_EQ(head->hours, 15);
//	head = head->next;
//	EXPECT_STREQ(head->date, "2021-09-13");
//	EXPECT_STREQ(head->surname, "Alexeeva");
//	EXPECT_STREQ(head->name, "Alla");
//	EXPECT_EQ(head->hours, 8);
//	head = head->next;
//	EXPECT_STREQ(head->date, "2020-12-09");
//	EXPECT_STREQ(head->surname, "Ivanov");
//	EXPECT_STREQ(head->name, "Ivan");
//	EXPECT_EQ(head->hours, 7);
//	head = head->next;
//	EXPECT_STREQ(head->date, "2015-04-13");
//	EXPECT_STREQ(head->surname, "Rubanova");
//	EXPECT_STREQ(head->name, "Valeria");
//	EXPECT_EQ(head->hours, 4);
//	EXPECT_EQ(head->next, NULL);
}