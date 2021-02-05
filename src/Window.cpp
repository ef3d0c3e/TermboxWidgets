#include "Window.hpp"
#include "Draw.hpp"

// Will return true if at least one event has been processed
bool Window::ProcessKeyboardEvent(Termbox& tb)
{
	for (auto it : m_widgets)
	{
		if (it.second->IsActive())
			it.first = it.second->ProcessKeyboardEvent(tb);

		if (it.first)
			return true;
	}

	return KeyboardInput::ProcessKeyboardEvent(tb); // Only redraw the whole window if needed
}

bool Window::ProcessMouseEvent(Termbox& tb, const Widget& w)
{
	for (auto it : m_widgets)
	{
		if (it.second->IsActive())
			it.first = it.second->ProcessMouseEvent(tb, *it.second);

		if (it.first)
			return true;
	}

	return MouseInput::ProcessMouseEvent(tb, w);
}

void Window::Draw()
{
	if (m_invalidate) [[unlikely]]
	{
		// Border
		const auto rect = BorderItem::Draw(GetPosition(), GetSize());
		m_ipos = rect.first;
		m_isize = rect.second;


		// Title
		if (BorderItem::GetBorderFlags() & Draw::BorderFlag::Top)
		{
			const auto p = Draw::TextLine(m_windowName, GetPosition() + Vec2i(1, 0), GetSize()[0] - 1, { Settings::trailing_character, Settings::default_window_name_style }).first;
			Draw::Horizontal(BorderItem::GetBorder()[4], GetPosition() + Vec2i(1 + p, 0), GetSize()[0] - 1 - p);
		}

		// Background
		Draw::Rectangle(m_bg, m_ipos, m_isize);
	}

	// Widgets
	for (auto it : m_widgets)
	{
		if (!m_invalidate && !it.first)
			continue;

		auto wPos = it.second->GetPosition();
		it.second->SetPosition(wPos + m_ipos);
		it.second->Draw();
		it.second->SetPosition(wPos);
		it.first = false;
	}

	if (m_invalidate) [[unlikely]]
		m_invalidate = false;
}

void Window::Resize(Vec2i dim)
{
}

void Window::AddWidget(Widget* widget)
{
	m_widgets.push_back(std::make_pair(true, widget));
}

Window::Window(const TBString& windowName, Window* parent):
	Widget::Widget(parent),
	m_windowName(windowName),
	m_bg(Settings::fill_character, { COLOR_DEFAULT, COLOR_DEFAULT, TextStyle::None }),
	m_invalidate(true),
	m_ipos(0, 0),
	m_isize(0, 0)
{
	GetPosition() = { 0, 0 };
	GetSize() = { 0, 0 };
}

Window::~Window()
{
	for (auto widget : m_widgets)
		delete widget.second;
}

void Window::SetName(const TBString& name)
{
	m_windowName = name;
}

const TBString& Window::GetName() const
{
	return m_windowName;
}

void Window::SetBackground(const TBChar& bg, bool border, bool title)
{
	m_bg = bg;
	if (border)
	{
		BorderItem::SetBackgroundColor(m_bg.s.bg);
		BorderItem::SetForegroundColor(m_bg.s.fg);
	}
	if (title)
	{
		for (auto& c : m_windowName)
		{
			c.s.bg = m_bg.s.bg;
			c.s.fg = m_bg.s.fg;
		}
	}
}

const TBChar& Window::GetBackground() const
{
	return m_bg;
}

bool Window::CheckAllWidgets() const
{
	for (auto widget : m_widgets)
	{
		if (!widget.second->IsCorrect())
			return false;
	}

	return true;
}

std::pair<Vec2i, Vec2i> Window::GetGlboalBounds() const
{
	return {m_ipos, m_isize};
}

void Window::ReDraw(Widget* widget) const
{
	auto wPos = widget->GetPosition();
	widget->SetPosition(wPos + m_ipos);
	widget->Draw();
	widget->SetPosition(wPos);
}

void Window::Invalidate()
{
	m_invalidate = true;
}
