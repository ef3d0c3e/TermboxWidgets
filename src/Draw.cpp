#include "Draw.hpp"
#include "Termbox.hpp"

static bool validCoordinates(Vec2i pos)
{
	const auto& [x, y] = pos;

	if (x < 0 || x >= tb_width())
		return false;
	if (y < 0 || y >= tb_height())
		return false;

	return true;
}

std::pair<Vec2i, Vec2i> Draw::Border(const std::array<TBChar, 8>& border, Vec2i pos, Vec2i size, Draw::BorderFlag flags)
{
	const auto& [w, h] = size;

	if (flags & Draw::BorderFlag::TopLeft)
		Draw::Char(border[0], pos);
	if (flags & Draw::BorderFlag::TopRight)
		Draw::Char(border[1], pos + Vec2i{ w, 0 });
	if (flags & Draw::BorderFlag::BottomRight)
		Draw::Char(border[2], pos + Vec2i{ w, h });
	if (flags & Draw::BorderFlag::BottomLeft)
		Draw::Char(border[3], pos + Vec2i{ 0, h });
	if (flags & Draw::BorderFlag::Top)
		Draw::Horizontal(border[4], pos + Vec2i{ 1, 0 }, w - 1);
	if (flags & Draw::BorderFlag::Right)
		Draw::Vertical(border[5], pos + Vec2i{ w, 1 }, h - 1);
	if (flags & Draw::BorderFlag::Bottom)
		Draw::Horizontal(border[6], pos + Vec2i{ 1, h }, w - 1);
	if (flags & Draw::BorderFlag::Left)
		Draw::Vertical(border[7], pos + Vec2i{ 0, 1 }, h - 1);

	Vec2i poff{ 0, 0 };
	Vec2i soff{ 1, 1 };
	if (flags & Draw::BorderFlag::Top)
	{
		poff += Vec2i{ 0, 1 };
		soff += Vec2i{ 0, -1 };
	}
	if (flags & Draw::BorderFlag::Right)
	{
		soff += Vec2i{ -1, 0 };
	}
	if (flags & Draw::BorderFlag::Bottom)
	{
		soff += Vec2i{ 0, -1 };
	}
	if (flags & Draw::BorderFlag::Left)
	{
		poff += Vec2i{ 1, 0 };
		soff += Vec2i{ -1, 0 };
	}

	return { pos + poff, size + soff };
}

void Draw::Char(const TBChar& c, Vec2i pos)
{
	const auto& [x, y] = pos;
	const auto cell = c();
	tb_char(x, y, cell.fg, cell.bg, cell.ch);
}

void Draw::Horizontal(const TBChar& c, Vec2i pos, int w)
{
	if (!validCoordinates(pos))
		return;

	const auto& [x, y] = pos;
	const auto cell = c();
	const auto width = wcwidth(cell.ch);
	w = std::max(w, 0);

	for (int i = 0; i < w; ++i)
		tb_char(x + i, y, cell.fg, cell.bg, (i % width || i + width > w) ? U' ' : cell.ch);
}

void Draw::Vertical(const TBChar& c, Vec2i pos, int h)
{
	const auto& [x, y] = pos;
	const auto cell = c();
	h = std::max(h, 0);

	for (int i = 0; i < h; ++i)
		tb_char(x, y + i, cell.fg, cell.bg, cell.ch);
}

void Draw::Vertical(std::function<struct tb_cell(const struct tb_cell&, Vec2i pos)> charFn, Vec2i pos, int h)
{

	const auto& [x, y] = pos;
	if (static_cast<unsigned int>(x) >= static_cast<unsigned int>(tb_width()))
		return;
	if (static_cast<unsigned int>(y) >= static_cast<unsigned int>(tb_height()))
		return;
	h = std::max(h, 0);

	for (int i = 0; i < h; ++i)
	{
		Vec2i p = { x, y + i };
		auto cell = charFn(Termbox::At(p), p);
		tb_cell(x, y + i, &cell);
	}
}

