#ifndef TERMBOX_WIDGETS_TESTS_HPP
#define TERMBOX_WIDGETS_TESTS_HPP

#include "Input.hpp"

struct Test
{
	String name;
	std::function<std::vector<String>(void)> testFn;
	std::vector<String> expected;

	Test(){};
	Test& operator=(const Test& t)
	{
		name = t.name;
		testFn = t.testFn;
		expected = t.expected;

		return *this;
	}

	Test(const String&& __name,
		 const std::function<std::vector<String>(void)> __testFn,
		 const std::vector<String> __expected)
	: name(__name), testFn(__testFn), expected(__expected)
	{}

	std::vector<String> RunTest() const
	{
		return testFn();
	}
};

static Test KeyCombTest(U"KeyComb", //+ Currently testing alpha chars only
						[]() {
							std::vector<String> r;
							const String kc[] = {
								//{{{* Basic chars
								U"a", U"S-a", U"S-A", U"A", U"b", U"S-b", U"S-B", U"B",
								U"c", U"S-c", U"S-C", U"C", U"d", U"S-d", U"S-D", U"D",
								U"e", U"S-e", U"S-E", U"E", U"f", U"S-f", U"S-F", U"F",
								U"g", U"S-g", U"S-G", U"G", U"h", U"S-h", U"S-H", U"H",
								U"i", U"S-i", U"S-I", U"I", U"j", U"S-j", U"S-J", U"J",
								U"k", U"S-k", U"S-K", U"K", U"l", U"S-l", U"S-L", U"L",
								U"m", U"S-m", U"S-M", U"M", U"n", U"S-n", U"S-N", U"N",
								U"o", U"S-o", U"S-O", U"O", U"p", U"S-p", U"S-P", U"P",
								U"q", U"S-q", U"S-Q", U"Q", U"r", U"S-r", U"S-R", U"R",
								U"s", U"S-s", U"S-S", U"S", U"t", U"S-t", U"S-T", U"T",
								U"u", U"S-u", U"S-U", U"U", U"v", U"S-v", U"S-V", U"V",
								U"w", U"S-w", U"S-W", U"W", U"x", U"S-x", U"S-X", U"X",
								U"y", U"S-y", U"S-Y", U"Y", U"z", U"S-z", U"S-Z", U"Z",
								///}}}
								//{{{* CTRL
								U"C-a", U"C-S-a", U"C-S-A", U"C-A", U"C-b", U"C-S-b", U"C-S-B", U"C-B",
								U"C-c", U"C-S-c", U"C-S-C", U"C-C", U"C-d", U"C-S-d", U"C-S-D", U"C-D",
								U"C-e", U"C-S-e", U"C-S-E", U"C-E", U"C-f", U"C-S-f", U"C-S-F", U"C-F",
								U"C-g", U"C-S-g", U"C-S-G", U"C-G", U"C-h", U"C-S-h", U"C-S-H", U"C-H",
								U"C-i", U"C-S-i", U"C-S-I", U"C-I", U"C-j", U"C-S-j", U"C-S-J", U"C-J",
								U"C-k", U"C-S-k", U"C-S-K", U"C-K", U"C-l", U"C-S-l", U"C-S-L", U"C-L",
								U"C-m", U"C-S-m", U"C-S-M", U"C-M", U"C-n", U"C-S-n", U"C-S-N", U"C-N",
								U"C-o", U"C-S-o", U"C-S-O", U"C-O", U"C-p", U"C-S-p", U"C-S-P", U"C-P",
								U"C-q", U"C-S-q", U"C-S-Q", U"C-Q", U"C-r", U"C-S-r", U"C-S-R", U"C-R",
								U"C-s", U"C-S-s", U"C-S-S", U"C-S", U"C-t", U"C-S-t", U"C-S-T", U"C-T",
								U"C-u", U"C-S-u", U"C-S-U", U"C-U", U"C-v", U"C-S-v", U"C-S-V", U"C-V",
								U"C-w", U"C-S-w", U"C-S-W", U"C-W", U"C-x", U"C-S-x", U"C-S-X", U"C-X",
								U"C-y", U"C-S-y", U"C-S-Y", U"C-Y", U"C-z", U"C-S-z", U"C-S-Z", U"C-Z",
								//}}}
								//{{{* ALT
								U"M-a", U"M-S-a", U"M-S-A", U"M-A", U"M-b", U"M-S-b", U"M-S-B", U"M-B",
								U"M-c", U"M-S-c", U"M-S-C", U"M-C", U"M-d", U"M-S-d", U"M-S-D", U"M-D",
								U"M-e", U"M-S-e", U"M-S-E", U"M-E", U"M-f", U"M-S-f", U"M-S-F", U"M-F",
								U"M-g", U"M-S-g", U"M-S-G", U"M-G", U"M-h", U"M-S-h", U"M-S-H", U"M-H",
								U"M-i", U"M-S-i", U"M-S-I", U"M-I", U"M-j", U"M-S-j", U"M-S-J", U"M-J",
								U"M-k", U"M-S-k", U"M-S-K", U"M-K", U"M-l", U"M-S-l", U"M-S-L", U"M-L",
								U"M-m", U"M-S-m", U"M-S-M", U"M-M", U"M-n", U"M-S-n", U"M-S-N", U"M-N",
								U"M-o", U"M-S-o", U"M-S-O", U"M-O", U"M-p", U"M-S-p", U"M-S-P", U"M-P",
								U"M-q", U"M-S-q", U"M-S-Q", U"M-Q", U"M-r", U"M-S-r", U"M-S-R", U"M-R",
								U"M-s", U"M-S-s", U"M-S-S", U"M-S", U"M-t", U"M-S-t", U"M-S-T", U"M-T",
								U"M-u", U"M-S-u", U"M-S-U", U"M-U", U"M-v", U"M-S-v", U"M-S-V", U"M-V",
								U"M-w", U"M-S-w", U"M-S-W", U"M-W", U"M-x", U"M-S-x", U"M-S-X", U"M-X",
								U"M-y", U"M-S-y", U"M-S-Y", U"M-Y", U"M-z", U"M-S-z", U"M-S-Z", U"M-Z",
								//}}}
								//{{{* ALT CTRL
								U"M-C-a", U"M-C-S-a", U"M-C-S-A", U"M-C-A", U"M-C-b", U"M-C-S-b", U"M-C-S-B", U"M-C-B",
								U"M-C-c", U"M-C-S-c", U"M-C-S-C", U"M-C-C", U"M-C-d", U"M-C-S-d", U"M-C-S-D", U"M-C-D",
								U"M-C-e", U"M-C-S-e", U"M-C-S-E", U"M-C-E", U"M-C-f", U"M-C-S-f", U"M-C-S-F", U"M-C-F",
								U"M-C-g", U"M-C-S-g", U"M-C-S-G", U"M-C-G", U"M-C-h", U"M-C-S-h", U"M-C-S-H", U"M-C-H",
								U"M-C-i", U"M-C-S-i", U"M-C-S-I", U"M-C-I", U"M-C-j", U"M-C-S-j", U"M-C-S-J", U"M-C-J",
								U"M-C-k", U"M-C-S-k", U"M-C-S-K", U"M-C-K", U"M-C-l", U"M-C-S-l", U"M-C-S-L", U"M-C-L",
								U"M-C-m", U"M-C-S-m", U"M-C-S-M", U"M-C-M", U"M-C-n", U"M-C-S-n", U"M-C-S-N", U"M-C-N",
								U"M-C-o", U"M-C-S-o", U"M-C-S-O", U"M-C-O", U"M-C-p", U"M-C-S-p", U"M-C-S-P", U"M-C-P",
								U"M-C-q", U"M-C-S-q", U"M-C-S-Q", U"M-C-Q", U"M-C-r", U"M-C-S-r", U"M-C-S-R", U"M-C-R",
								U"M-C-s", U"M-C-S-s", U"M-C-S-S", U"M-C-S", U"M-C-t", U"M-C-S-t", U"M-C-S-T", U"M-C-T",
								U"M-C-u", U"M-C-S-u", U"M-C-S-U", U"M-C-U", U"M-C-v", U"M-C-S-v", U"M-C-S-V", U"M-C-V",
								U"M-C-w", U"M-C-S-w", U"M-C-S-W", U"M-C-W", U"M-C-x", U"M-C-S-x", U"M-C-S-X", U"M-C-X",
								U"M-C-y", U"M-C-S-y", U"M-C-S-Y", U"M-C-Y", U"M-C-z", U"M-C-S-z", U"M-C-S-Z", U"M-C-Z",
								//}}}
							};

							for (std::size_t i = 0; i < ARRAY_LENGTH(kc); ++i)
								r.push_back(KeyComb(kc[i], [](Termbox&) {}).GetName());

							return r;
						},

						{
							//{{{* Basic chars
							U"a", U"A", U"A", U"A", U"b", U"B", U"B", U"B",
							U"c", U"C", U"C", U"C", U"d", U"D", U"D", U"D",
							U"e", U"E", U"E", U"E", U"f", U"F", U"F", U"F",
							U"g", U"G", U"G", U"G", U"h", U"H", U"H", U"H",
							U"i", U"I", U"I", U"I", U"j", U"J", U"J", U"J",
							U"k", U"K", U"K", U"K", U"l", U"L", U"L", U"L",
							U"m", U"M", U"M", U"M", U"n", U"N", U"N", U"N",
							U"o", U"O", U"O", U"O", U"p", U"P", U"P", U"P",
							U"q", U"Q", U"Q", U"Q", U"r", U"R", U"R", U"R",
							U"s", U"S", U"S", U"S", U"t", U"T", U"T", U"T",
							U"u", U"U", U"U", U"U", U"v", U"V", U"V", U"V",
							U"w", U"W", U"W", U"W", U"x", U"X", U"X", U"X",
							U"y", U"Y", U"Y", U"Y", U"z", U"Z", U"Z", U"Z",
							///}}}
							//{{{* CTRL
							U"C-a", U"C-A", U"C-A", U"C-A", U"C-b", U"C-B", U"C-B", U"C-B",
							U"C-c", U"C-C", U"C-C", U"C-C", U"C-d", U"C-D", U"C-D", U"C-D",
							U"C-e", U"C-E", U"C-E", U"C-E", U"C-f", U"C-F", U"C-F", U"C-F",
							U"C-g", U"C-G", U"C-G", U"C-G", U"C-h", U"C-H", U"C-H", U"C-H",
							U"C-i", U"C-I", U"C-I", U"C-I", U"C-j", U"C-J", U"C-J", U"C-J",
							U"C-k", U"C-K", U"C-K", U"C-K", U"C-l", U"C-L", U"C-L", U"C-L",
							U"C-m", U"C-M", U"C-M", U"C-M", U"C-n", U"C-N", U"C-N", U"C-N",
							U"C-o", U"C-O", U"C-O", U"C-O", U"C-p", U"C-P", U"C-P", U"C-P",
							U"C-q", U"C-Q", U"C-Q", U"C-Q", U"C-r", U"C-R", U"C-R", U"C-R",
							U"C-s", U"C-S", U"C-S", U"C-S", U"C-t", U"C-T", U"C-T", U"C-T",
							U"C-u", U"C-U", U"C-U", U"C-U", U"C-v", U"C-V", U"C-V", U"C-V",
							U"C-w", U"C-W", U"C-W", U"C-W", U"C-x", U"C-X", U"C-X", U"C-X",
							U"C-y", U"C-Y", U"C-Y", U"C-Y", U"C-z", U"C-Z", U"C-Z", U"C-Z",
							///}}}
							//{{{* ALT
							U"M-a", U"M-A", U"M-A", U"M-A", U"M-b", U"M-B", U"M-B", U"M-B",
							U"M-c", U"M-C", U"M-C", U"M-C", U"M-d", U"M-D", U"M-D", U"M-D",
							U"M-e", U"M-E", U"M-E", U"M-E", U"M-f", U"M-F", U"M-F", U"M-F",
							U"M-g", U"M-G", U"M-G", U"M-G", U"M-h", U"M-H", U"M-H", U"M-H",
							U"M-i", U"M-I", U"M-I", U"M-I", U"M-j", U"M-J", U"M-J", U"M-J",
							U"M-k", U"M-K", U"M-K", U"M-K", U"M-l", U"M-L", U"M-L", U"M-L",
							U"M-m", U"M-M", U"M-M", U"M-M", U"M-n", U"M-N", U"M-N", U"M-N",
							U"M-o", U"M-O", U"M-O", U"M-O", U"M-p", U"M-P", U"M-P", U"M-P",
							U"M-q", U"M-Q", U"M-Q", U"M-Q", U"M-r", U"M-R", U"M-R", U"M-R",
							U"M-s", U"M-S", U"M-S", U"M-S", U"M-t", U"M-T", U"M-T", U"M-T",
							U"M-u", U"M-U", U"M-U", U"M-U", U"M-v", U"M-V", U"M-V", U"M-V",
							U"M-w", U"M-W", U"M-W", U"M-W", U"M-x", U"M-X", U"M-X", U"M-X",
							U"M-y", U"M-Y", U"M-Y", U"M-Y", U"M-z", U"M-Z", U"M-Z", U"M-Z",
							///}}}
							//{{{* ALT CTRL
							U"C-M-a", U"C-M-A", U"C-M-A", U"C-M-A", U"C-M-b", U"C-M-B", U"C-M-B", U"C-M-B",
							U"C-M-c", U"C-M-C", U"C-M-C", U"C-M-C", U"C-M-d", U"C-M-D", U"C-M-D", U"C-M-D",
							U"C-M-e", U"C-M-E", U"C-M-E", U"C-M-E", U"C-M-f", U"C-M-F", U"C-M-F", U"C-M-F",
							U"C-M-g", U"C-M-G", U"C-M-G", U"C-M-G", U"C-M-h", U"C-M-H", U"C-M-H", U"C-M-H",
							U"C-M-i", U"C-M-I", U"C-M-I", U"C-M-I", U"C-M-j", U"C-M-J", U"C-M-J", U"C-M-J",
							U"C-M-k", U"C-M-K", U"C-M-K", U"C-M-K", U"C-M-l", U"C-M-L", U"C-M-L", U"C-M-L",
							U"C-M-m", U"C-M-M", U"C-M-M", U"C-M-M", U"C-M-n", U"C-M-N", U"C-M-N", U"C-M-N",
							U"C-M-o", U"C-M-O", U"C-M-O", U"C-M-O", U"C-M-p", U"C-M-P", U"C-M-P", U"C-M-P",
							U"C-M-q", U"C-M-Q", U"C-M-Q", U"C-M-Q", U"C-M-r", U"C-M-R", U"C-M-R", U"C-M-R",
							U"C-M-s", U"C-M-S", U"C-M-S", U"C-M-S", U"C-M-t", U"C-M-T", U"C-M-T", U"C-M-T",
							U"C-M-u", U"C-M-U", U"C-M-U", U"C-M-U", U"C-M-v", U"C-M-V", U"C-M-V", U"C-M-V",
							U"C-M-w", U"C-M-W", U"C-M-W", U"C-M-W", U"C-M-x", U"C-M-X", U"C-M-X", U"C-M-X",
							U"C-M-y", U"C-M-Y", U"C-M-Y", U"C-M-Y", U"C-M-z", U"C-M-Z", U"C-M-Z", U"C-M-Z",
							///}}}
						});

