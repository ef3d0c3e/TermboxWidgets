#ifndef TERMBOXWIDGETS_WIDGETS_HPP
#define TERMBOXWIDGETS_WIDGETS_HPP

#include "Draw.hpp"
#include "Termbox.hpp"
#include <deque>
class Window;

// {{{ BorderItem
class BorderItem
{
	std::array<TBChar, 8> m_border;
	Draw::BorderFlag m_borderFlags;
	bool m_fixDoubleWide;

public:
	BorderItem();

	////////////////////////////////////////////////
	/// \brief Draw the border on screen
	/// \returns A rectangle corresponding the the border interior's bounds
	///		(i.e: available space)
	/// \see Draw::Border
	////////////////////////////////////////////////
	std::pair<Vec2i, Vec2i> Draw(Vec2i pos, Vec2i size) const;

	////////////////////////////////////////////////
	/// \brief Set the border's character
	/// \param border The new border
	/// \see Settings::default_border
	////////////////////////////////////////////////
	void SetBorders(const std::array<TBChar, 8>& border);
	////////////////////////////////////////////////
	/// \brief Get the border
	/// \return The current border
	////////////////////////////////////////////////
	const std::array<TBChar, 8>& GetBorder() const;

	////////////////////////////////////////////////
	/// \brief Set the border flags
	/// \param flags The border drawing flags
	/// \see Draw::Border
	////////////////////////////////////////////////
	void SetBorderFlags(Draw::BorderFlag flags = Draw::BorderFlag::All);
	////////////////////////////////////////////////
	/// \brief Get the border flags
	/// \return The current border flags
	////////////////////////////////////////////////
	Draw::BorderFlag GetBorderFlags() const;

	////////////////////////////////////////////////
	/// \brief Set the border's background color
	/// \param bg The color
	////////////////////////////////////////////////
	void SetBackgroundColor(Color bg);

	////////////////////////////////////////////////
	/// \brief Set the border's foreground color
	/// \param fg The color
	////////////////////////////////////////////////
	void SetForegroundColor(Color fg);

	////////////////////////////////////////////////
	/// \brief Fix the double wide character issue
	/// This fixes an issue with wide character from
	/// another widget getting in the way of the border
	/// \param fix Set to true to fix the issue
	////////////////////////////////////////////////
	void SetFixDoubleWide(bool fix);

	////////////////////////////////////////////////
	/// \brief Get the available space of the window
	/// \returns A rectangle corresponding to the interior space
	////////////////////////////////////////////////
	std::pair<Vec2i, Vec2i> GetInteriorSpace(Vec2i pos, Vec2i size) const;
};
// }}}

// {{{ Widget
////////////////////////////////////////////////
/// \brief Abstract class for a widget
////////////////////////////////////////////////
class Widget : public Drawable, public KeyboardInput, public MouseInput, public TimedManager, public Resizeable
{
	Window* m_parent;
	bool m_visible;
	bool m_active;

	Vec2i m_pos;
	Vec2i m_size;
protected:
	TBChar m_trailingChar;

public:
	Widget(Window* parent = nullptr);
	virtual ~Widget() = 0;

	////////////////////////////////////////////////
	/// \brief Set the widget's position
	/// \param pos The widget's new position
	////////////////////////////////////////////////
	void SetPosition(Vec2i pos);
	EventListener<Vec2i> OnSetPosition;
	////////////////////////////////////////////////
	/// \brief Get the widget's position
	/// \return The widget's current position
	////////////////////////////////////////////////
	Vec2i& GetPosition();
	const Vec2i& GetPosition() const;

	////////////////////////////////////////////////
	/// \brief Set the widget's dimension
	/// \param pos The widget's new dimension
	////////////////////////////////////////////////
	void SetSize(Vec2i dim);
	EventListener<Vec2i> OnSetSize;
	////////////////////////////////////////////////
	/// \brief Get the widget's dimension
	/// \return The widget's current dimension
	////////////////////////////////////////////////
	Vec2i& GetSize();
	const Vec2i& GetSize() const;

