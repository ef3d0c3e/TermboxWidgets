#ifndef TESTWINDOW_HPP
#define TESTWINDOW_HPP

#include "../Window.hpp"

class MyWindow : public Window
{
public:
	MyWindow(Window* parent = nullptr);
	~MyWindow();
};

#endif // TESTWINDOW_HPP
