// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

# pragma warning(disable:4251) // Disable VC warning about dll linkage required (for private members?)
# pragma warning(disable:4275) // disable warning about non dll-interface base class.
# pragma warning(disable:4996) // Disable VC warning that std library may be unsafe

#include <vcclr.h>

#include <vector>
#include <map>

#include <boost/shared_ptr.hpp>
#include <boost/scoped_array.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/operators.hpp>

#include "Common/QuickFAST_Export.h"
#include "Common/Types.h"
#include "Common/Decimal.h"

#include "Codecs/TemplateRegistry_fwd.h"
#include "Codecs/Decoder.h"
#include "Codecs/Encoder.h"
#include "Codecs/DataSource.h"
#include "Codecs/DataDestination.h"
#include "Codecs/DataDestination_fwd.h"
#include "Codecs/XMLTemplateParser.h"

#include "Messages/Message.h"
#include "Messages/FieldSet.h"
#include "Messages/FieldAscii.h"
#include "Messages/FieldByteVector.h"
#include "Messages/FieldDecimal.h"
#include "Messages/FieldGroup.h"
#include "Messages/FieldInt32.h"
#include "Messages/FieldInt64.h"
#include "Messages/FieldUInt32.h"
#include "Messages/FieldUInt64.h"
#include "Messages/FieldSequence.h"
#include "Messages/FieldSet.h"
#include "Messages/FieldUtf8.h"
#include "Messages/Sequence_fwd.h"
#include "Messages/Sequence.h"
#include "Messages/Group_fwd.h"
#include "Messages/FieldInt8.h"
#include "Messages/FieldUInt8.h"
#include "Messages/FieldInt16.h"
#include "Messages/FieldUInt16.h"
#include "Messages/FieldBitmap.h"

