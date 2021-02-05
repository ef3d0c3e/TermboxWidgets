#ifndef TERMBOXWIDGETS_WINDOW_HPP
#define TERMBOXWIDGETS_WINDOW_HPP

#include "Widgets.hpp"

class Window : public Widget, public Resizeable, public BorderItem
{
	////////////////////////////////////////////////
	/// \brief A class that holds information about
	///  a window
	////////////////////////////////////////////////

	TBString m_windowName;

	TBChar m_bg;
	std::vector<std::pair<bool, Widget*>> m_widgets;
	bool m_invalidate;

	Vec2i m_ipos;
	Vec2i m_isize;

	virtual void Draw();
	virtual bool ProcessKeyboardEvent(Termbox& tb);
	virtual bool ProcessMouseEvent(Termbox& tb, const Widget& w);

	void Resize(Vec2i dim);

protected:
	void AddWidget(Widget* widget);

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
};

#endif // TERMBOXWIDGETS_WINDOW_HPP
