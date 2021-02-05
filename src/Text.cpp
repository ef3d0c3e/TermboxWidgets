#include "Text.hpp"
#include <type_traits>

void Color::SetColor(std::uint32_t color)
{
	m_color = color;
}

std::uint32_t Color::GetColor(std::uint32_t color) const
{
	return m_color;
}

tb_color Color::operator()() const
{
	/*
	if (m_color == TB_DEFAULT)
		return m_color;
	switch (s_mode)
	{
		case Color::COLORS_8:
			return (m_color >> 16 & 0x80) | (m_color >> 8 & 0x80) << 1 |
				   (m_color & 0x80) << 2;
		case Color::COLORS_16:
			return (((m_color >> 16 & 0xFF) | (m_color >> 8 & 0xFF) | (m_color & 0xFF)) ?
						0b1000 :
						0b0000) |
				   (m_color >> 16 & 0x80) | (m_color >> 8 & 0x80) << 1 |
				   (m_color & 0x80) << 2;
		case Color::COLORS_256: {
			std::uint8_t shade = (m_color >> 4 & 0x18);
			// Grayscale
			if ((m_color >> 20 & 0x18) == shade && (m_color >> 12 & 0x18) == shade)
				return 255 - shade;
			// 6x6x6 RGB cube
			return (m_color >> 21 & 0x06) * 36 + (m_color >> 13 & 0x06) * 6 +
				   (m_color >> 5 & 0x06);
		}
		case Color::COLORS_TRUECOLOR:
			return m_color;
	}
	*/
	return m_color;
}

void Color::SetMode(Color::COLOR_MODE mode)
{
	s_mode = mode;
	tb_select_output_mode(mode);
}

const Color::COLOR_MODE& Color::GetMode()
{
	return s_mode;
}

TBStyle& TBStyle::operator=(const TBStyle& s)
{
	this->fg = s.fg;
	this->bg = s.bg;
	this->s = s.s;
	return *this;
}

TBChar& TBChar::operator=(const TBChar& c)
{
	ch = c.ch;
	s = c.s;
	return *this;
}


struct tb_cell TBChar::operator()() const
{
	struct tb_cell c;
	c.ch = ch;
	c.fg = s.fg() | (s.s << 24);
	c.bg = s.bg();
	return c;
}

TBString::TBString(const TBChar* a, std::size_t size)
{
	m_string = std::vector<TBChar>(size, TBChar());
	for (std::size_t i = 0; i < size; ++i)
		m_string[i] = a[i];
}

TBString::TBString(const Char* str, TBStyle s)
{
	std::size_t size = Util::Strlen(str);
	m_string = std::vector<TBChar>(size, TBChar(0, s));
	for (std::size_t i = 0; i < size; ++i)
		m_string[i].ch = str[i];
}

TBString::TBString(TBChar c, std::size_t size)
{
	m_string = std::vector<TBChar>(size, c);
}

TBString::TBString(const String& str, TBStyle s)
{
	std::size_t size = str.size();
	m_string = std::vector<TBChar>(size, TBChar(0, s));
	for (std::size_t i = 0; i < size; ++i)
		m_string[i].ch = str[i];
}

TBString::TBString(const TBString& s)
{
	m_string = s.m_string;
}

TBString::TBString(TBString&& s)
{
	m_string = std::move(s.m_string);
}

TBString::~TBString()
{
	m_string.clear();
}

TBString& TBString::operator=(const TBString& s)
{
	m_string = s.m_string;
	return *this;
}

const TBChar& TBString::operator[](std::size_t i) const
{
	return m_string[i];
}

TBChar& TBString::operator[](std::size_t i)
{
	return m_string[i];
}

std::size_t TBString::Size() const
{
	return m_string.size();
}

std::size_t TBString::SizeWide() const
{
	std::size_t width = 0;
	for (const auto& c : m_string)
		width += wcwidth(c.ch);

	return width;
}

void TBString::Clear()
{
	m_string.clear();
}

String TBString::Str() const
{
	String r(Size(), U' ');

	for (std::size_t i = 0; i < Size(); ++i)
		r[i] = m_string[i].ch;

	return r;
}
