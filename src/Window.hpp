#ifndef TERMBOXWIDGETS_WINDOW_HPP
#define TERMBOXWIDGETS_WINDOW_HPP

#include "Widgets.hpp"

class Window : public Widget, public BorderItem
{
	////////////////////////////////////////////////
	/// \brief A class that holds information about
	///  a window
	////////////////////////////////////////////////

	TBString m_windowName;

	TBChar m_bg;
	std::vector<std::pair<Widget*, bool>> m_widgets;
	bool m_invalidate;

	Vec2i m_ipos;
	Vec2i m_isize;

	virtual void Draw();
	virtual std::pair<bool, bool> ProcessKeyboardEvent(Termbox& tb);
	virtual bool ProcessMouseEvent(Termbox& tb, const Widget& w);

	void Resize(Vec2i dim);

protected:
	////////////////////////////////////////////////
	/// \brief Add a widget to the widget list
	///
	/// \param widget A pointer to the widget to add
	/// \returns The ID of the widget if it was succesfully added to the list.
	/// \note O(n) complexity
	////////////////////////////////////////////////
	std::size_t AddWidget(Widget* widget);

	////////////////////////////////////////////////
	/// \brief Remove a widget from the widget list
	///
	/// \param id The id of the widget
	/// \returns The widget's address if it succeeds
	///          nullptr if the widget was not found
	/// \note O(n) complexity
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
	/// \note O(1) complexity
	////////////////////////////////////////////////
	bool SetWidgetExpired(std::size_t id, bool expired);

public:
	////////////////////////////////////////////////
	/// \brief Constructor
	/// \param windowName The window's name
	/// \param parent The parent window
	////////////////////////////////////////////////
	Window(const TBString& windowName, Window* parent = nullptr);
	~Window();

	////////////////////////////////////////////////
	/// \brief Set the window's name
	/// \param name The new window's name
	/// \see TBString
	////////////////////////////////////////////////
	void SetName(const TBString& name);
	////////////////////////////////////////////////
	/// \brief Get the window's name
	/// \returns A const reference to the window's name
	/// \see TBString
	////////////////////////////////////////////////
	const TBString& GetName() const;

	////////////////////////////////////////////////
	/// \brief Set the window's background
	/// \param pos The window's new background
	////////////////////////////////////////////////
	void SetBackground(const TBChar& bg, bool border = true, bool title = true);
	////////////////////////////////////////////////
	/// \brief Get the window's background
	/// \return The window's current background
	////////////////////////////////////////////////
	const TBChar& GetBackground() const;

	////////////////////////////////////////////////
	/// \brief Check if all widgets fir in the window
	/// \returns True if all widgets fit
	////////////////////////////////////////////////
	bool CheckAllWidgets() const;

	////////////////////////////////////////////////
	/// \brief Get the window's global bounds
	/// \returns The global bounds
	/// \warning The window must have been drawn at least once!
	////////////////////////////////////////////////
	std::pair<Vec2i, Vec2i> GetGlboalBounds() const;

	////////////////////////////////////////////////
	/// \brief Draw/Redraw a widget as part of this window
	/// \param widget The widget
	////////////////////////////////////////////////
	void ReDraw(Widget* widget) const;

	////////////////////////////////////////////////
	/// \brief Invalidate the window, marking it for complete redrawing
	////////////////////////////////////////////////
	void Invalidate();

	////////////////////////////////////////////////
	/// \brief Set all widgets to inactive
	/// \returns A list of widgets with their state prior calling SetAllInactive()
	////////////////////////////////////////////////
	std::vector<std::pair<Widget*, bool>> SetAllInactive();

	////////////////////////////////////////////////
	/// \brief Restore widgets to the state stored in list
	/// \param list The list of states for the widgets
	////////////////////////////////////////////////
	void RestoreAllActive(std::vector<std::pair<Widget*, bool>>&& list);
};

#endif // TERMBOXWIDGETS_WINDOW_HPP