	////////////////////////////////////////////////
	/// \brief Determines if the widget can fit on the window
	/// \return true if the widget can correctly fit on the window
	////////////////////////////////////////////////
	bool IsCorrect() const;

	////////////////////////////////////////////////
	/// \brief Set the widget to visibla/invisible
	/// An invisible widget is not shown
	/// \param v The value
	////////////////////////////////////////////////
	void SetVisible(bool v);
	EventListener<bool> OnSetVisible;

	////////////////////////////////////////////////
	/// \brief Returns wether or not the widget is visible
	/// \returns True if the widget is visible
	////////////////////////////////////////////////
	bool IsVisible() const;

	////////////////////////////////////////////////
	/// \brief Set the widget to active/inactive
	/// An inactive widget cannot process input
	/// \param v The value
	////////////////////////////////////////////////
	void SetActive(bool v);
	EventListener<bool> OnSetActive;

	////////////////////////////////////////////////
	/// \brief Returns wether or not the widget is active
	/// \returns True if the widget is active
	////////////////////////////////////////////////
	bool IsActive() const;

	////////////////////////////////////////////////
	/// \brief Set wether or not timed event will be processed
	/// \param v The value
	/// \param wait If true, will wait for all timed event to finish
	////////////////////////////////////////////////
	void SetProcessTimed(bool v, bool wait = true);
	EventListener<bool> OnSetProcessTimed;

	////////////////////////////////////////////////
	/// \brief Returns wether or not the widget is processing timed event
	/// \returns True if the widget is processing timed events
	////////////////////////////////////////////////
	bool IsProcessingTimed() const;

	void Resize(Vec2i dim) { }
};
// }}}

////////////////////////////////////////////////
/// \brief Widgets
/// \ingroup Records
////////////////////////////////////////////////
namespace Widgets
{
// {{{ TextLine
class TextLine : public Widget
{
	TBString m_line;
	TBChar m_bg;
	virtual void Draw();

public:
	////////////////////////////////////////////////
	/// \brief Constructor
	/// \param win The parent window
	/// \param line The textline
	////////////////////////////////////////////////
	TextLine(Window* win, const TBString& line);


	////////////////////////////////////////////////
	/// \brief Set the line
	/// \param line The new textline
	////////////////////////////////////////////////
	void SetLine(const TBString& line);

	////////////////////////////////////////////////
	/// \brief Get the line
	/// \returns The textline
	////////////////////////////////////////////////
	const TBString& GetLine() const;

	////////////////////////////////////////////////
	/// \brief Set the TextLine's background
	/// \param bg The new background
	////////////////////////////////////////////////
	void SetBackground(const TBChar& bg);
	////////////////////////////////////////////////
	/// \brief Get the TextLine's background
	/// \returns The current background
	////////////////////////////////////////////////
	const TBChar& GetBackground() const;
};
// }}}

// {{{ Button
class Button : public Widget, public BorderItem
{
	TBString m_text;
	TBChar m_bg;

	virtual void Draw();

public:
	Button(Window* win, const TBString& text);

	void SetText(const TBString& text);
	const TBString& GetText() const;

	void SetBackground(const TBChar& bg);
	const TBChar& GetBackground() const;
};
// }}}

// {{{ InputLine
class InputLine : public Widget
{
	String m_text;
	std::size_t m_position; // Position in text
	int m_cursor; // Position relative to the left side of the window
	int m_leftScroll;
	int m_rightScroll;
	std::size_t m_textOffset; // First bound of the text
	TBChar m_bg;
	TBStyle m_textStyle;

	std::size_t m_maxSize;

	std::deque<String> m_history;

