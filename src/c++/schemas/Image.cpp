#include "c++/schemas/Image.h"
#include "c/ModioC.h"                      // for ModioImage
#include "dependencies/nlohmann/json.hpp"  // for json

namespace modio
{
void Image::initialize(ModioImage modio_image)
{
  if (modio_image.filename)
    filename = modio_image.filename;
  if (modio_image.original)
    original = modio_image.original;
  if (modio_image.thumb_320x180)
    thumb_320x180 = modio_image.thumb_320x180;
}

nlohmann::json toJson(Image &image)
{
  nlohmann::json image_json;

  image_json["filename"] = image.filename;
  image_json["original"] = image.original;
  image_json["thumb_320x180"] = image.thumb_320x180;

  return image_json;
}
} // namespace modio
