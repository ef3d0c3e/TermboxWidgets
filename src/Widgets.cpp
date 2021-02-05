#include "Widgets.hpp"
#include "Draw.hpp"
#include "Window.hpp"

// {{{ BorderItem
BorderItem::BorderItem()
: m_fixDoubleWide(true)
{
	for (std::size_t i = 0; i < m_border.size(); ++i)
		m_border[i] = TBChar(Settings::default_border[i], Settings::default_border_style);
	m_borderFlags = Draw::BorderFlag::All;
}

std::pair<Vec2i, Vec2i> BorderItem::Draw(Vec2i pos, Vec2i size) const
{
	// Remove wide characters
	if (m_fixDoubleWide)
	{
		static constexpr auto charFn = [](const struct tb_cell& cell, Vec2i pos) -> struct tb_cell
		{
			if (wcwidth(cell.ch) != 1)
				return { Settings::trailing_character, cell.fg, cell.bg };

			return cell;
		};
		Draw::Vertical(charFn, pos - Vec2i(1, 0), size[1] + 1);
	}
	//TODO: Maybe fix triple wide?

	return Draw::Border(m_border, pos, size, m_borderFlags);
}

void BorderItem::SetBorders(const std::array<TBChar, 8>& border)
{
	m_border = border;
}

const std::array<TBChar, 8>& BorderItem::GetBorder() const
{
	return m_border;
}

void BorderItem::SetBorderFlags(Draw::BorderFlag flags)
{
	m_borderFlags = flags;
}

Draw::BorderFlag BorderItem::GetBorderFlags() const
{
	return m_borderFlags;
}

void BorderItem::SetBackgroundColor(Color bg)
{
	for (auto& c : m_border)
		c.s.bg = bg;
}

void BorderItem::SetForegroundColor(Color fg)
{
	for (auto& c : m_border)
		c.s.fg = fg;
}

void BorderItem::SetFixDoubleWide(bool fix)
{
	m_fixDoubleWide = fix;
}

std::pair<Vec2i, Vec2i> BorderItem::GetInteriorSpace(Vec2i pos, Vec2i size) const
{
	Vec2i poff{ 0, 0 };
	Vec2i soff{ 1, 1 };
	if (m_borderFlags & Draw::BorderFlag::Top)
	{
		poff += Vec2i{ 0, 1 };
		soff += Vec2i{ 0, -1 };
	}
	if (m_borderFlags & Draw::BorderFlag::Right)
	{
		soff += Vec2i{ -1, 0 };
	}
	if (m_borderFlags & Draw::BorderFlag::Bottom)
	{
		soff += Vec2i{ 0, -1 };
	}
	if (m_borderFlags & Draw::BorderFlag::Left)
	{
		poff += Vec2i{ 1, 0 };
		soff += Vec2i{ -1, 0 };
	}

	return { pos + poff, size + soff };
}
// }}}

// {{{ Widget
Widget::Widget(Window* parent):
	m_parent(parent),
	m_visible(true),
	m_active(true),
	m_pos(0, 0),
	m_size(0, 0),
	m_trailingChar(Settings::trailing_character, Settings::default_text_style)
{
}

Widget::~Widget()
{
}

void Widget::SetPosition(Vec2i pos)
{
	OnSetPosition.Notify<EventWhen::BEFORE>(pos);
	m_pos = pos;
	OnSetPosition.Notify<EventWhen::AFTER>(pos);
}

Vec2i& Widget::GetPosition()
{
	return m_pos;
}

const Vec2i& Widget::GetPosition() const
{
	return m_pos;
}

void Widget::SetSize(Vec2i size)
{
	OnSetSize.Notify<EventWhen::BEFORE>(size);
	m_size = size;
	OnSetSize.Notify<EventWhen::AFTER>(size);
}

Vec2i& Widget::GetSize()
{
	return m_size;
}

const Vec2i& Widget::GetSize() const
{
	return m_size;
}

bool Widget::IsCorrect() const
{
	if (!m_parent)
		return true;

	const Vec2i size = ((Widget*)m_parent)->GetSize();
	const Vec2i c1 = m_pos;
	const Vec2i c2 = m_pos + m_size;

	//TODO: BORDERS
	if (c1[0] < 0 || c1[1] < 0)
		return false;
	if (c2[0] + 1 >= size[0] || c2[1] + 1 >= size[1])
		return false;

	return true;
}

void Widget::SetVisible(bool v)
{
	OnSetVisible.Notify<EventWhen::BEFORE>(v);
	m_visible = v;
	OnSetVisible.Notify<EventWhen::AFTER>(v);
}

bool Widget::IsVisible() const
{
	return m_visible;
}

void Widget::SetActive(bool v)
{
	OnSetActive.Notify<EventWhen::BEFORE>(v);
	m_active = v;
	OnSetActive.Notify<EventWhen::AFTER>(v);
}

bool Widget::IsActive() const
{
	return m_active;
}

void Widget::SetProcessTimed(bool v, bool wait)
{
	OnSetProcessTimed.Notify<EventWhen::BEFORE>(v);
	if (v)
	{
		StartEvents(*this);
	}
	else
	{
		StopEvents(wait);
	}
	OnSetProcessTimed.Notify<EventWhen::AFTER>(v);
}

