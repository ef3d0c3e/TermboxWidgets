#include "TestWindow.hpp"
#include "../Settings.hpp"
#include "../Draw.hpp"


MyWindow::MyWindow(Window* parent) : Window::Window(TBString{U"Fenêtre test avec un nom très long", Settings::default_window_name_style}, parent)
{
	SetSize({40, 24});
	SetPosition({3,4});

	SetBorderFlags(0b1111'1111);
	SetBackground(TBChar(U' ', 0xFFFFFF, 0x909090), false, false);

	auto fun = [&](
		std::size_t i,
		const std::vector<String>& entries,
		Vec2i pos,
		int w,
		bool hovered,
		bool marked,
		Char trailing
	) -> std::pair<int, TBStyle>
	{
		TBStyle s;
		if (hovered)
			s = {0xFFFFFF, 0x000000, TextStyle::Bold};
		else
			s = {0x000000, 0xFFFFFF, TextStyle::None};

		TBStyle _s = s;
		_s.fg = 0xFF0000;
		const auto p = Draw::TextLine(entries[i], s, pos, w, {trailing, _s}).first;
		Draw::Horizontal({' ', s}, pos+Vec2i(p,0), w-p);


		return std::make_pair(w, _s);
	};


	using ListSelect = Widgets::ListSelect<
		Widgets::ListSelectSettings{},
		String>;

	auto* list = new ListSelect(fun);
	list->SetEntries({
		U" Cours",
		U" Desktop",
		U" Documents",
		U" Downloads",
		U" Drives",
		U" interpolation-samples",
		U" Music",
		U" phone",
		U" Pictures",
		U" Programming",
		U" Tex",
		U" USB",
		U" vectest",
		U" a",
		U" a.c",
		U" a.out",
		U" compression_test.png",
		U" compression_test.xcf",
		U" ftl",
		U" Manifest",
		U" polynomes_exos_classiques.pdf",
		U" Svencoop.md",
		U" trashmail"});
	//auto&& [pos, dim] = GetInteriorSpace(m_pos, m_dim);
	list->SetPosition({1,5});
	list->SetSize({37,15});
	list->DefaultKeyBindings();
	AddWidget(list);


	auto* line = new Widgets::TextLine(this, {U"test", Settings::default_text_style});
	line->SetSize({20, 0});
	AddWidget(line);
	
	Termbox::OnRepeatChange.AddEvent([this, line](std::size_t r)
	{
		line->SetLine({Util::ToString(r), Settings::default_text_style});
		line->SetSize({20, 0});
		ReDraw(line);
	});



	//TODO: '0 G'
	//TODO: resizeable...

	//list->EventListener.OnMove([]()...);

	AddKeyboardInput({U"q", [](Termbox& tb){ tb.GetContext().stop = true; }});

	/*
	AddEvent({[this](const TimedEvent& ev, Widget& w, Termbox& tb)
	{
		static Vec2i dir(3,1);
		if (GetPosition()[0] <= 0)
			dir[0] = 3;
		if (GetPosition()[1] <= 0)
			dir[1] = 1;
		if (GetPosition()[0]+GetSize()[0] >= Termbox::GetDim()[0])
			dir[0] = -3;
		if (GetPosition()[1]+GetSize()[1] >= Termbox::GetDim()[1])
			dir[1] = -1;
		SetPosition(GetPosition()+dir);
		tb.Clear();
		tb.ForceDraw();
		tb.Display();
	}, 16, 1000}, true);*/
	/*
	AddEvent({[this](const TimedEvent& ev, Widget& w, Termbox& tb)
	{
		SetPosition(GetPosition()+Vec2i(1, 0));
		tb.Clear();
		tb.ForceDraw();
		tb.Display();
	}, 200, 50}, true);

	OnSetPosition.AddEvent([this, line](const Vec2i& pos){
		line->SetLine({Util::ToString<10>(GetPosition()[0]), Settings::default_text_style});
	});*/


	/*
	auto* line = new Widgets::TextLine(this, {U"Ligne de texte", Settings::default_text_style});
	line->SetPosition({15, 1});
	line->SetDim({12, 1});
	AddWidget(line);


	std::pair<Vec2i, Vec2i> area = {{0, 0}, {15, 15}};
	AddMouseInput({area, Mouse::MOUSE_WHEEL_UP, [](Termbox& tb, const Vec2i& pos){ tb.GetContext().stop = true; }});

	auto* button = new Widgets::Button(this, {U"Texte du boutton, ce boutton a beaucoup de texte 1234567890.", TBStyle(0xFF0000, 0x00FF00, TextStyle::Underline)});
	button->SetBackground({U' ', {0xFFFFFF, 0xFF0000, TextStyle::None}});
	button->SetPosition({9, 7});
	button->SetDim({13, 12});
	AddWidget(button);*/

	/*auto* in = new Widgets::InputLine(U"baaaaゆedcbaほdそぴれabcdefgh", 0);
	in->SetPosition({2, 2});
	in->SetDim({16,1});
	in->SetActive(true);
	AddWidget(in);*/

	SetProcessTimed(true);
}


MyWindow::~MyWindow()
{
	SetProcessTimed(false);
}
