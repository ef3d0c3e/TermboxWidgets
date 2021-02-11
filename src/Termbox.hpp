#ifndef TERMBOXWIDGETS_TERMBOX_HPP
#define TERMBOXWIDGETS_TERMBOX_HPP

#include "Input.hpp"
#include "Timed.hpp"
#include "Listener.hpp"
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
	} m_ctx;

	static inline Termbox* m_this;
public:
	////////////////////////////////////////////////
	/// \brief Constructor
	/// Will initialize termbox
	///
	/// \param mode The color output mode
	/// \param bg The background color
	/// \throws Util::Exception If termbox could not be initialized
	/// \see Color
	////////////////////////////////////////////////
	Termbox(Color::COLOR_MODE mode, Color bg);

	////////////////////////////////////////////////
	/// \brief Destructor
	///
	/// Will uninitialize termbox
	////////////////////////////////////////////////
	~Termbox();

	////////////////////////////////////////////////
	/// \brief Draws a Drawable object
	///
	/// \param d A Drawable object
	/// \see Drawable
	////////////////////////////////////////////////
	void Draw(Drawable& d) const;

	////////////////////////////////////////////////
	/// \brief Clears the current screen
	////////////////////////////////////////////////
	void Clear() const;
	////////////////////////////////////////////////
	/// \brief Resize the termbox display
	////////////////////////////////////////////////
	void Resize();
	////////////////////////////////////////////////
	/// \brief Display what has been drawn to the screen
	////////////////////////////////////////////////
	void Display();

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
	std::size_t AddWidget(Widget* widget);

	////////////////////////////////////////////////
	/// \brief Remove a widget from the widget list
	///
	/// \param id The id of the widget
	/// \returns The widget's address if it succeeds
	///          nullptr if the widget was not found
	////////////////////////////////////////////////
	Widget* RemoveWidget(std::size_t id);

	////////////////////////////////////////////////
	/// \brief Get a widget from the widget list
	///
	/// \param id The id of the widget
	/// \returns The widget's address if it succeeds
	///          nullptr if the widget was not found
	/// \note O(1) complexity
	////////////////////////////////////////////////
	Widget* GetWidget(std::size_t id);

	////////////////////////////////////////////////
	/// \brief Change the redrawing state of a widget
	///
	/// \param id The id of the widget
	/// \param expired The new state of the widget
	/// \returns True if it succeded
	///          False if it failed
	////////////////////////////////////////////////
	bool SetWidgetExpired(std::size_t id, bool expired);

	////////////////////////////////////////////////
	/// \brief Redraw widgets to the screen
	///
	/// Will only redraw wigets from the widget list that have been modified
	////////////////////////////////////////////////
	void ReDraw();

	////////////////////////////////////////////////
	/// \brief Process all events (poll) and set a redrawing flag if needed
	////////////////////////////////////////////////
	void ProcessEvent();

	void RenderLoop();

	////////////////////////////////////////////////
	/// \brief Redraw all widgets to the screen
	////////////////////////////////////////////////
	void ForceDraw();

	////////////////////////////////////////////////
	/// \brief Get the context
	///
	/// \returns The context
	////////////////////////////////////////////////
	Context& GetContext();

	////////////////////////////////////////////////
	/// \brief Get the termbox environement
	///
	/// \returns The termbox environement
	////////////////////////////////////////////////
	static Termbox& GetTermbox();

	static inline EventListener<std::size_t> OnRepeatChange;
};

#endif // TERMBOXWIDGETS_TERMBOX_HPP
