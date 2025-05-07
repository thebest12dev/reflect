#pragma once
#include "TypeDefinitions.h"
#include <string>
namespace cinnamontoast {
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
} // namespace cinnamontoast