/*
   Copyright (c) 2014, The Linux Foundation. All rights reserved.
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are
   met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.
    * Neither the name of The Linux Foundation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.
   THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
   WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
   ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
   BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
   BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
   OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
   IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <cstdlib>
#include <cstdio>
#include <fstream>
#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>

#include <android-base/properties.h>
#include "property_service.h"
#include "vendor_init.h"

using android::base::GetProperty;
using android::init::property_set;
using std::string;

bool has_tianma_panel()
{
    std::ifstream cmdline("/proc/cmdline");
    string line;
    bool ret = false;

    std::getline(cmdline, line);
    if (line.find("tianma") != string::npos)
        ret = true;

    cmdline.close();
    return ret;
}

void vendor_load_properties()
{
    // Hax for Tianma panels burn-in issue
    // https://forum.xda-developers.com/redmi-note-8/how-to/huge-issue-custom-roms-spoiling-display-t4075133
    if (has_tianma_panel()) {
        FILE *red = fopen("/sys/module/msm_drm/parameters/kcal_red", "wb");
        FILE *green = fopen("/sys/module/msm_drm/parameters/kcal_green", "wb");
        FILE *blue = fopen("/sys/module/msm_drm/parameters/kcal_blue", "wb");

        // Incase the kernel doesn't have KCAL support
        if (red != NULL && green != NULL && blue != NULL) {
            // 90% is the desired rgb value to prevent burn-in in Tianma panels
            // 90% of default value 256 = 230.4 ~= 230
            fprintf(red, "230\n");
            fprintf(green, "230\n");
            fprintf(blue, "230\n");

            fclose(red);
            fclose(green);
            fclose(blue);
        }
    }
}
