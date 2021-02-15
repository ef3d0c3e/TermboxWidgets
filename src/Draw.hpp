#ifndef TERMBOXWIDGETS_DRAW_HPP
#define TERMBOXWIDGETS_DRAW_HPP

#include "Settings.hpp"
////////////////////////////////////////////////
/// \brief Low level drawing primitives
/// \ingroup Records
////////////////////////////////////////////////
namespace Draw
{
/** @cond */
MAKE_CENUM_Q(BorderFlag, std::uint8_t,
	None        , 0b00000000,
	Top         , 0b00000001,
	Right       , 0b00000010,
	Bottom      , 0b00000100,
	Left        , 0b00001000,
	TopLeft     , 0b00010000,
	TopRight    , 0b00100000,
	BottomRight , 0b01000000,
	BottomLeft  , 0b10000000,
	Corners     , 0b11110000,
	Borders     , 0b00001111,
	All         , Corners | Borders)
/** @endcond */

////////////////////////////////////////////////
/// \brief Draw a border
///
/// \param border The border, see BorderFlag for information on the border
/// \param pos The position of the border
/// \param size The size of the border
/// \param flags The draw flags of the border
/// \returns A rectangle corresponding the the border interior's bounds
///
/// \note The returned rectangle will ignore the corners and may overlap with them
////////////////////////////////////////////////
std::pair<Vec2i, Vec2i> Border(const std::array<TBChar, 8>& border, Vec2i pos, Vec2i size, BorderFlag flags = BorderFlag::All);
////////////////////////////////////////////////
/// \brief Draw a single TBChar
///
/// \param c The TBChar to draw
/// \param pos The position of the char
////////////////////////////////////////////////
void Char(const TBChar& c, Vec2i pos);
////////////////////////////////////////////////
/// \brief Draw a horizontal line
///
/// \param c The TBChar to fill the line with
/// \param pos The begining position of the line
/// \param w The width of the line
////////////////////////////////////////////////
void Horizontal(const TBChar& c, Vec2i pos, int w);
////////////////////////////////////////////////
/// \brief Draw a vercital line
///
/// \param c The TBChar to fill the line with
/// \param pos The begining position of the line
/// \param h The height of the line
////////////////////////////////////////////////
void Vertical(const TBChar& c, Vec2i pos, int h);
////////////////////////////////////////////////
/// \brief Draw a vertical line
///
/// \param charFn A function that will return the character to be displayed
/// \param pos The begining position of the line
/// \param h The height of the line
////////////////////////////////////////////////
void Vertical(std::function<struct tb_cell(const struct tb_cell&, Vec2i pos)> charFn, Vec2i pos, int h);
////////////////////////////////////////////////
/// \brief Draw text on a single line
///
/// \param s The TBString to draw
/// \param pos The begining position of the text
/// \param w The maximum width of the line
/// \param trailing The character to indicate that the line was too long
/// \param beg The begining position in the string
/// \returns The width of the drawn line and the index of the last character drawn
///
/// \note Passing ```U'\0'``` as a trailing char, will cause it to ignore it and write text until no space is left
////////////////////////////////////////////////
std::pair<int, std::size_t> TextLine(const TBString& s, Vec2i pos, int w, const TBChar& trailing, std::size_t beg = 0ul);
////////////////////////////////////////////////
/// \brief Draw text on a single line
///
/// \param s The TBString to draw
/// \param textstyle Additional text style
/// \param pos The begining position of the text
/// \param w The maximum width of the line
/// \param trailing The character to indicate that the line was too long
/// \param beg The begining position in the string
/// \returns The width of the drawn line and the index of the last character drawn
///
/// \note Passing ```U'\0'``` as a trailing char, will cause it to ignore it and write text until no space is left
////////////////////////////////////////////////
std::pair<int, std::size_t> TextLine(const TBString& s, TextStyle textstyle, Vec2i pos, int w, const TBChar& trailing, std::size_t beg = 0ul);
////////////////////////////////////////////////
/// \brief Draw text on a single line
///
/// \param s The String to draw
/// \param style The style of the string
/// \param pos The begining position of the text
/// \param w The maximum width of the line
/// \param trailing The character to indicate that the line was too long
/// \param beg The begining position in the string
/// \returns The width of the drawn line and the index of the last character drawn
///
/// \note Passing ```U'\0'``` as a trailing char, will cause it to ignore it and write text until no space is left
////////////////////////////////////////////////
std::pair<int, std::size_t> TextLine(const String& s, const TBStyle& style, Vec2i pos, int w, const TBChar& trailing, std::size_t beg = 0ul);
////////////////////////////////////////////////
/// \brief Draw text in a box
///
/// \param s The TBString to draw
/// \param pos The begining position of the text
/// \param dim The dimension of the pox
/// \param trailing The character to indicate that the line was too long
/// \returns The position of the end of the text and the index of the last drawn character
////////////////////////////////////////////////
std::pair<Vec2i, std::size_t> TextBox(const TBString& s, Vec2i pos, Vec2i dim, const TBChar& trailing);
////////////////////////////////////////////////
/// \brief Draw a rectangle
///
/// \param c The TBChar to fill the rectangle with
/// \param pos The begining position of the rectangle
/// \param size The sizes of the rectangle
////////////////////////////////////////////////
void Rectangle(const TBChar& c, Vec2i pos, Vec2i size);
}

#endif // TERMBOXWIDGETS_DRAW_HPP
