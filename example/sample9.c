/*
  $Id: sample9.c,v 1.1 2004/07/29 05:26:46 rocky Exp $

  Copyright (C) 2004 Rocky Bernstein <rocky@panix.com>
  
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

/* Simple program to show use of SCSI MMC interface. */
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif
#include <stdio.h>
#include <sys/types.h>
#include <cdio/cdio.h>
#include <cdio/scsi_mmc.h>
#include <string.h>

/* Set how long do wto wait for SCSI-MMC commands to complete */
#define DEFAULT_TIMEOUT_MS 10000

int
main(int argc, const char *argv[])
{
  CdIo *p_cdio;

  p_cdio = cdio_open (NULL, DRIVER_UNKNOWN);

  if (NULL == p_cdio) {
    printf("Couldn't find CD\n");
    return 1;
  } else {
    int i_status;                  /* Result of SCSI MMC command */
    char buf[36] = { 0, };         /* Place to hold returned data */
    scsi_mmc_cdb_t cdb = {{0, }};  /* Command Descriptor Block */

    CDIO_MMC_SET_COMMAND(cdb.field, CDIO_MMC_GPCMD_INQUIRY);
    cdb.field[4] = sizeof(buf);

    i_status = scsi_mmc_run_cmd(p_cdio, DEFAULT_TIMEOUT_MS, 
				&cdb, SCSI_MMC_DATA_READ, 
				sizeof(buf), &buf);
    if (i_status == 0) {
      char vendor[CDIO_MMC_HW_VENDOR_LEN+1];
      char model[CDIO_MMC_HW_MODEL_LEN+1];
      char rev[CDIO_MMC_HW_REVISION_LEN+1];
      
      memcpy(vendor, buf + 8, sizeof(vendor)-1);
      vendor[sizeof(vendor)-1] = '\0';
      memcpy(model,  buf + 8 + CDIO_MMC_HW_VENDOR_LEN, sizeof(model)-1);
      model[sizeof(model)-1] = '\0';
      memcpy(rev,    buf + 8 + CDIO_MMC_HW_VENDOR_LEN +CDIO_MMC_HW_MODEL_LEN,
	     sizeof(rev)-1);
      rev[sizeof(rev)-1] = '\0';

      printf("Vendor: %s\nModel: %s\nRevision: %s\n",
	     vendor, model, rev);
    } else {
      printf("Couldn't get INQUIRY data (vendor, model, and revision\n");
    }
  }
  
  cdio_destroy(p_cdio);
  
  return 0;
}