bool Widget::IsProcessingTimed() const
{
	return EventsRunning();
}
// }}}

// {{{ TextLine
void Widgets::TextLine::Draw()
{
	const auto p = Draw::TextLine(m_line, GetPosition(), GetSize()[0], Widget::m_trailingChar).first;
	Draw::Horizontal(m_bg, GetPosition()+Vec2i(p, 0), GetSize()[0]-p);
}

Widgets::TextLine::TextLine(Window* win, const TBString& line):
	Widget::Widget(win),
	m_line(line),
	m_bg(U' ', Settings::default_text_style)
{
}

void Widgets::TextLine::SetLine(const TBString& line)
{
	m_line = line;
}

const TBString& Widgets::TextLine::GetLine() const
{
	return m_line;
}


void Widgets::TextLine::SetBackground(const TBChar& bg)
{
	m_bg = bg;
}

const TBChar& Widgets::TextLine::GetBackground() const
{
	return m_bg;
}
// }}}

// {{{ Button
void Widgets::Button::Draw()
{
	auto&& [pos, size] = BorderItem::Draw(GetPosition(), GetSize());

	Draw::Rectangle(m_bg, pos, size);

	Draw::TextBox(m_text, pos, size, m_trailingChar);
}

Widgets::Button::Button(Window* win, const TBString& text)
: Widget::Widget(win), m_text(text)
{
	m_bg = { Settings::fill_character, { COLOR_DEFAULT, COLOR_DEFAULT, TextStyle::None } };
}

void Widgets::Button::SetText(const TBString& text)
{
	m_text = text;
}

const TBString& Widgets::Button::GetText() const
{
	return m_text;
}

void Widgets::Button::SetBackground(const TBChar& bg)
{
	m_bg = bg;
}
const TBChar& Widgets::Button::GetBackground() const
{
	return m_bg;
}
// }}}

// {{{ InputLine
void Widgets::InputLine::Draw()
{
	// Debug
	if constexpr (true)
	{
		auto p = Draw::TextLine( Util::StringConvert<Char>(std::to_string(m_position)),  m_textStyle, {1,1}, 10, m_trailingChar).first;
		Draw::Horizontal(m_bg, Vec2i(1,1) + Vec2i(p, 0), 10 - p);
		p = Draw::TextLine( Util::StringConvert<Char>(std::to_string(m_cursor)),  m_textStyle, {1,2}, 10, m_trailingChar).first;
		Draw::Horizontal(m_bg, Vec2i(1,2) + Vec2i(p, 0), 10 - p);
		p = Draw::TextLine( Util::StringConvert<Char>(std::to_string(m_textOffset)),  m_textStyle, {1,3}, 10, m_trailingChar).first;
		Draw::Horizontal(m_bg, Vec2i(1,3) + Vec2i(p, 0), 10 - p);
		p = Draw::TextLine( Util::StringConvert<Char>(std::to_string(m_leftScroll)),  m_textStyle, {1,4}, 10, m_trailingChar).first;
		Draw::Horizontal(m_bg, Vec2i(1,4) + Vec2i(p, 0), 10 - p);
	}

	//Todo : draw horizontal with fill char
	Draw::Horizontal({m_trailingChar.ch, m_bg.s}, GetPosition(), m_leftScroll);
	const auto p = Draw::TextLine(m_text, m_textStyle, GetPosition()+Vec2i(m_leftScroll, 0), GetSize()[0]-m_leftScroll, m_trailingChar, m_textOffset).first;
	Draw::Horizontal(m_bg, GetPosition() + Vec2i(p+m_leftScroll, 0), GetSize()[0] - p - m_leftScroll);

	Termbox::SetCursor(GetPosition() + Vec2i(static_cast<int>(m_cursor), 0));
}

