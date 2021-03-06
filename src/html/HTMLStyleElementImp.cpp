/*
 * Copyright 2010-2013 Esrille Inc.
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

#include "HTMLStyleElementImp.h"

#include "css/CSSParser.h"
#include "css/CSSStyleSheetImp.h"

#include <boost/bind.hpp>

#include "TextImp.h"
#include "DocumentImp.h"
#include "WindowImp.h"

#include "Test.util.h"

namespace org { namespace w3c { namespace dom { namespace bootstrap {

HTMLStyleElementImp::HTMLStyleElementImp(DocumentImp* ownerDocument) :
    ObjectMixin(ownerDocument, u"style"),
    mutationListener(boost::bind(&HTMLStyleElementImp::handleMutation, this, _1, _2)),
    type(u"text/css"),
    media(u"all"),
    scoped(false),
    styleSheet(0)
{
    addEventListener(u"DOMNodeInserted", &mutationListener, false, EventTargetImp::UseDefault);
    addEventListener(u"DOMNodeRemoved", &mutationListener, false, EventTargetImp::UseDefault);
    // TODO: The following code makes the html paser very slow...
    addEventListener(u"DOMCharacterDataModified", &mutationListener, false, EventTargetImp::UseDefault);
}

HTMLStyleElementImp::HTMLStyleElementImp(HTMLStyleElementImp* org, bool deep) :
    ObjectMixin(org, deep),
    mutationListener(boost::bind(&HTMLStyleElementImp::handleMutation, this, _1, _2)),
    type(org->type),
    media(org->media),
    scoped(org->scoped),
    styleSheet(org->styleSheet) // TODO: make a clone sheet, too?
{
    addEventListener(u"DOMNodeInserted", &mutationListener, false, EventTargetImp::UseDefault);
    addEventListener(u"DOMNodeRemoved", &mutationListener, false, EventTargetImp::UseDefault);
    // TODO: The following code makes the the html paser very slow...
    addEventListener(u"DOMCharacterDataModified", &mutationListener, false, EventTargetImp::UseDefault);
}

void HTMLStyleElementImp::handleMutation(EventListenerImp* listener, events::Event event)
{
    // TODO: update type, media, and scoped. Then check type.

    events::MutationEvent mutation(interface_cast<events::MutationEvent>(event));

    DocumentImp* document = getOwnerDocumentImp();
    if (!document)
        return;

    if (mutation.getType() == u"DOMNodeRemoved" && event.getTarget().self() == this)
        styleSheet = 0;
    else {
        std::u16string content;
        for (Node node = getFirstChild(); node; node = node.getNextSibling()) {
            if (TextImp* text = dynamic_cast<TextImp*>(node.self()))  // TODO better to avoid imp call?
                content += text->getData();
        }
        CSSParser parser;
        styleSheet = parser.parse(document, content);
        if (auto imp = dynamic_cast<CSSStyleSheetImp*>(styleSheet.self())) {
            imp->setOwnerNode(this);
            if (4 <= getLogLevel())
                dumpStyleSheet(std::cerr, imp);
        }
    }
    if (WindowImp* view = document->getDefaultWindow())
        view->setFlags(Box::NEED_SELECTOR_REMATCHING);
    document->resetStyleSheets();
}

// Node
Node HTMLStyleElementImp::cloneNode(bool deep)
{
    return new(std::nothrow) HTMLStyleElementImp(this, deep);
}

// HTMLStyleElement
bool HTMLStyleElementImp::getDisabled()
{
    if (!styleSheet)
        return false;
    return styleSheet.getDisabled();
}

void HTMLStyleElementImp::setDisabled(bool disabled)
{
    if (styleSheet)
        styleSheet.setDisabled(disabled);
}

std::u16string HTMLStyleElementImp::getMedia()
{
    return media;
}

void HTMLStyleElementImp::setMedia(const std::u16string& media)
{
    this->media = media;
}

std::u16string HTMLStyleElementImp::getType()
{
    return type;
}

void HTMLStyleElementImp::setType(const std::u16string& type)
{
    this->type = type;
}

bool HTMLStyleElementImp::getScoped()
{
    return scoped;
}

void HTMLStyleElementImp::setScoped(bool scoped)
{
    this->scoped = scoped;
}

// LinkStyle
stylesheets::StyleSheet HTMLStyleElementImp::getSheet()
{
    return styleSheet;
}

}}}}  // org::w3c::dom::bootstrap
