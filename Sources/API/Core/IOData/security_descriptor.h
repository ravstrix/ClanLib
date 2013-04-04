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

/// \addtogroup clanCore_I_O_Data clanCore I/O Data
/// \{

#pragma once

#include "../api_core.h"
#include "security_identifier.h"
#include "access_control_list.h"

namespace clan
{

/// \brief Security descriptor.
class CL_API_CORE SecurityDescriptor
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a security descriptor.
	SecurityDescriptor();

	/// \brief Constructs a SecurityDescriptor
	///
	/// \param copy = Security Descriptor
	SecurityDescriptor(const SecurityDescriptor &copy);

	~SecurityDescriptor();

/// \}
/// \name Attributes
/// \{

public:

/// \}
/// \name Operations
/// \{

public:
	SecurityDescriptor &operator =(const SecurityDescriptor &copy);

/// \}
/// \name Implementation
/// \{

private:
	SecurityIdentifier owner;

	SecurityIdentifier primary_group;

	AccessControlList discretionary_acl;

	bool owner_defaulted;

	bool group_defaulted;

	bool dacl_defaulted;
/// \}
};

}

/// \}
