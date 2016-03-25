#include <UnitTest.hpp>
#include <String.hpp>

TEST_CASE(StringConstruct)
{
    using Yupei::utf8_string;

    utf8_string str {"hahahahahahahahahahahahahahahah"};
    TEST_ASSERT(str[0] == 'h');
    TEST_ASSERT(str[1] == 'a');
    TEST_ASSERT(str[2] == 'h');
    TEST_ASSERT(str[3] == 'a');
    TEST_ASSERT(str[4] == 'h');
    TEST_ASSERT(str[5] == 'a');
    TEST_ASSERT(str[6] == 'h');
    TEST_ASSERT(str[7] == 'a');
    TEST_ASSERT(str[8] == 'h');
    TEST_ASSERT(str[9] == 'a');
    TEST_ASSERT(str[10] == 'h');
    TEST_ASSERT(str[11] == 'a');
    TEST_ASSERT(str[12] == 'h');
    TEST_ASSERT(str[13] == 'a');
    TEST_ASSERT(str[14] == 'h');
    TEST_ASSERT(str[15] == 'a');
    TEST_ASSERT(str[16] == 'h');
    TEST_ASSERT(str[17] == 'a');
    TEST_ASSERT(str[18] == 'h');
    TEST_ASSERT(str[19] == 'a');
    TEST_ASSERT(str[20] == 'h');
    TEST_ASSERT(str[21] == 'a');
    TEST_ASSERT(str[22] == 'h');
    TEST_ASSERT(str[23] == 'a');
    TEST_ASSERT(str[24] == 'h');
    TEST_ASSERT(str[25] == 'a');
    TEST_ASSERT(str[26] == 'h');
    TEST_ASSERT(str[27] == 'a');
    TEST_ASSERT(str[28] == 'h');
    TEST_ASSERT(str[29] == 'a');
    TEST_ASSERT(str[30] == 'h');
    TEST_ASSERT(str.size() == 31);
    
    const auto cStr = str.c_str();
    TEST_ASSERT(cStr[31] == '\0');
    TEST_ASSERT(str.size() == 31);
}

TEST_CASE(StringAppendAndReplace)
{
    using namespace Yupei;
    utf8_string str;
    for (int i = 0; i < 100; ++i)
        str.append("haha");
    
    TEST_ASSERT(str.size() == 400);

    str.append("Cool!");

    for (int i = 0;i < 400; i += 2)
    {
        TEST_ASSERT(str[i] == 'h');
        TEST_ASSERT(str[i + 1] == 'a');
    }

    //ǿ���ط��䡣
    str.replace({"ha"}, {"haha"});

    TEST_ASSERT(str.size() == 805);

    str.replace("haha", "ha");

    TEST_ASSERT(str.size() == 405);
}

TEST_CASE(SmallStringReplace)
{
    using namespace Yupei;

    //���Ծ���С�ڴ�������
    utf8_string str {"abcba"};

    str.replace("ab", "abc");

    TEST_ASSERT(str[0] == 'a');
    TEST_ASSERT(str[1] == 'b');
    TEST_ASSERT(str[2] == 'c');
    TEST_ASSERT(str[3] == 'c');
    TEST_ASSERT(str[4] == 'b');
    TEST_ASSERT(str[5] == 'a');

    TEST_ASSERT(str.size() == 6);

    auto cStr = str.c_str();

    TEST_ASSERT(cStr[6] == '\0');

    TEST_ASSERT(str.size() == 6);
}

TEST_CASE(SmallStringReplaceToBig)
{
    using namespace Yupei;

    //������С�ڴ棬replace ��Ϻ󵽴��ڴ�������
    utf8_string str {"abcba"};

    str.replace("ab", "abcdefghi");

    TEST_ASSERT(str[0] == 'a');
    TEST_ASSERT(str[1] == 'b');
    TEST_ASSERT(str[2] == 'c');
    TEST_ASSERT(str[3] == 'd');
    TEST_ASSERT(str[4] == 'e');
    TEST_ASSERT(str[5] == 'f');
    TEST_ASSERT(str[6] == 'g');
    TEST_ASSERT(str[7] == 'h');
    TEST_ASSERT(str[8] == 'i');
    TEST_ASSERT(str[9] == 'c');
    TEST_ASSERT(str[10] == 'b');
    TEST_ASSERT(str[11] == 'a');

    TEST_ASSERT(str.size() == 12);

    auto cStr = str.c_str();

    TEST_ASSERT(cStr[12] == '\0');

    TEST_ASSERT(str.size() == 12);
}

TEST_CASE(StringSmallInsert)
{
    using namespace Yupei;

    utf8_string str {"abefg"};

    str.insert(2, {"cd"});

    TEST_ASSERT(str.size() == 7);

    for (std::size_t i = 0; i < str.size(); ++i)
        TEST_ASSERT(str[i] == static_cast<char>('a' + i));
}

TEST_CASE(StringBigInsert)
{
    using namespace Yupei;

    utf8_string str;

    for (int i = 0;i < 100;++i)
        str.append("abcd");

    str.insert(0, "abcd");

    for (int i = 0; i < 404; i += 4)
    {
        TEST_ASSERT(str[i] == 'a');
        TEST_ASSERT(str[i + 1] == 'b');
        TEST_ASSERT(str[i + 2] == 'c');
        TEST_ASSERT(str[i + 3] == 'd');
    }
}

TEST_CASE(StringRemove)
{
    using namespace Yupei;

    utf8_string str {"abfcd"};

    str.remove(2, 1);

    TEST_ASSERT(str.size() == 4);

    for (std::size_t i = 0; i < str.size(); ++i)
        TEST_ASSERT(str[i] == static_cast<char>('a' + i));
}
