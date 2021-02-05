#include "my/TestWindow.hpp"
#include "tests.hpp"
#include <unistd.h>
#include <chrono>

int main()
{
	std::setlocale(LC_ALL, "en_US.UTF-8");

	//if (!TestAll())
	//	return EXIT_FAILURE;

	//std::cout << "FIT:" << std::boolalpha << my.CheckAllWidgets() << "\n";

	try
	{
		Termbox tb(Color::COLORS_TRUECOLOR, 0xF0F000);
		MyWindow my;
		tb.AddWidget(&my);

		tb.Clear();
		tb.ReDraw();
		tb.Display();

		tb.RenderLoop();
	}
	catch (Util::Exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	return EXIT_SUCCESS;
}
