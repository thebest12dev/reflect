#include "ProcessorRegistry.h"
#include "../Main.h"
#include "Console.h"
#include "core/ui/Button.h"
#include "core/ui/Container.h"
#include "core/ui/Image.h"
#include "core/ui/Label.h"
#include "core/ui/ProgressBar.h"
#include "core/ui/TextField.h"

namespace reflect {
std::unordered_map<std::string, Processor> ProcessorRegistry::mapOfProcessors =
    {{"label",
      [](Window *win,
         tinyxml2::XMLElement *label) -> std::pair<Component &, std::string> {
        // Access attributes
        std::string c = std::string(label->GetText());
        std::string *contents = &c;
        std::string id = label->Attribute("id");
        Vector2 position(std::stoi(label->Attribute("x")),
                         std::stoi(label->Attribute("y")));
        Label *labelComp = new Label(*contents, position);
        labelComp->setFont(label->Attribute("font"));
        labelComp->setFontSize(std::stoi(label->Attribute("fontSize")));
        return {*labelComp, id};
      }},
     {"button",
      [](Window *win,
         tinyxml2::XMLElement *button) -> std::pair<Component &, std::string> {
        // Access attributes
        std::string contents = button->GetText();
        std::string id = button->Attribute("id");
        Vector2 position(std::stoi(button->Attribute("x")),
                         std::stoi(button->Attribute("y")));
        Button *buttonComp = new Button(contents, position);

        buttonComp->setFont(button->Attribute("font"));
        buttonComp->setFontSize(std::stoi(button->Attribute("fontSize")));

        return {*buttonComp, id};
      }},
     {"progressBar",
      [](Window *win,
         tinyxml2::XMLElement *label) -> std::pair<Component &, std::string> {
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
        return {*pb, id};
      }},
     {"image",
      [](Window *win,
         tinyxml2::XMLElement *img) -> std::pair<Component &, std::string> {
        // Access attributes
        std::string id = img->Attribute("id");
        Vector2 position(std::stoi(img->Attribute("x")),
                         std::stoi(img->Attribute("y")));
        Vector2 size(std::stoi(img->Attribute("width")),
                     std::stoi(img->Attribute("height")));
        Image *imgComp = new Image();
        imgComp->setImageLocation(img->Attribute("location"));
        imgComp->setPosition(position);
        imgComp->setSize(size);
        return {*imgComp, id};
      }},
     {"textField",
      [](Window *win,
         tinyxml2::XMLElement *field) -> std::pair<Component &, std::string> {
        // Access attributes
        // Access attributes
        std::string id = field->Attribute("id");
        Vector2 position(std::stoi(field->Attribute("x")),
                         std::stoi(field->Attribute("y")));
        TextField *fieldComp = new TextField();
        fieldComp->setSize(Vector2(std::stoi(field->Attribute("width")),
                                   std::stoi(field->Attribute("height"))));
        fieldComp->setPosition(position);
        return {*fieldComp, id};
      }},
     {"container",
      [](Window *win, tinyxml2::XMLElement *container)
          -> std::pair<Component &, std::string> {
        // Access attributes
        // Access attributes
        std::string id = container->Attribute("id");
        Vector2 position(std::stoi(container->Attribute("x")),
                         std::stoi(container->Attribute("y")));
        Container *containerComp = new Container();

        std::string bgColor = container->Attribute("bgColor");
        reflectDebug("setting window background color...");
        if (bgColor == "systemDefault") {
          containerComp->setColor(255, 255, 255);
        } else {
          if (bgColor.at(0) == '#' && bgColor.length() == 7) {

            const std::string hex = bgColor.substr(1);

            const uint8_t r = stoi(hex.substr(0, 2), nullptr, 16);
            const uint8_t g = stoi(hex.substr(2, 2), nullptr, 16);
            const uint8_t b = stoi(hex.substr(4, 2), nullptr, 16);
            const Color3 color = {r, g, b};
            containerComp->setColor(color);
          } else {
            reflectError("invalid hex color representation");
            std::exit(REFLECT_ERROR_HEX_COLOR_MALFORMED);
          }
        }
        containerComp->setSize(
            Vector2(std::stoi(container->Attribute("width")),
                    std::stoi(container->Attribute("height"))));
        containerComp->setPosition(position);

        // recursive stuff

        for (tinyxml2::XMLElement *element = container->FirstChildElement();
             element != nullptr; element = element->NextSiblingElement()) {
          std::pair<Component &, std::string> comp =
              reflect::ProcessorRegistry::invokeProcessor(element->Name(), win,
                                                          element);
          containerComp->add(comp.first, comp.second);
          Component *ptr = &comp.first;
          addToHeap(ptr);
        }
        return {*containerComp, id};
      }}};

void ProcessorRegistry::createProcessor(Processor processor,
                                        const std::string &elementId) {
  mapOfProcessors[elementId] = processor;
}
std::pair<Component &, std::string>
ProcessorRegistry::invokeProcessor(const std::string &elementId, Window *win,
                                   tinyxml2::XMLElement *element) {
  return mapOfProcessors[elementId](win, element);
}

} // namespace reflect