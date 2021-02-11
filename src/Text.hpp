#ifndef TERMBOXWIDGETS_TEXT_HPP
#define TERMBOXWIDGETS_TEXT_HPP

#include "Util.hpp"
#include "termbox/src/termbox.h"

/** @cond */
MAKE_CENUM_Q(TextStyle, int,
	None,      0,
	Bold,      1 << 0,
	Underline, 1 << 1,
	Reverse,   1 << 2,
	Italic,    1 << 3,
	Strike,    1 << 4
)
/** @endcond */


////////////////////////////////////////////////
/// \brief Class that holds informations about
///  a color
////////////////////////////////////////////////
/* TODO: Allow low-level color setting in termbox */
class Color
{
public:
	////////////////////////////////////////////////
	/// \brief Mode for color output
	////////////////////////////////////////////////
	MAKE_CENUMV_Q(COLOR_MODE, std::uint8_t,
		COLORS_8, TB_OUTPUT_NORMAL, ///< 8 Colors (TTY)
		///< \see
		///< https://en.wikipedia.org/wiki/ANSI_escape_code#3-bit_and_4-bit
		COLORS_256, TB_OUTPUT_256, ///< 256 Colors: 6x6x6 RGB Colors + 16 base colors + 24 Shades of
		///< gray (Most terminals)
		/// \see https://en.wikipedia.org/wiki/ANSI_escape_code#8-bit
		COLORS_TRUECOLOR, TB_OUTPUT_TRUECOLOR, ///< 2^32 Colors (Some terminals)
		///< \see https://en.wikipedia.org/wiki/ANSI_escape_code#24-bit
	)

private:
	inline static COLOR_MODE s_mode = COLORS_8;

	std::uint32_t m_color;

public:
	////////////////////////////////////////////////
	/// \brief Constructor
	///
	/// \param color The color in 24 bit RGB
	////////////////////////////////////////////////
	constexpr Color(std::uint32_t color = 0)
	{
		m_color = color;
	}

	////////////////////////////////////////////////
	/// \brief Copy constructor
	////////////////////////////////////////////////
	constexpr Color(const Color& c)
	{
		m_color = c.m_color;
	}


	////////////////////////////////////////////////
	/// \brief Set the current color
	///
	/// \param color The color in 24 bit RGB
	////////////////////////////////////////////////
	void SetColor(std::uint32_t color);

	////////////////////////////////////////////////
	/// \brief Get the current color
	///
	/// \returns The current color in 24bits RGB
	////////////////////////////////////////////////
	std::uint32_t GetColor(std::uint32_t color) const;


	////////////////////////////////////////////////
	/// \brief Get the color in the current mode
	///
	/// \returns a termbox-compatible color for the
	///  current display mode
	////////////////////////////////////////////////
	tb_color operator()() const;


	////////////////////////////////////////////////
	/// \brief Mode used for outputting
	////////////////////////////////////////////////
	static void SetMode(COLOR_MODE mode);

	////////////////////////////////////////////////
	/// \brief Returns the current mode
	////////////////////////////////////////////////
	const static COLOR_MODE& GetMode();
};
constexpr Color COLOR_DEFAULT(TB_DEFAULT);

////////////////////////////////////////////////
/// \brief Class that holds style for a single character
/// \see Color
/// \see TextStyle
////////////////////////////////////////////////
struct TBStyle
{
	Color fg, bg;
	TextStyle s = TextStyle::None;

	////////////////////////////////////////////////
	/// \brief Default constructor
	////////////////////////////////////////////////
	constexpr TBStyle()
	{
		fg = Color();
		bg = Color();
	}

	////////////////////////////////////////////////
	/// \brief Copy constructor
	////////////////////////////////////////////////
	constexpr TBStyle(const TBStyle& tbs)
	{
		fg = tbs.fg;
		bg = tbs.bg;
		s = tbs.s;
	}

	////////////////////////////////////////////////
	/// \brief Constructor
	///
	/// \param fg The foreground color
	/// \param bg The background color
	/// \param s The style
	/// \see Style
	////////////////////////////////////////////////
	constexpr TBStyle(Color fg, Color bg, TextStyle s)
	{
		this->fg = fg;
		this->bg = bg;
		this->s = s;
	}

	////////////////////////////////////////////////
	/// \brief Assign operator
	////////////////////////////////////////////////
	constexpr TBStyle& operator=(const TBStyle& s)
	{
		this->fg = s.fg;
		this->bg = s.bg;
		this->s = s.s;
		return *this;
	}
};

////////////////////////////////////////////////
/// \brief Class that holds a single character
/// \see Char
/// \see TBStyle
////////////////////////////////////////////////
struct TBChar
{
	Char ch;
	TBStyle s;

	////////////////////////////////////////////////
	/// \brief Default constructor
	////////////////////////////////////////////////
	constexpr TBChar()
	{
		ch = 0;
		s = TBStyle();
	}

