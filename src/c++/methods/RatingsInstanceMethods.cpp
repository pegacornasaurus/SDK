#include "c++/ModIOInstance.h"

namespace modio
{
void Instance::addModRating(u32 mod_id, bool vote_up, const std::function<void(const modio::Response &response)> &callback)
{
  struct GenericCall *add_mod_rating_call = new GenericCall{callback};
  add_mod_rating_calls[current_call_id] = add_mod_rating_call;

  modioAddModRating(new u32(current_call_id), mod_id, vote_up, &onAddModRating);

  current_call_id++;
}
} // namespace modio