	virtual void Draw();

public:
	struct InputLineSettings
	{
		int ScrollTriggerLeft = 20; // %
		int ScrollTriggerRight = 20;
		std::size_t MaxHistory = 50;
		// TODO... (put the style here...)
	};

private:
	InputLineSettings m_settings;

public:
	////////////////////////////////////////////////
	/// \brief InputLine constructor
	/// \param text The default text
	/// \param position The default position
	////////////////////////////////////////////////
	InputLine(const String& text = U"", std::size_t position = 0);

	////////////////////////////////////////////////
	/// \brief Set the InputLine's text
	/// \param text The new text
	////////////////////////////////////////////////
	void SetText(const String& text);
	////////////////////////////////////////////////
	/// \brief Get the InputLine's text
	/// \returns The current text
	////////////////////////////////////////////////
	const String& GetText() const;

	////////////////////////////////////////////////
	/// \brief Set the InputLine's position in text
	/// \param position The new position
	////////////////////////////////////////////////
	void SetTextPosition(std::size_t position);
	////////////////////////////////////////////////
	/// \brief Get the InputLine's cursor position
	/// \returns The current position in text
	////////////////////////////////////////////////
	std::size_t GetTextPosition() const;

	////////////////////////////////////////////////
	/// \brief Set the InputLine's background
	/// \param bg The new background
	////////////////////////////////////////////////
	void SetBackground(const TBChar& bg);
	////////////////////////////////////////////////
	/// \brief Get the InputLine's background
	/// \returns The current background
	////////////////////////////////////////////////
	const TBChar& GetBackground() const;

	////////////////////////////////////////////////
	/// \brief Set the InputLine's text style
	/// \param style The new text style
	////////////////////////////////////////////////
	void SetTextStyle(const TBStyle& style);
	////////////////////////////////////////////////
	/// \brief Get the InputLine's text style
	/// \returns The current text style
	////////////////////////////////////////////////
	const TBStyle& GetTextStyle() const;

	////////////////////////////////////////////////
	/// \brief Set the InputLine's history
	/// \param history The new history
	////////////////////////////////////////////////
	void SetHistory(const std::deque<String>& history);
	////////////////////////////////////////////////
	/// \brief Get the InputLine's history
	/// \returns The current history
	////////////////////////////////////////////////
	const std::deque<String>& GetHistory() const;

	////////////////////////////////////////////////
	/// \brief Set the InputLine's settings
	/// \param settings The new settings
	////////////////////////////////////////////////
	void SetSettings(const InputLineSettings& settings);
	////////////////////////////////////////////////
	/// \brief Get the InputLine's settings
	/// \returns The current settings
	////////////////////////////////////////////////
	const InputLineSettings& GetSettings() const;

	////////////////////////////////////////////////
	/// \brief Set the InputLine's max size
	/// \param maxSize The new max size
	/// \note Putting a ```maxSize``` of ```0``` results in no limitation at all
	////////////////////////////////////////////////
	void SetMaxSize(std::size_t maxSize);
	////////////////////////////////////////////////
	/// \brief Get the InputLine's max text size
	/// \returns The current max text size
	////////////////////////////////////////////////
	std::size_t GetMaxSize() const;
};
// }}}

// {{{ ListSelect
struct ListSelectSettings
{
	int ScrollTriggerUp = 20;
	int ScrollTriggerDown = 20;

	int LeftMargin = 1;
	int NumberSpacing = 1;
	int RightMargin = 1;

	std::size_t NumberBase = 10;
	bool DrawNumbers = true;
	bool RelativeNumbers = true;
	bool NumberRightAlign = true;

	Char TrailingChar = U'~';

	bool Cycling = true;
};

MAKE_CENUM_Q(MarkFnAction, std::uint8_t,
	SET, 0,
	TOGGLE, 1,
	REMOVE, 2,
);

//DrawEntryFn(
//	std::size_t i,
//	const std::vector<Entry>& entries,
//	Vec2i pos,
//	int w,
//	bool hovered,
//	Char trailing
//) -> int (width)
template <ListSelectSettings Settings, class MarkType>
class ListSelect : public Widget
{
	std::function<std::pair<TBStyle, TBStyle>(std::size_t, Vec2i, int, bool, Char)> m_drawEntryFn;
	std::function<void(std::size_t, MarkType, MarkFnAction)> m_markFn;