	////////////////////////////////////////////////
	/// \brief Copy constructor
	////////////////////////////////////////////////
	constexpr TBChar(const TBChar& c)
	{
		ch = c.ch;
		s = c.s;
	}

	////////////////////////////////////////////////
	/// \brief Move constructor
	////////////////////////////////////////////////
	constexpr TBChar(TBChar&& c)
	{
		ch = std::move(c.ch);
		s = std::move(c.s);
	}

	////////////////////////////////////////////////
	/// \brief Constructor
	///
	/// \param ch The character
	/// \param fg The character's foreground color
	/// \param bg The character's background color
	/// \param s The character's style
	/// \see TBStyle
	////////////////////////////////////////////////
	constexpr TBChar(Char ch, Color fg, Color bg, TextStyle s = TextStyle::None)
	{
		this->ch = ch;
		this->s = TBStyle(fg, bg, s);
	}

	////////////////////////////////////////////////
	/// \brief Constructor
	///
	/// \param ch The character
	/// \param s The style
	/// \see TBStyle
	////////////////////////////////////////////////
	constexpr TBChar(Char ch, TBStyle s)
	{
		this->ch = ch;
		this->s = s;
	}

	////////////////////////////////////////////////
	/// \brief Assign operator
	////////////////////////////////////////////////
	TBChar& operator=(const TBChar& c);

	////////////////////////////////////////////////
	/// \brief Get the character ready for printing
	///
	/// \returns a termbox-compatible cell
	////////////////////////////////////////////////
	struct tb_cell operator()() const;
};


////////////////////////////////////////////////
/// \brief Class that holds a string of TBChar
/// \see TBChar
////////////////////////////////////////////////
class TBString
{
	std::vector<TBChar> m_string;

public:
	////////////////////////////////////////////////
	/// \brief Array constructor
	///
	/// \param a An array or TBChar
	/// \param size The size of the array
	////////////////////////////////////////////////
	TBString(const TBChar* a, std::size_t size);
	////////////////////////////////////////////////
	/// \brief String constructor
	///
	/// \param str A null-terminated string of Char
	/// \param s Text style
	////////////////////////////////////////////////
	TBString(const Char* str, TBStyle s);

	////////////////////////////////////////////////
	/// \brief Char constructor
	///
	/// \param c The character to fill the string with
	/// \param size The size of the string
	////////////////////////////////////////////////
	TBString(TBChar c, std::size_t size);

	////////////////////////////////////////////////
	/// \brief String constructor
	///
	/// \param str A String
	/// \param s Text style
	/// \see String
	////////////////////////////////////////////////
	TBString(const String& str, TBStyle s);

	////////////////////////////////////////////////
	/// \brief Copy constructor
	////////////////////////////////////////////////
	TBString(const TBString& s);

	////////////////////////////////////////////////
	/// \brief Move constructor
	////////////////////////////////////////////////
	TBString(TBString&& s);

	////////////////////////////////////////////////
	/// \brief Destructor
	////////////////////////////////////////////////
	~TBString();

	////////////////////////////////////////////////
	/// \brief Assign operator
	////////////////////////////////////////////////
	TBString& operator=(const TBString& s);

	////////////////////////////////////////////////
	/// \brief Const subscript operator
	/// \param i The index
	///
	/// \returns The TBChar at position i
	/// \warn Performs no bound checking.
	////////////////////////////////////////////////
	const TBChar& operator[](std::size_t i) const;
	////////////////////////////////////////////////
	/// \brief Subscript operator
	/// \param i The index
	///
	/// \returns A reference to the TBChar at position i
	/// \warn Performs no bound checking.
	////////////////////////////////////////////////
	TBChar& operator[](std::size_t i);


	auto cbegin() const
	{
		return m_string.cbegin();
	}

	auto begin()
	{
		return m_string.begin();
	}

	auto cend() const
	{
		return m_string.cend();
	}

	auto end()
	{
		return m_string.end();
	}

	auto crbegin() const
	{
		return m_string.crbegin();
	}

	auto rbegin()
	{
		return m_string.rbegin();
	}

	auto crend() const
	{
		return m_string.crend();
	}

	auto rend()
	{
		return m_string.rend();
	}

	////////////////////////////////////////////////
	/// \brief Size of the string
	///
	/// \returns The size of the string
	////////////////////////////////////////////////
	std::size_t Size() const;

	////////////////////////////////////////////////
	/// \brief Size of the string (in cell)
	///
	/// \returns The size of the string in cell
	////////////////////////////////////////////////
	std::size_t SizeWide() const;
	
	////////////////////////////////////////////////
	/// \brief Clear the string's content
	////////////////////////////////////////////////
	void Clear();

	////////////////////////////////////////////////
	/// \brief String content
	///
	/// \returns The string's content
	////////////////////////////////////////////////
	String Str() const;
};

#endif // TERMBOXWIDGETS_TEXT_HPP
