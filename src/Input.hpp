#ifndef TERMBOXWIDGETS_INPUT_HPP
#define TERMBOXWIDGETS_INPUT_HPP

#include "Settings.hpp"
class Termbox;
class Widget;

// {{{ Mouse
////////////////////////////////////////////////
/// \brief Defines an area over which a mouse event can happen
////////////////////////////////////////////////
class Mouse
{
	Vec2i m_posOffset;
	Vec2i m_szOffset;

	std::function<void(const Vec2i&)> m_callback;

public:
	MAKE_CENUMV_Q(Type, std::uint16_t,
		MOUSE_LEFT, TB_KEY_MOUSE_LEFT,
		MOUSE_RIGHT, TB_KEY_MOUSE_RIGHT,
		MOUSE_MIDDLE, TB_KEY_MOUSE_MIDDLE,
		MOUSE_RELEASE, TB_KEY_MOUSE_RELEASE,
		MOUSE_WHEEL_UP, TB_KEY_MOUSE_WHEEL_UP,
		MOUSE_WHEEL_DOWN, TB_KEY_MOUSE_WHEEL_DOWN
	)

private:
	Type m_type;

public:
	////////////////////////////////////////////////
	/// \brief Constructor
	/// \param callback The callback
	////////////////////////////////////////////////
	Mouse(Type type, decltype(m_callback) callback);
	////////////////////////////////////////////////
	/// \brief Constructor
	/// \param offset Offset for the ares (relative to the widget's sizes)
	/// \param callback The callback
	////////////////////////////////////////////////
	Mouse(const std::pair<Vec2i, Vec2i>& offset, Type type, decltype(m_callback) callback);

	////////////////////////////////////////////////
	/// \brief Destructor
	////////////////////////////////////////////////
	~Mouse();

	////////////////////////////////////////////////
	/// \brief Set the offsets
	/// \param offset The offsets
	////////////////////////////////////////////////
	void SetOffset(const std::pair<Vec2i, Vec2i>& offset);

	////////////////////////////////////////////////
	/// \brief Will match with the area
	/// \param tb The Termbox environement
	/// \param w The Widget
	/// \return True if the callback was called
	////////////////////////////////////////////////
	bool Match(Termbox& tb, const Widget& w) const;
};
// }}}

// {{{ Key
////////////////////////////////////////////////
/// \brief Class that hold a single key
////////////////////////////////////////////////
struct Key
{
	Char code;
	MAKE_CENUMV_Q(Type, std::uint8_t,
		CHAR, 0,
		KEY, 1,
	)
	Type type;
	MAKE_CENUMV_Q(Meta, std::uint8_t,
		NONE, 0,
		ANY, 1,
		SHIFT, 2,
		ALT, 3,
		ALTSHIFT, 4,
		CTRL, 5,
		CTRLSHIFT, 6,
		ALTCTRL, 7,
		ALTCTRLSHIFT, 8,
	)
	Meta meta;

	////////////////////////////////////////////////
	/// \brief Default constructor
	////////////////////////////////////////////////
	Key();

	////////////////////////////////////////////////
	/// \brief Constructor
	/// \param code Code for the key
	/// \param type The type of the key
	/// \param meta The meta of the key
	/// \see Key::Meta, Key::Type
	////////////////////////////////////////////////
	Key(Char code, Type type, Meta meta = Meta::ANY);

	////////////////////////////////////////////////
	/// \brief Comparison
	/// \param k The key to compare
	/// \returns `true` if the two keys are the same
	////////////////////////////////////////////////
	bool operator==(const Key& k) const;

	////////////////////////////////////////////////
	/// \brief Get the name of a key
	/// A key with a SHIFT meta will be capitalized if it is a character.
	/// Else a '`S-`' will be outputted
	////////////////////////////////////////////////
	String GetKeyName() const;
};
// }}}

// {{{ KeyComb
////////////////////////////////////////////////
/// \brief Class that hold a combination of keys
/// \see Key
////////////////////////////////////////////////
class KeyComb
{
	Key* m_keys;
	std::size_t m_keys_num;
	std::function<void()> m_callback;

