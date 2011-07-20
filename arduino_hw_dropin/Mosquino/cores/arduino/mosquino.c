/*
  mosquino.c - board-specific functions for Mosquino (Aedes and probably others)
  Part of Mosquino

  Copyright (c) 2011 Tim Gipson (Drmn4ea)

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA

  $Id: $
*/

#include "mosquino.h"

// The POWER_GOOD and POWER_FULL signals of Mosquino are both active-low, which is counterintuitive, and probably confusing for some users.
// For clarity and futureproofing, these wrappers will return the correct polarity.

uint8_t power_good(void)
{
   return !(digitalRead(PIN_POWER_GOOD) );
}

uint8_t power_full(void)
{
   return !(digitalRead(PIN_POWER_FULL) );
}

// In an early prototype, there was a dedicated BUS_SENSE pin whose only function was to detect the USB attached state.
// This function is now multiplexed with the POWER_FULL signal (attachment to a probably wall-powered PC is 'unlimited' power for our purposes).
// As before (but moreso), user code should actually check for a connection as part of its comm protocol rather than blindly assuming
// bus_sense() / power_full() means a PC is present and a client is listening.

uint8_t bus_sense(void)
{
   return !(digitalRead(PIN_POWER_FULL) );
}