Widgets::InputLine::InputLine(const String& text, std::size_t position)
:	m_text(text),
	m_position(position),
	m_cursor(0),
	m_leftScroll(0),
	m_rightScroll(0),
	m_textOffset(0),
	m_bg('-', { 0x777777, 0x000000, TextStyle::None }),
	m_textStyle(Settings::default_text_style),
	m_maxSize(0)
{
	// Note: it works but has some kind of flicker with Wide chars (though, just visual)
	const auto Left = [&](Termbox& tb) {
		if (m_position == 0) [[unlikely]]
			return;
		--m_position;

		if (m_position+1 == m_text.size()) [[unlikely]]
			--m_cursor;
		else [[likely]]
			m_cursor -= wcwidth(m_text[m_position]);

		const int trigger = GetSize()[0] * m_settings.ScrollTriggerLeft / 100;
		if (m_cursor <= trigger && m_textOffset != 0) [[unlikely]]
		{
			if (m_leftScroll != 0) [[unlikely]]
			{
				m_cursor += wcwidth(m_text[m_position]);
				m_leftScroll -= wcwidth(m_text[m_textOffset])-1;
			}
			else [[likely]]
			{
				m_cursor += wcwidth(m_text[m_position]);
			}
			--m_textOffset;
		}
	};

	const auto Right = [&](Termbox& tb) {
		if (m_position >= m_text.size()) [[unlikely]]
			return;
		++m_position;

		if (m_position == m_text.size()) [[unlikely]]
			++m_cursor;
		else [[likely]]
			m_cursor += wcwidth(m_text[m_position-1]);

		const int trigger = 10;//m_dim[0] * (100-m_settings.ScrollTriggerRight) / 100;
		if (m_cursor+2 >= trigger) [[unlikely]]
		{
			if (m_leftScroll != 0)
			{
				m_cursor -= wcwidth(m_text[m_position-1]);
				--m_leftScroll;
			}
			else
			{
				m_leftScroll += wcwidth(m_text[m_textOffset])-1;
				if (wcwidth(m_text[m_position])-1 <= m_leftScroll)
					m_leftScroll -= wcwidth(m_text[m_position])-1;
				m_cursor = trigger-2;

				switch (wcwidth(m_text[m_position]))
				{
					case 1:
						m_textOffset += (m_cursor+1 == trigger ? 0 : 1);
						break;
					case 2:
						switch (wcwidth(m_text[m_textOffset]))
						{
							case 1:
								m_textOffset += 2; // Will always do event is the char at m_textOffset+1 is of width 2
								break;
							case 2:
								m_textOffset += 1;
								break;
						}
						break;
				}
			}
		}
	};

	AddKeyboardInput({U"LEFT", Left});
	AddKeyboardInput({U"C-LEFT", [&](Termbox& tb) {

	}});
	AddKeyboardInput({U"RIGHT", Right});
	AddKeyboardInput({U"C-RIGHT", [&](Termbox& tb) {

	}});
	AddKeyboardInput({U"UP", [&](Termbox& tb) {
		m_position = m_cursor = m_textOffset = m_leftScroll = 0;
	}});
	AddKeyboardInput({U"DOWN", [&](Termbox& tb) {

	}});
	AddKeyboardInput({U"BACKSPACE", [&](Termbox& tb) {
		if (m_position == 0) [[unlikely]]
			return;

		--m_position;
		m_cursor -= wcwidth(m_text[m_position]);
		m_text.erase(m_position, 1);

		const int trigger = GetSize()[0] * m_settings.ScrollTriggerLeft / 100;
		if (m_cursor <= trigger && m_textOffset != 0) [[unlikely]]
		{
			if (m_leftScroll != 0)
			{
				++m_cursor;
				m_leftScroll -= wcwidth(m_text[m_textOffset])-1;
			}
			else
			{
				m_cursor += wcwidth(m_text[m_textOffset-1]);
			}
				--m_textOffset;
		}
	}});

	AddKeyboardInput({U"ENTER", [&](Termbox& tb) {
	}});

	AddKeyboardInput({U"ESC", [&](Termbox& tb) {
		SetActive(false);
		SetBackground(TBChar(U'#', {0xFF0000, 0xFFFFFF, TextStyle::Underline}));
	}});

	AddKeyboardInput({U"#SCHAR", [&, Right](Termbox& tb) {
		if (m_text.size() >= m_maxSize && m_maxSize != 0) [[unlikely]]
			return;
		m_text.insert(m_text.begin()+m_position, tb.GetContext().ev.ch);
		//Right(tb);
	}});

	//TODO: calculate offset depending on the cursor pos
	// m_cursor, m_textOffset = f{m_position, [[m_text]]};
}

void Widgets::InputLine::SetText(const String& text)
{
	m_text = text;
}

const String& Widgets::InputLine::GetText() const
{
	return m_text;
}

void Widgets::InputLine::SetTextPosition(std::size_t position)
{
	m_position = position;
}

std::size_t Widgets::InputLine::GetTextPosition() const
{
	return m_position;
}

void Widgets::InputLine::SetBackground(const TBChar& bg)
{
	m_bg = bg;
}

const TBChar& Widgets::InputLine::GetBackground() const
{
	return m_bg;
}

void Widgets::InputLine::SetTextStyle(const TBStyle& style)
{
	m_textStyle = style;
}

const TBStyle& Widgets::InputLine::GetTextStyle() const
{
	return m_textStyle;
}

void Widgets::InputLine::SetHistory(const std::deque<String>& history)
{
	m_history = history;
}

const std::deque<String>& Widgets::InputLine::GetHistory() const
{
	return m_history;
}

void Widgets::InputLine::SetSettings(const InputLine::InputLineSettings& settings)
{
	m_settings = settings;
	//TODO: perform some sanity check
}

const Widgets::InputLine::InputLineSettings& Widgets::InputLine::GetSettings() const
{
	return m_settings;
}

void Widgets::InputLine::SetMaxSize(std::size_t maxSize)
{
	if (maxSize < m_text.size())
		throw Util::Exception("Tried to change InputLine maxsize with a size smaller than the current string's size.");
	m_maxSize = maxSize;
}

std::size_t Widgets::InputLine::GetMaxSize() const
{
	return m_maxSize;
}
// }}}