	std::size_t m_entries;

	std::size_t m_position;
	std::size_t m_offset;

	TBChar m_bg;

public:
	// {{{ Draw
	EventListener<> OnDraw;
	virtual void Draw()
	{
		OnDraw.Notify<EventWhen::BEFORE>();

		int numberWidth = 0; // Width for numbers
		if constexpr (Settings.DrawNumbers)
		{
			if constexpr (Settings.RelativeNumbers)
			{
				numberWidth += Util::GetDigitsNum<Settings.NumberBase>(std::max(static_cast<std::size_t>(GetSize()[1]), m_position));
			}
			else
			{
				numberWidth += Util::GetDigitsNum<Settings.NumberBase>(m_offset+GetSize()[1]);
			}
		}

		for (int y = 0; y < GetSize()[1]; ++y)
		{
			if (m_offset+static_cast<std::size_t>(y) >= m_entries)
			{
				Draw::Horizontal(m_bg, GetPosition()+Vec2i(0, y), GetSize()[0]);
				continue;
			}

			int left = Settings.LeftMargin;
			if constexpr (Settings.DrawNumbers)
			{
				left += Settings.NumberSpacing + numberWidth;
			}

			const auto&& [s1, s2] = m_drawEntryFn(m_offset+static_cast<std::size_t>(y),
					GetPosition()+Vec2i(left, y),
					GetSize()[0]-left-Settings.RightMargin,
					m_offset+static_cast<std::size_t>(y) == m_position,
					Settings.TrailingChar
				);

			// Draw the numbers...
			if constexpr (Settings.DrawNumbers)
			{
				String s;
				if constexpr (Settings.RelativeNumbers)
				{
					if (m_offset+static_cast<std::size_t>(y) == m_position) [[unlikely]]
						s = Util::ToString<Settings.NumberBase>(m_offset+static_cast<std::size_t>(y));
					else [[likely]]
					{
						if (m_position-m_offset < static_cast<std::size_t>(y))
							s = Util::ToString<Settings.NumberBase>(static_cast<std::size_t>(y) - m_position+m_offset);
						else
							s = Util::ToString<Settings.NumberBase>(m_position-m_offset-static_cast<std::size_t>(y));
					}
				}
				else
				{
					String s = Util::ToString<Settings.NumberBase>(m_offset+static_cast<std::size_t>(y));
				}

				if constexpr (Settings.NumberRightAlign)
				{
					const auto p = Draw::TextLine(s, s2, GetPosition()+Vec2i(Settings.LeftMargin+numberWidth-static_cast<int>(s.size()), y), s.size(), {Settings.TrailingChar, s2}).first;
					Draw::Horizontal({m_bg.ch, s2}, GetPosition()+Vec2i(Settings.LeftMargin, y), numberWidth-p);
				}
				else
				{
					const auto p = Draw::TextLine(s, s2, GetPosition()+Vec2i(Settings.LeftMargin, y), numberWidth, {Settings.TrailingChar, s2}).first;
					Draw::Horizontal({m_bg.ch, s2}, GetPosition()+Vec2i(Settings.LeftMargin+p, y), numberWidth-p);
				}
			}

			Draw::Horizontal({m_bg.ch, s1}, GetPosition()+Vec2i(0, y), Settings.LeftMargin);
			Draw::Horizontal({m_bg.ch, s1}, GetPosition()+Vec2i(Settings.LeftMargin+numberWidth, y), Settings.NumberSpacing);
			Draw::Horizontal({m_bg.ch, s1}, GetPosition()+Vec2i(GetSize()[0]-Settings.RightMargin, y), Settings.RightMargin);
		}

		OnDraw.Notify<EventWhen::AFTER>();
	}
	// }}}

