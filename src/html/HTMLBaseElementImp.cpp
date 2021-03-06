/*
 * Copyright 2010, 2011 Esrille Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "HTMLBaseElementImp.h"

namespace org { namespace w3c { namespace dom { namespace bootstrap {

// Node
Node HTMLBaseElementImp::cloneNode(bool deep)
{
    return new(std::nothrow) HTMLBaseElementImp(this, deep);
}

// HTMLBaseElement
std::u16string HTMLBaseElementImp::getHref()
{
    // TODO: implement me!
    return u"";
}

void HTMLBaseElementImp::setHref(const std::u16string& href)
{
    // TODO: implement me!
}

std::u16string HTMLBaseElementImp::getTarget()
{
    // TODO: implement me!
    return u"";
}

void HTMLBaseElementImp::setTarget(const std::u16string& target)
{
    // TODO: implement me!
}

}}}}  // org::w3c::dom::bootstrap
