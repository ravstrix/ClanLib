/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Magnus Norddahl
*/

/// \addtogroup clanCore_Signals clanCore Signals
/// \{

#pragma once

#include "slot.h"
#include "signals_impl.h"

namespace clan
{

/// (Internal ClanLib Class)
template <class Param1, class Param2, class Param3, class Param4, class Param5>
class SlotCallback_v5 : public SlotCallback
{
public:
	virtual void invoke(Param1 param1, Param2 param2, Param3 param3, Param4 param4, Param5 param5) = 0;
};

/// (Internal ClanLib Class)
template <class Param1, class Param2, class Param3, class Param4, class Param5>
class SlotCallback_v5_static : public SlotCallback_v5<Param1, Param2, Param3, Param4, Param5>
{
public:
	SlotCallback_v5_static(void (*static_func)(Param1, Param2, Param3, Param4, Param5))
	: static_func(static_func) { return; }
	void invoke(Param1 param1, Param2 param2, Param3 param3, Param4 param4, Param5 param5) { static_func(param1, param2, param3, param4, param5); }
	void (*static_func)(Param1, Param2, Param3, Param4, Param5);
};

/// (Internal ClanLib Class)
template <class Param1, class Param2, class Param3, class Param4, class Param5, class UserData>
class SlotCallback_v5_static_user : public SlotCallback_v5<Param1, Param2, Param3, Param4, Param5>
{
public:
	SlotCallback_v5_static_user(void (*static_func)(Param1, Param2, Param3, Param4, Param5, UserData), const UserData &user_data)
	: static_func(static_func), user_data(user_data) { return; }
	void invoke(Param1 param1, Param2 param2, Param3 param3, Param4 param4, Param5 param5) { static_func(param1, param2, param3, param4, param5, user_data); }
	void (*static_func)(Param1, Param2, Param3, Param4, Param5, UserData);
	UserData user_data;
};

/// (Internal ClanLib Class)
template <class Param1, class Param2, class Param3, class Param4, class Param5, class InstanceClass>
class SlotCallback_v5_member : public SlotCallback_v5<Param1, Param2, Param3, Param4, Param5>
{
public:
	SlotCallback_v5_member(InstanceClass *instance, void (InstanceClass::*member_func)(Param1, Param2, Param3, Param4, Param5))
	: instance(instance), member_func(member_func) { return; }
	void invoke(Param1 param1, Param2 param2, Param3 param3, Param4 param4, Param5 param5) { (instance->*member_func)(param1, param2, param3, param4, param5); }
	InstanceClass *instance;
	void (InstanceClass::*member_func)(Param1, Param2, Param3, Param4, Param5);
};

/// (Internal ClanLib Class)
template <class Param1, class Param2, class Param3, class Param4, class Param5, class InstanceClass, class UserData>
class SlotCallback_v5_member_user : public SlotCallback_v5<Param1, Param2, Param3, Param4, Param5>
{
public:
	SlotCallback_v5_member_user(InstanceClass *instance, void (InstanceClass::*member_func)(Param1, Param2, Param3, Param4, Param5, UserData), const UserData &user_data)
	: instance(instance), member_func(member_func), user_data(user_data) { return; }
	void invoke(Param1 param1, Param2 param2, Param3 param3, Param4 param4, Param5 param5) { (instance->*member_func)(param1, param2, param3, param4, param5, user_data); }
	InstanceClass *instance;
	void (InstanceClass::*member_func)(Param1, Param2, Param3, Param4, Param5, UserData);
	UserData user_data;
};

/// (Internal ClanLib Class)
template <class Param1, class Param2, class Param3, class Param4, class Param5, class Functor>
class SlotCallback_v5_functor : public SlotCallback_v5<Param1, Param2, Param3, Param4, Param5>
{
public:
	SlotCallback_v5_functor(const Functor &functor)
	: functor(functor) { return; }
	void invoke(Param1 param1, Param2 param2, Param3 param3, Param4 param4, Param5 param5) { functor(param1, param2, param3, param4, param5); }
	Functor functor;
};

/// \brief Signal_v5
///
template <class Param1, class Param2, class Param3, class Param4, class Param5>
class Signal_v5
{
/// \name Construction
/// \{

public:
	Signal_v5()
	: impl(new Signal_Impl) { return; }

	Signal_v5(const Signal_v5<Param1, Param2, Param3, Param4, Param5> &copy)
	: impl(copy.impl) { return; }


/// \}
/// \name Operations
/// \{

public:
	Slot connect(void (*function)(Param1, Param2, Param3, Param4, Param5))
	{
		clean_up();
		std::shared_ptr<SlotCallback> callback(
			new SlotCallback_v5_static<Param1, Param2, Param3, Param4, Param5>(function));
		impl->connected_slots.push_back(callback);
		return Slot(callback);
	}

	template<class UserData>
	Slot connect(void (*function)(Param1, Param2, Param3, Param4, Param5, UserData), const UserData &user_data)
	{
		clean_up();
		std::shared_ptr<SlotCallback> callback(
			new SlotCallback_v5_static_user<Param1, Param2, Param3, Param4, Param5, UserData>(function, user_data));
		impl->connected_slots.push_back(callback);
		return Slot(callback);
	}

	template<class InstanceClass>
	Slot connect(InstanceClass *instance, void (InstanceClass::*function)(Param1, Param2, Param3, Param4, Param5))
	{
		clean_up();
		std::shared_ptr<SlotCallback> callback(
			new SlotCallback_v5_member<Param1, Param2, Param3, Param4, Param5, InstanceClass>(instance, function));
		impl->connected_slots.push_back(callback);
		return Slot(callback);
	}

	template<class InstanceClass, class UserData>
	Slot connect(InstanceClass *instance, void (InstanceClass::*function)(Param1, Param2, Param3, Param4, Param5, UserData), const UserData &user_data)
	{
		clean_up();
		std::shared_ptr<SlotCallback> callback(
			new SlotCallback_v5_member_user<Param1, Param2, Param3, Param4, Param5, InstanceClass, UserData>(instance, function, user_data));
		impl->connected_slots.push_back(callback);
		return Slot(callback);
	}

	template<class Functor>
	Slot connect_functor(const Functor &functor)
	{
		clean_up();
		std::shared_ptr<SlotCallback> callback(
			new SlotCallback_v5_functor<Param1, Param2, Param3, Param4, Param5, Functor>(functor));
		impl->connected_slots.push_back(callback);
		return Slot(callback);
	}

	void invoke(Param1 param1, Param2 param2, Param3 param3, Param4 param4, Param5 param5) const
	{
		std::vector< std::shared_ptr<SlotCallback> > callbacks = impl->connected_slots;
		std::vector< std::shared_ptr<SlotCallback> >::size_type i, size;
		size = callbacks.size();
		for (i = 0; i < size; i++)
			if (callbacks[i]->valid && callbacks[i]->enabled)
				((SlotCallback_v5<Param1, Param2, Param3, Param4, Param5> *) callbacks[i].get())->invoke(param1, param2, param3, param4, param5);
	}


/// \}
/// \name Implementation
/// \{

private:
	void clean_up()
	{
		std::vector< std::shared_ptr<SlotCallback> >::size_type i, size;
		size = impl->connected_slots.size();
		for (i = 0; i < size; i++)
		{
			if (!impl->connected_slots[i]->valid)
			{
				impl->connected_slots.erase(impl->connected_slots.begin()+i);
				i--;
				size--;
			}
		}
	}

	std::shared_ptr<Signal_Impl> impl;
/// \}
};

}

/// \}
