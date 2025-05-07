#pragma once
#include "TypeDefinitions.h"
#include <string>
namespace cinnamontoast {

using ObjectUID = unsigned int;
class Object {
private:
  static std::map<int, int> parentOf;
  ObjectUID uid = 0;

protected:
  CTOAST_API virtual void
  initializeObject(ObjectUID uid,
                   ObjectUID parent = CTOAST_OBJECT_OBJECT) final;

public:
  CTOAST_API Object();
  CTOAST_API virtual inline std::string toString() noexcept;
  CTOAST_API virtual inline unsigned int getUID() const noexcept;
  CTOAST_API virtual ~Object() = 0;

  friend void setUidToParent(Object &obj, ObjectUID parentUid);
  template <typename T>
  friend T *fastCast(Object *comp, ObjectUID expectedType);
};
template <typename T>
concept DerivedFromObject = std::is_base_of_v<Object, T>;

template <DerivedFromObject T>
T *fastCast(Object *comp, ObjectUID expectedType) {
  if (!comp)
    return nullptr;

  int currentUid = comp->getUID();

  // Traverse up the parent chain
  while (true) {
    if (currentUid == expectedType) {
      return reinterpret_cast<T *>(comp);
    }

    auto it = Object::parentOf.find(currentUid);
    if (it == Object::parentOf.end()) {
      break; // No parent found, end of chain
    }
    currentUid = it->second;
  }

  return nullptr; // No match found in the ancestry
};
void setUidToParent(Object &obj, ObjectUID parentUid);
} // namespace cinnamontoast