	// {{{ Actions
	EventListener<> OnChangePosition;
	EventListener<> OnMarkElement;
	void ActionDown()
	{
		OnChangePosition.Notify<EventWhen::BEFORE>();

		if (m_position == m_entries-1) [[unlikely]]
		{
			if constexpr (Settings.Cycling)
			{
				m_position = m_offset = 0;
			}

			OnChangePosition.Notify<EventWhen::AFTER>();
			return;
		}

		++m_position;
		const int trigger = GetSize()[1] * (100-Settings.ScrollTriggerDown) / 100;
		if (m_position-m_offset >= static_cast<std::size_t>(trigger)
			&& m_offset+GetSize()[1] < m_entries)
			++m_offset;

		OnChangePosition.Notify<EventWhen::AFTER>();
	}

	void ActionDownN(std::size_t N)
	{
		OnChangePosition.Notify<EventWhen::BEFORE>();

		m_position = std::min(m_entries-1, m_position+N);

		const int trigger = GetSize()[1] * (100-Settings.ScrollTriggerDown) / 100;
		if (m_position - m_offset >= static_cast<std::size_t>(trigger)
			&& m_offset+GetSize()[1] < m_entries)
				m_offset = std::min(m_entries-GetSize()[1], m_position + 1 - trigger);

		OnChangePosition.Notify<EventWhen::AFTER>();
	}

	void ActionUp()
	{
		OnChangePosition.Notify<EventWhen::BEFORE>();

		if (m_position == 0) [[unlikely]]
		{
			if constexpr (Settings.Cycling)
			{
				m_position = m_entries-1;
				if (m_entries < static_cast<std::size_t>(GetSize()[1]))
					m_offset = 0;
				else
					m_offset = m_position-GetSize()[1]+1;
			}

			OnChangePosition.Notify<EventWhen::AFTER>();
			return;
		}

		--m_position;
		const std::size_t trigger = GetSize()[1] * (Settings.ScrollTriggerUp) / 100;
		if (m_offset + trigger >= m_position
			&& m_offset != 0)
				m_offset = m_position >= trigger ? m_position - trigger : 0;

		OnChangePosition.Notify<EventWhen::AFTER>();
	}

	void ActionUpN(std::size_t N)
	{
		OnChangePosition.Notify<EventWhen::BEFORE>();

		m_position = m_position-std::min(m_position, N);

		const std::size_t trigger = GetSize()[1] * (Settings.ScrollTriggerUp) / 100;
		if (m_offset + trigger >= m_position
			&& m_offset != 0)
				m_offset = m_position >= trigger ? m_position - trigger : 0;

		OnChangePosition.Notify<EventWhen::AFTER>();
	}

	void ActionSetPosition(std::size_t pos)
	{
		OnChangePosition.Notify<EventWhen::BEFORE>();

		if (m_entries <= static_cast<std::size_t>(GetSize()[1]))
		{
			m_offset = 0;
			m_position = std::min(m_entries-1, pos);
		}
		else if (pos <= m_position)
		{
			m_position = pos;

			const std::size_t trigger = GetSize()[1] * (Settings.ScrollTriggerUp) / 100;
			if (m_offset + trigger >= m_position
				&& m_offset != 0)
					m_offset = (m_position >= trigger ? m_position - trigger : 0);
		}
		else
		{
			m_position = std::min(m_entries-1, pos);

			const int trigger = GetSize()[1] * (100-Settings.ScrollTriggerDown) / 100;
			if (m_position - m_offset >= static_cast<std::size_t>(trigger)
				&& m_offset+GetSize()[1] < m_entries)
					m_offset = std::min(m_entries-GetSize()[1], m_position + 1 - trigger);
		}

		OnChangePosition.Notify<EventWhen::AFTER>();
	}

