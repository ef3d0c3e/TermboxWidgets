#ifndef TERMBOXWIDGETS_LISTENER_HPP
#define TERMBOXWIDGETS_LISTENER_HPP

#include <functional>
#include "Settings.hpp"

MAKE_CENUM_Q(EventWhen, std::uint8_t,
	BEFORE, 0,
	AFTER, 1);

template <class... Args>
class EventListener
{
	using Fn = std::function<void(Args...)>;
	std::vector<Fn> m_fnBefore;
	std::vector<Fn> m_fnAfter;
public:
	////////////////////////////////////////////////
	/// \brief Constructor
	////////////////////////////////////////////////
	EventListener() {}

	////////////////////////////////////////////////
	/// \brief Add an event listener
	/// \param fn The callback
	/// \param when When the event should be run
	/// \returns The id of the event listener
	////////////////////////////////////////////////
	std::size_t AddEvent(Fn fn, EventWhen when = EventWhen::AFTER)
	{
		if (when == EventWhen::BEFORE)
		{
			m_fnBefore.push_back(fn);
			return m_fnBefore.size()-1;
		}
		else if (when == EventWhen::AFTER)
		{
			m_fnAfter.push_back(fn);
			return m_fnAfter.size()-1;
		}

		throw Util::Exception("Unhandled EventWhen.");
	}

	////////////////////////////////////////////////
	/// \brief Remove an event listener
	/// \param id The id of the event listener
	/// \param when When the event is ran
	/// \returns true if the event listener was removed
	////////////////////////////////////////////////
	bool RemoveEvent(std::size_t id, EventWhen when)
	{
		if (when == EventWhen::BEFORE && id < m_fnBefore.size())
		{
			m_fnBefore.erase(m_fnBefore.begin()+id);

			return true;
		}
		else if (when == EventWhen::AFTER && id < m_fnAfter.size())
		{
			m_fnAfter.erase(m_fnAfter.begin()+id);

			return true;
		}

		return false;
	}

	////////////////////////////////////////////////
	/// \brief Get an event listener
	/// \param id The id of the event listener
	/// \param when When the event is ran
	/// \returns The event listener
	/// \note Will throw if there is no event listener with id
	////////////////////////////////////////////////
	EventListener& GetEvent(std::size_t id, EventWhen when)
	{
		if (when == EventWhen::BEFORE)
		{
			if (id >= m_fnBefore.size())
				throw Util::Exception("Tried to get Event with invalid id.");

			return m_fnBefore[id];
		}
		else if (when == EventWhen::AFTER)
		{
			if (id >= m_fnAfter.size())
				throw Util::Exception("Tried to get Event with invalid id.");

			return m_fnAfter[id];
		}

		throw Util::Exception("Unhandled EventWhen.");
	}
	const EventListener& GetEvent(std::size_t id, EventWhen when) const;

	////////////////////////////////////////////////
	/// \brief Notify the callbacks
	/// \param widget The widget that calls it
	/// \param data Some data about the callback
	/// \tparam When the type of event
	////////////////////////////////////////////////
	template <EventWhen When>
	void Notify(Args... args) const
	{
		if constexpr (When == EventWhen::BEFORE)
		{
			for (const auto& fn : m_fnBefore)
				fn(args...);
		}
		else if constexpr (When == EventWhen::AFTER)
		{
			for (const auto& fn : m_fnAfter)
				fn(args...);
		}
		else
		{
			[]<bool v = false>()
			{
				static_assert(v, "Unhandled EventWhen.");
			}();
		}
	}
};


#endif // TERMBOXWIDGETS_LISTENER_HPP
