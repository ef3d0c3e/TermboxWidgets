#include "Termbox.hpp"

int main()
{
	std::setlocale(LC_ALL, "en-US.UTF-8");

	try
	{
		Termbox tb(Color::COLORS_TRUECOLOR, COLOR_DEFAULT);
		//MainWindow win("~/Programming/index/src");
		//tb.AddWidget(&win);

		tb.RenderLoop();
	}
	catch (Util::Exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	return EXIT_SUCCESS;
}