std::pair<int, std::size_t> Draw::TextLine(const TBString& s, Vec2i pos, int w, const TBChar& trailing, std::size_t beg)
{
	const auto& [x, y] = pos;
	std::size_t i = beg;
	int p = 0;

	do
	{
		const auto cell = s[i]();
		int glyph_size = wcwidth(cell.ch);
		if (p + glyph_size > w)
		{
			if (trailing.ch == U'\0')
				break;
			const auto cell = trailing();
			if (i != 0)
				p -= wcwidth(s[i - 1].ch);
			tb_cell(x + p, y, &cell);
			++p;
			break;
		}
		tb_cell(x + p, y, &cell);
		p += glyph_size;
	} while (++i < s.Size());

	return { p, i };
}

std::pair<int, std::size_t> Draw::TextLineStyle(const TBString& s, TextStyle textstyle, Vec2i pos, int w, const TBChar& trailing, std::size_t beg)
{
	const auto& [x, y] = pos;
	std::size_t i = beg;
	int p = 0;

	do
	{
		TBChar tbc = s[i];
		tbc.s.s = static_cast<std::uint32_t>(tbc.s.s) | textstyle;
		const auto cell = tbc();
		int glyph_size = wcwidth(cell.ch);
		if (p + glyph_size > w)
		{
			if (trailing.ch == U'\0')
				break;
			const auto cell = trailing();
			if (i != 0)
				p -= wcwidth(s[i - 1].ch);
			tb_cell(x + p, y, &cell);
			++p;
			break;
		}
		tb_cell(x + p, y, &cell);
		p += glyph_size;
	} while (++i < s.Size());

	return { p, i };
}

std::pair<int, std::size_t> Draw::TextLineBackground(const TBString& s, Color bg, Vec2i pos, int w, const TBChar& trailing, std::size_t beg)
{
	const auto& [x, y] = pos;
	std::size_t i = beg;
	int p = 0;

	do
	{
		TBChar tbc = s[i];
		tbc.s.bg = bg;
		const auto cell = tbc();
		int glyph_size = wcwidth(cell.ch);
		if (p + glyph_size > w)
		{
			if (trailing.ch == U'\0')
				break;
			const auto cell = trailing();
			if (i != 0)
				p -= wcwidth(s[i - 1].ch);
			tb_cell(x + p, y, &cell);
			++p;
			break;
		}
		tb_cell(x + p, y, &cell);
		p += glyph_size;
	} while (++i < s.Size());

	return { p, i };
}

std::pair<int, std::size_t> Draw::TextLine(const String& s, const TBStyle& style, Vec2i pos, int w, const TBChar& trailing, std::size_t beg)
{
	const auto& [x, y] = pos;
	std::size_t i = beg;
	int p = 0;

	do
	{
		const auto cell = TBChar(s[i], style)();
		int glyph_size = wcwidth(cell.ch);
		if (p + glyph_size > w)
		{
			if (trailing.ch == U'\0')
				break;
			const auto cell = trailing();
			if (i != 0)
				p -= wcwidth(s[i - 1]);
			tb_cell(x + p, y, &cell);
			++p;
			break;
		}
		tb_cell(x + p, y, &cell);
		p += glyph_size;
	} while (++i < s.size());

	return { p, i };
}

std::pair<Vec2i, std::size_t> Draw::TextBox(const TBString& s, Vec2i pos, Vec2i dim, const TBChar& trailing)
{
	//TODO
	const auto& [x, y] = pos;
	const auto& [w, h] = dim;
	std::size_t i = 0;
	int j = 0;
	while (j < h && i < s.Size() - 1)
	{
		auto&& [w_, i_] = TextLine(s, { x, y + j }, w, { U'\0', trailing.s }, i);
		i = i_;
		++j;
	}

	return { { x, y + j }, i };
}

void Draw::Rectangle(const TBChar& c, Vec2i pos, Vec2i size)
{
	const auto& [x, y] = pos;
	const auto& [w, h] = size;
	const auto cell = c();
	const auto width = wcwidth(cell.ch);

	for (int i = 0; i < w; ++i)
		for (int j = 0; j < h; ++j)
			tb_char(x + i, y + j, cell.fg, cell.bg, (i % width || i + width > w) ? U' ' : cell.ch);
}
