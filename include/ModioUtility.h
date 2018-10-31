#ifndef MODIO_MODIOUTILITY_H
#define MODIO_MODIOUTILITY_H

#include "Utility.h"
#include "Globals.h"
#include "wrappers/CurlWrapper.h"
#include "c/ModioC.h"
#include "c++/schemas/Mod.h"

struct GenericRequestParams
{
  void* object;
  void (*callback)(void* object, ModioResponse response);
};

namespace modio
{
  void addModsToDownloadQueue(std::vector<u32> mod_ids);
  void pollEvents();
}

#endif