	void ActionMouseClick(const Vec2i& pos)
	{
		const std::size_t newPos = m_offset+pos[1]-GetPosition()[1];
		if (newPos < m_entries)
			ActionSetPosition(newPos);
	}

	void ActionMarkN(std::size_t N, MarkType mark, MarkFnAction action)
	{
		OnMarkElement.Notify<EventWhen::BEFORE>();

		const auto max = std::min(m_entries-m_position, N);

		for (std::size_t i = 0; i < max; ++i)
			m_markFn(m_position+i, mark, action);

		OnMarkElement.Notify<EventWhen::AFTER>();

		ActionDownN(max);
	}
	// }}}

	ListSelect(decltype(m_drawEntryFn) drawEntryFn, decltype(m_markFn) markFn):
		m_drawEntryFn(drawEntryFn),
		m_markFn(markFn),
		m_entries(0),
		m_position(0),
		m_offset(0),
		m_bg(U' ', Settings::default_text_style)
	{
	}

	void DefaultKeyBindings()
	{
		RemoveAllKeyboardInput();

		AddKeyboardInput(KeyComb(U"DOWN", [this]()
		{
			Termbox& tb = Termbox::GetTermbox();
			if (tb.GetContext().repeat == 0) [[likely]]
				ActionDown();
			else [[unlikely]]
				ActionDownN(tb.GetContext().repeat);
		}));
		AddKeyboardInput(KeyComb(U"PGDN", [this](){ ActionDownN(10); }));
		AddMouseInput(Mouse(std::make_pair(Vec2i(0, 0), GetSize()), Mouse::MOUSE_WHEEL_DOWN,
					[this](const Vec2i&){ ActionDownN(1); }));
		AddKeyboardInput(KeyComb(U"UP", [this]()
		{
			Termbox& tb = Termbox::GetTermbox();
			if (tb.GetContext().repeat == 0) [[likely]]
				ActionUp();
			else [[unlikely]]
				ActionUpN(tb.GetContext().repeat);
		}));
		AddKeyboardInput(KeyComb(U"PGUP", [this](){ ActionUpN(10); }));
		AddMouseInput(Mouse(std::make_pair(Vec2i(0, 0), GetSize()), Mouse::MOUSE_WHEEL_UP,
					[this](const Vec2i&){ ActionUpN(1); }));

		AddKeyboardInput(KeyComb(U"g g",  [this](){ ActionSetPosition(0); }));
		AddKeyboardInput(KeyComb(U"S-G",  [this]()
		{
			Termbox& tb = Termbox::GetTermbox();
			if (tb.GetContext().hasRepeat)
				ActionSetPosition(tb.GetContext().repeat);
			else
				ActionSetPosition(m_entries-1);
		}));
	}

	////////////////////////////////////////////////
	/// \brief Set the ListSelect's entries
	/// \param entries The new number of entries
	////////////////////////////////////////////////
	void SetEntries(std::size_t entries)
	{
		m_entries = entries;
	}

	////////////////////////////////////////////////
	/// \brief Get the ListSelect's entries
	/// \returns The number of entries
	////////////////////////////////////////////////
	std::size_t GetEntries() const
	{
		return m_entries;
	}

	////////////////////////////////////////////////
	/// \brief Set the ListSelect's background
	/// \param bg The new background
	////////////////////////////////////////////////
	void SetBackground(const TBChar& bg)
	{
		m_bg = bg;
	}
	////////////////////////////////////////////////
	/// \brief Get the ListSelect's background
	/// \returns The current background
	////////////////////////////////////////////////
	const TBChar& GetBackground() const
	{
		return m_bg;
	}

	const std::size_t GetPos() const
	{
		return m_position;
	}

	const std::size_t GetOffset() const
	{
		return m_offset;
	}
};
// }}}
}

#endif // TERMBOXWIDGETS_WIDGETS_HPP
