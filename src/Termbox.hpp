#ifndef TERMBOXWIDGETS_TERMBOX_HPP
#define TERMBOXWIDGETS_TERMBOX_HPP

#include "Input.hpp"
#include "Timed.hpp"
#include "Listener.hpp"
#include <deque>
class Widget;
class Window;
class Termbox;


////////////////////////////////////////////////
/// \brief A drawable object
////////////////////////////////////////////////
class Drawable
{
public:
	virtual void Draw() = 0;
};

////////////////////////////////////////////////
/// \brief A resizeable object
////////////////////////////////////////////////
class Resizeable
{
public:
	virtual void Resize(Vec2i dim) = 0;
};

////////////////////////////////////////////////
/// \brief Interface with the termbox api
/// \warning As the termbox library only supports one instance, the constructor must only be called once
////////////////////////////////////////////////
class Termbox
{

	Color m_bg;

	static inline Vec2i s_dim;
	static inline std::function<bool(void)> s_predicate;

	std::vector<std::pair<Widget*, bool>> m_widgets;

	struct Context
	{
		bool stop = false;
		bool clear = false;
		std::size_t frameCount = 0;
		std::size_t repeat = 0;
		bool hasRepeat = false;
		tb_event ev;
		bool lock = false;
		bool stopInput = false;
		bool dontResetRepeat = false;
		bool noRepeat = false;

		std::size_t hasMatched = 0; // (internal to input.hpp)
		// Will be >0 if at least one key has matched, to prevent keybindings like "g t" to trigger "t" on the second key press
	} m_ctx;

	static inline std::deque<Widget*> s_deleteQueue;
	static inline Termbox* m_this;
public:
	////////////////////////////////////////////////
	/// \brief Constructor
	/// Will initialize termbox
	///
	/// \param mode The color output mode
	/// \param bg The background color
	/// \param pedicate Custom predicate, if it returns true, will process events
	/// \throws Util::Exception If termbox could not be initialized
	/// \see Color
	////////////////////////////////////////////////
	Termbox(Color::COLOR_MODE mode, Color bg, std::function<bool(void)> predicate);

	////////////////////////////////////////////////
	/// \brief Destructor
	///
	/// Will uninitialize termbox
	////////////////////////////////////////////////
	~Termbox();

	////////////////////////////////////////////////
	/// \brief Reopen Termbox
	////////////////////////////////////////////////
	static void ReOpen();

	////////////////////////////////////////////////
	/// \brief Close Termbox
	////////////////////////////////////////////////
	static void Close();

	////////////////////////////////////////////////
	/// \brief Draws a Drawable object
	///
	/// \param d A Drawable object
	/// \see Drawable
	////////////////////////////////////////////////
	static void Draw(Drawable& d);

	////////////////////////////////////////////////
	/// \brief Clears the current screen
	////////////////////////////////////////////////
	static void Clear();
	////////////////////////////////////////////////
	/// \brief Resize the termbox display
	////////////////////////////////////////////////
	static void Resize();
	////////////////////////////////////////////////
	/// \brief Display what has been drawn to the screen
	////////////////////////////////////////////////
	static void Display();

	////////////////////////////////////////////////
	/// \brief Set the background's color
	///
	/// \param bg The background's new color
	////////////////////////////////////////////////
	void SetColor(Color bg);
	////////////////////////////////////////////////
	/// \brief Get the background's color
	///
	/// \returns The background's current color
	////////////////////////////////////////////////
	const Color& GetColor() const;

	////////////////////////////////////////////////
	/// \brief Set the new color output mode
	///
	/// \param mode The new color output mode
	/// \see Color::COLOR_MODE
	////////////////////////////////////////////////
	void SetMode(Color::COLOR_MODE mode) const;
	////////////////////////////////////////////////
	/// \brief Get the current color output mode
	///
	/// \returns The current color output mode
	/// \see Color::COLOR_MODE
	////////////////////////////////////////////////
	const Color::COLOR_MODE& GetMode() const;

	////////////////////////////////////////////////
	/// \brief Get the current dimensions of the termbox window
	///
	/// \returns The current dimensions
	////////////////////////////////////////////////
	static Vec2i GetDim();

	////////////////////////////////////////////////
	/// \brief Get the cell at
	///
	/// \param pos The position of cell
	/// \returns The cell at pos, or an empty cell if pos is invalid
	////////////////////////////////////////////////
	static struct tb_cell At(Vec2i pos);

	////////////////////////////////////////////////
	/// \brief Set the cursor's position
	///
	/// \param pos The cursor position
	////////////////////////////////////////////////
	static void SetCursor(Vec2i pos);

	////////////////////////////////////////////////
	/// \brief Add a widget to the widget list
	///
	/// \param widget A pointer to the widget to add
	/// \returns The ID of the widget if it was succesfully added to the list.
	///          std::size_t(-1) if insertion failed
	////////////////////////////////////////////////
	static std::size_t AddWidget(Widget* widget);

	////////////////////////////////////////////////
	/// \brief Remove a widget from the widget list
	///
	/// \param id The id of the widget
	/// \param del If true, will delete the widget after the current event
	/// \returns The widget's address if it succeeds
	///          nullptr if the widget was not found
	////////////////////////////////////////////////
	static Widget* RemoveWidget(std::size_t id, bool del);

	////////////////////////////////////////////////
	/// \brief Get a widget from the widget list
	///
	/// \param id The id of the widget
	/// \returns The widget's address if it succeeds
	///          nullptr if the widget was not found
	/// \note O(1) complexity
	////////////////////////////////////////////////
	static Widget* GetWidget(std::size_t id);

	////////////////////////////////////////////////
	/// \brief Finds a widget ID
	///
	/// \param ptr The widget's pointer
	/// \returns The widget's id if it succeeds
	///          size_t(-1) if the widget was not found
	/// \note O(n = m_widgets.size()) complexity
	////////////////////////////////////////////////
	static std::size_t FindWidget(Widget* ptr);

	////////////////////////////////////////////////
	/// \brief Change the redrawing state of a widget
	///
	/// \param id The id of the widget
	/// \param expired The new state of the widget
	/// \returns True if it succeded
	///          False if it failed
	////////////////////////////////////////////////
	static bool SetWidgetExpired(std::size_t id, bool expired);

	////////////////////////////////////////////////
	/// \brief Redraw widgets to the screen
	///
	/// Will only redraw wigets from the widget list that have been modified
	////////////////////////////////////////////////
	static void ReDraw();

	////////////////////////////////////////////////
	/// \brief Process all events (poll) and set a redrawing flag if needed
	////////////////////////////////////////////////
	static void ProcessEvent();

	static void RenderLoop();

	////////////////////////////////////////////////
	/// \brief Redraw all widgets to the screen
	////////////////////////////////////////////////
	static void ForceDraw();

	////////////////////////////////////////////////
	/// \brief Get the context
	///
	/// \returns The context
	////////////////////////////////////////////////
	static Context& GetContext();

	////////////////////////////////////////////////
	/// \brief Get the termbox environement
	///
	/// \returns The termbox environement
	////////////////////////////////////////////////
	static Termbox& GetTermbox();

	static inline EventListener<std::size_t> OnRepeatChange;
};

#endif // TERMBOXWIDGETS_TERMBOX_HPP
