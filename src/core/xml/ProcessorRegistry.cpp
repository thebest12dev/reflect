#include "ProcessorRegistry.h"
#include "core/ui/Button.h"
#include "core/ui/Label.h"
#include "core/ui/ProgressBar.h"
#include "core/ui/TextField.h"

namespace reflect {
std::unordered_map<std::string, Processor> ProcessorRegistry::mapOfProcessors =
    {{"label",
      [](Window *win, tinyxml2::XMLElement *label) -> Component & {
        // Access attributes
        std::string c = std::string(label->GetText());
        std::string *contents = &c;
        std::string id = label->Attribute("id");
        Vector2 position(std::stoi(label->Attribute("x")),
                         std::stoi(label->Attribute("y")));
        Label *labelComp = new Label(*contents, position);
        labelComp->setFont(label->Attribute("font"));
        labelComp->setFontSize(std::stoi(label->Attribute("fontSize")));
        win->add(*labelComp, id);
        return *labelComp;
      }},
     {"button",
      [](Window *win, tinyxml2::XMLElement *button) -> Component & {
        // Access attributes
        std::string contents = button->GetText();
        std::string id = button->Attribute("id");
        Vector2 position(std::stoi(button->Attribute("x")),
                         std::stoi(button->Attribute("y")));
        Button *buttonComp = new Button(contents, position);

        buttonComp->setFont(button->Attribute("font"));
        buttonComp->setFontSize(std::stoi(button->Attribute("fontSize")));

        win->add(*buttonComp, id);
        return *buttonComp;
      }},
     {"progressBar",
      [](Window *win, tinyxml2::XMLElement *label) -> Component & {
        // Access attributes
        std::string id = label->Attribute("id");
        Vector2 position(std::stoi(label->Attribute("x")),
                         std::stoi(label->Attribute("y")));
        Vector2 size(std::stoi(label->Attribute("width")),
                     std::stoi(label->Attribute("height")));
        ProgressBar *pb = new ProgressBar();
        pb->setMininumValue(std::stof(label->Attribute("min")));
        pb->setMaximumValue(std::stof(label->Attribute("max")));
        pb->setValue(std::stof(label->Attribute("value")));
        pb->setPosition(position);
        pb->setSize(size);
        win->add(*pb, id);
        return *pb;
      }},
     {"textField", [](Window *win, tinyxml2::XMLElement *field) -> Component & {
        // Access attributes
        // Access attributes
        std::string id = field->Attribute("id");
        Vector2 position(std::stoi(field->Attribute("x")),
                         std::stoi(field->Attribute("y")));
        TextField *fieldComp = new TextField();
        fieldComp->setSize(Vector2(std::stoi(field->Attribute("width")),
                                   std::stoi(field->Attribute("height"))));
        fieldComp->setPosition(position);
        win->add(*fieldComp, id);
        return *fieldComp;
      }}};

void ProcessorRegistry::createProcessor(Processor processor,
                                        const std::string &elementId) {
  mapOfProcessors[elementId] = processor;
}
Component &ProcessorRegistry::invokeProcessor(const std::string &elementId,
                                              Window *win,
                                              tinyxml2::XMLElement *element) {
  return mapOfProcessors[elementId](win, element);
}

} // namespace reflect