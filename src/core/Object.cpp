#include "Object.h"

namespace reflect {
std::map<int, int> Object::parentOf;
Object::Object() {};
unsigned int Object::getUID() const noexcept { return uid; }
std::string Object::toString() noexcept { return typeid(*this).name(); }
Object::~Object() = default;
void setUidToParent(Object &obj, ObjectUID parentUid) {
  Object::parentOf[obj.getUID()] = parentUid;
}
void Object::initializeObject(ObjectUID uid_, ObjectUID parent) {
  this->uid = uid_;
  setUidToParent(*this, parent);
};
// template <typename T> T *fastCast(Object *comp, ObjectUID expectedType) {
//   if (!comp)
//     return nullptr;
//
//   int currentUid = comp->getUID();
//
//   // Traverse up the parent chain
//   while (true) {
//     if (currentUid == expectedType) {
//       return static_cast<T *>(comp);
//     }
//
//     auto it = Object::parentOf.find(currentUid);
//     if (it == Object::parentOf.end()) {
//       break; // No parent found, end of chain
//     }
//     currentUid = it->second;
//   }
//
//   return nullptr; // No match found in the ancestry
// }
// template Object *fastCast<Object>(Object *comp, ObjectUID expectedType);
} // namespace reflect