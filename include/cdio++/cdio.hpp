/* -*- C++ -*-
    $Id: cdio.hpp,v 1.5 2006/01/17 02:09:32 rocky Exp $

    Copyright (C) 2005, 2006 Rocky Bernstein <rocky@panix.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/** \file cdio.hpp
 *
 *  \brief C++ class for libcdio: the CD Input and Control
 *  library. Applications use this for anything regarding libcdio.
 */

#ifndef __CDIO_HPP__
#define __CDIO_HPP__

#include <cdio/cdio.h>
#include <cdio/audio.h>
#include <cdio/dvd.h>
#include <cdio/mmc.h>

// Make pre- and post-increment operators for enums in libcdio where it 
// makes sense.
#include <cdio++/enum.hpp>

class Cdio {

public:

  // Other member functions
#include "devices.hpp"
};

/** A class relating to CD-Text. Use invalid track number 0 to specify
    CD-Text for the CD (as opposed to a specific track).
*/
class CdioCDText
{
public: 
  CdioCDText(cdtext_t *p)
  { 
    p_cdtext = p;
    cdtext_init(p); // make sure we're initialized on the C side
  }

  ~CdioCDText() 
  {
    cdtext_destroy(p_cdtext);
    p_cdtext = (cdtext_t *) NULL;
  }

  // Other member functions
#include "cdtext.hpp"

private:
  cdtext_t *p_cdtext;
};
    
/** A class relating to tracks. A track object basically saves device
    and track number information so that in track operations these
    don't have be specified.
*/
class CdioTrack
{

public: 
  CdioTrack(CdIo_t *p, track_t t)
  { 
    i_track = t;
    p_cdio = p;
  }

  // Other member functions
#include "track.hpp"

private:
  track_t i_track;
  CdIo_t *p_cdio;
};
    
/** A class relating to a CD-ROM device or pseudo CD-ROM device with
    has a particular CD image. A device basically saves the libcdio
    "object" (of type CdIo *). 
*/
class CdioDevice 
{

public:
  CdioDevice(CdIo_t *p = (CdIo_t *) NULL)
  { 
    p_cdio=p; 
  };

  ~CdioDevice() 
  { 
    cdio_destroy(p_cdio); 
    p_cdio = (CdIo_t *) NULL;
  };

  /** Class for driver exceptions. **/
  class DriverOpException 
  {
  public:
    driver_return_code_t driver_return_code;
    DriverOpException( void ) { };
    DriverOpException( driver_return_code_t drc ) { 
      driver_return_code = drc; 
    };
    driver_return_code_t get_code(void) { 
      return driver_return_code; 
    };
    const char *get_msg(void) { 
      return cdio_driver_return_code_to_str(driver_return_code); 
    };
  };

  class DriverOpError: public DriverOpException
  {
  public:
    DriverOpError(void) { driver_return_code = DRIVER_OP_ERROR; }
  };

  class DriverOpUnsupported: public DriverOpException 
  { 
  public:
    DriverOpUnsupported(void) { driver_return_code = DRIVER_OP_UNSUPPORTED; }
  };

  class DriverOpUninit: public DriverOpException
  {
  public:
    DriverOpUninit(void) { driver_return_code = DRIVER_OP_UNINIT; }
  };

  class DriverOpNotPermitted: public DriverOpException
  {
  public:
    DriverOpNotPermitted(void) {driver_return_code = DRIVER_OP_NOT_PERMITTED;}
  };

  class DriverOpBadParameter: public DriverOpException
  {
  public:
    DriverOpBadParameter(void) {driver_return_code = DRIVER_OP_BAD_PARAMETER;}
  };

  class DriverOpBadPointer: public DriverOpException
  {
  public:
    DriverOpBadPointer(void) {driver_return_code = DRIVER_OP_BAD_POINTER;}
  };

  class DriverOpNoDriver: public DriverOpException
  {
  public:
    DriverOpNoDriver(void) {driver_return_code = DRIVER_OP_NO_DRIVER;}
  };

  // Other member functions  
#include "device.hpp"
#include "disc.hpp"
#include "mmc.hpp"
#include "read.hpp"

private:
  CdIo_t *p_cdio;
  void possible_throw_device_exception(driver_return_code_t drc) 
  {
    switch (drc) {
    case DRIVER_OP_SUCCESS:
      return;
    case DRIVER_OP_ERROR:
      throw DriverOpError();
    case DRIVER_OP_UNSUPPORTED:
      throw DriverOpUnsupported();
    case DRIVER_OP_UNINIT:
      throw DriverOpUninit();
    case DRIVER_OP_NOT_PERMITTED:
      throw DriverOpNotPermitted();
    case DRIVER_OP_BAD_PARAMETER:
      throw DriverOpBadParameter();
    case DRIVER_OP_BAD_POINTER:
      throw DriverOpBadPointer();
    case DRIVER_OP_NO_DRIVER:
      throw DriverOpNoDriver();
    default:
      throw DriverOpException(drc);
    }
  }
};

#endif /* __CDIO_HPP__ */