#include <memory>

#include "PyObj.hpp"

std::shared_ptr<NoneType> NoneType::NonePtr = std::make_shared<NoneType>();