	std::size_t m_matchState;

public:
	////////////////////////////////////////////////
	/// \brief Constructor
	/// \param callback The callback
	////////////////////////////////////////////////
	KeyComb(decltype(m_callback) callback);
	////////////////////////////////////////////////
	/// \brief Constructor
	/// \param s The string for the combination
	/// \param callback The callback
	////////////////////////////////////////////////
	KeyComb(const String& s, decltype(m_callback) callback);

	////////////////////////////////////////////////
	/// \brief Destructor
	////////////////////////////////////////////////
	~KeyComb();

	////////////////////////////////////////////////
	/// \brief Comparison
	/// \param kc The KeyComb to compare
	/// \returns `true` if the two KeyComb are the same
	////////////////////////////////////////////////
	bool operator==(const KeyComb& kc) const;

	////////////////////////////////////////////////
	/// \brief Set the combination
	/// \param s The string for the combination
	////////////////////////////////////////////////
	std::pair<bool, std::size_t> SetComb(const String& s);

	////////////////////////////////////////////////
	/// \brief Number of keys
	/// \return The number of keys
	////////////////////////////////////////////////
	std::size_t Size() const;

	////////////////////////////////////////////////
	/// \brief Key at
	/// \param i The position
	/// \warning Performs n bound checking
	/// \return The key at position i
	////////////////////////////////////////////////
	const Key& operator[](std::size_t i) const;

	////////////////////////////////////////////////
	/// \brief Get the name of a combination
	/// \return The name of the combination, ready for printing
	////////////////////////////////////////////////
	String GetName() const;

	////////////////////////////////////////////////
	/// \brief Will match with the keys
	/// \param tb The termbox environement
	/// \return True if the callback was called
	////////////////////////////////////////////////
	bool Match(Termbox& tb);
};
// }}}

// {{{ Input
////////////////////////////////////////////////
/// \brief Process keyboard events
/// \param ev The event to process
/// \param tb The termbox environement
/// \returns true If at least one event has been processed
////////////////////////////////////////////////
class KeyboardInput
{
	std::vector<KeyComb> m_keys;
public:
	virtual bool ProcessKeyboardEvent(Termbox& tb)
	{
		bool matched = false;
		for (auto& kc : m_keys)
		{
			if (matched)
				break;
			matched |= kc.Match(tb);
		}
		return matched;
	}

	////////////////////////////////////////////////
	/// \brief Add an input
	/// \param kc The Key combination event
	////////////////////////////////////////////////
	void AddKeyboardInput(const KeyComb& kc)
	{
		m_keys.push_back(kc);
	}

	////////////////////////////////////////////////
	/// \brief Remove an input
	/// \param kc The KeyComb event to remove
	/// \returns true If an event was removed
	////////////////////////////////////////////////
	bool RemoveKeyboardInput(const KeyComb& kc)
	{
		return std::erase_if(m_keys, [&](const KeyComb& __kc){ return kc == __kc; }) != 0;
	}

	////////////////////////////////////////////////
	/// \brief Remove all input
	////////////////////////////////////////////////
	void RemoveAllKeyboardInput()
	{
		m_keys.clear();
	}
};

////////////////////////////////////////////////
/// \brief Process keyboard events
////////////////////////////////////////////////
class MouseInput
{
	std::vector<Mouse> m_mouse;
public:
	////////////////////////////////////////////////
	/// \brief Process mouse events
	/// \param ev The event to process
	/// \param tb The termbox environement
	/// \param w The widget
	/// \returns true If at least one event has been processed
	////////////////////////////////////////////////
	virtual bool ProcessMouseEvent(Termbox& tb, const Widget& w)
	{
		bool matched = false;
		for (auto& m : m_mouse)
		{
			if (matched)
				break;
			matched |= m.Match(tb, w);
		}
		return matched;
	}

	////////////////////////////////////////////////
	/// \brief Add an input
	/// \param m The mouse event
	////////////////////////////////////////////////
	void AddMouseInput(const Mouse& m)
	{
		m_mouse.push_back(m);
	}

	////////////////////////////////////////////////
	/// \brief Remove all input
	////////////////////////////////////////////////
	void RemoveAllMouseInput()
	{
		m_mouse.clear();
	}
};
// }}}

#endif // TERMBOXWIDGETS_INPUT_HPP
