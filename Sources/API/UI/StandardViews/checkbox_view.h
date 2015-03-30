/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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
**    Harry Storbacka
**    Mark Page
*/

#pragma once

#include "../View/view.h"

namespace clan
{
	class CheckBoxView_Impl;

	class CheckBoxView : public View
	{
	public:

		CheckBoxView();

		enum class Check
		{
			unchecked = 0,
			checked = 1,
			indeterminated = 2
		};

		bool disabled() const;
		void set_disabled();
		void set_enabled();

		/// \brief Check
		///
		/// \return checked state
		Check check() const;

		/// \brief Set checked
		///
		/// \param enable = bool
		void set_check(Check check_state = Check::checked);

		/// \brief Set 3state
		///
		/// \param enable = bool
		void set_tri_state(bool enable);

		/// \brief Func state changed
		std::function<void()> &func_state_changed();

	private:
		std::shared_ptr<CheckBoxView_Impl> impl;

	};

}