static Test ConversionTest(U"Conversion Test", []() {
	std::vector<String> r;

	// Integral
	r.push_back(Util::ToString<10>(189412574));
	r.push_back(Util::ToString<10>(7984718));
	r.push_back(Util::ToString<10>(-37818947));
	r.push_back(Util::ToString<10>(1527837818947ul));
	r.push_back(Util::ToString<10>(95474529137818947ul));

	return r;
},
{ U"189412574", U"7984718", U"-37818947", U"1527837818947", U"95474529137818947" });

static const auto testList = Util::make_array(KeyCombTest, ConversionTest);

static bool TestAll()
{
	for (const auto& T : testList)
	{
		auto results = T.RunTest();
		if (results != T.expected)
		{
			std::cerr << " - Test Failed: " << Util::StringConvert<char>(T.name) << "\n"
					  << "________________[ EXPECTED ]________________\n";
			for (const auto& x : T.expected)
				std::cerr << Util::StringConvert<char>(x) << "\n";
			std::cerr << "________________[   GOT    ]________________\n";
			for (const auto& x : results)
				std::cerr << Util::StringConvert<char>(x) << "\n";
			std::cerr << "____________________________________________\n";

			return false;
		}
		std::cerr << " - Test Passed: " << Util::StringConvert<char>(T.name) << "\n";
	}

	return true;
}

#endif // TERMBOX_WIDGETS_TESTS_HPP
