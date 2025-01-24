#include "arena.h"
#include "rectangle.h"

#include "tinyxml2.h"

Arena::Arena(int height) { _height = height; }

const int &Arena::height() const { return _height; }

bool Arena::loadFrom(const tinyxml2::XMLDocument &doc) {
    const tinyxml2::XMLElement *svg = doc.FirstChildElement("svg");
    if (svg == nullptr)
        return false;

    float backgroundSvgX = .0;
    float backgroundSvgY = .0;
    float backgroundSvgWidth = .0;
    float backgroundSvgHeight = .0;

    float backgroundSvgToWindowHeightFactor = .0;

    // find background
    for (const tinyxml2::XMLElement *element = svg->FirstChildElement();
         element != nullptr; element = element->NextSiblingElement()) {

        const char *elementName = element->Name();
        
        if (strcmp(elementName, "rect") == 0 && element->Attribute("fill", "blue")) {
            backgroundSvgX = element->FloatAttribute("x");
            backgroundSvgY = element->FloatAttribute("y");
            backgroundSvgWidth = element->FloatAttribute("width");
            backgroundSvgHeight = element->FloatAttribute("height");

            backgroundSvgToWindowHeightFactor = (float)_height / backgroundSvgHeight;

            _background = new Rectangle(
                _height,
                _height * backgroundSvgHeight,
                0.259f, 0.259f, 0.941f, // RGB(66, 66, 240) 
                .0f, .0f
            );

            break;
        }
    }

    if (_background == nullptr)
        return false;

    for (const tinyxml2::XMLElement *element = svg->FirstChildElement();
         element != nullptr; element = element->NextSiblingElement()) {

        const char *elementName = element->Name();
        
        if (strcmp(elementName, "rect") == 0 && !element->Attribute("fill", "blue")) {
            float x = element->FloatAttribute("x");
            float y = element->FloatAttribute("y");
            float w = element->FloatAttribute("width");
            float h = element->FloatAttribute("height");

            // because arena's origin is (0, 0) as the instance of Background Rectangle
            float svgToWindowX = (x - backgroundSvgX) * backgroundSvgToWindowHeightFactor;
            float svgToWindowY = (y - backgroundSvgY - h) * backgroundSvgToWindowHeightFactor;
            
            _platforms.push_back(Rectangle(
                h * backgroundSvgToWindowHeightFactor,
                w * backgroundSvgToWindowHeightFactor,
                0.361f, 0.227f, 0.039f, // rgb(92 58 10)
                svgToWindowX, svgToWindowY
            ));
        } else if (strcmp(elementName, "circle") == 0) {
            float cx = element->FloatAttribute("cx");
            float cy = element->FloatAttribute("cy");
            float r = element->FloatAttribute("r");

            float svgToWindowX = (cx - backgroundSvgX - r) * backgroundSvgToWindowHeightFactor;
            float svgToWindowY = (cy - backgroundSvgY + r) * backgroundSvgToWindowHeightFactor;

            float characterHeight = 2 * r * backgroundSvgToWindowHeightFactor;
            float characterWidth = characterHeight * 0.75f;

            if (element->Attribute("fill", "green"))
                _player = new Character(
                    characterHeight, 
                    characterWidth,
                    0.059f, 0.741f, 0.059f, // rgb(15 189 15)
                    svgToWindowX, svgToWindowY
                );
            else
                _foes.push_back(Character(
                    characterHeight, 
                    characterWidth,
                    0.741f, 0.059f, 0.059f, // rgb(189 15 15)
                    svgToWindowX, svgToWindowY
                ));
        }
    }
}
