#include "arena.h"

#include <iostream>

#include "character.h"
#include "color_rgb.h"

#include "tinyxml2.h"

Arena::Arena(int height) { _height = height; }
Arena::~Arena() {
    delete _background;
}

const int &Arena::height() const { return _height; }
const int &Arena::width() const { return _width; }

const Platform &Arena::background() const { return *_background; }
const std::vector<Platform> &Arena::platforms() const { return _platforms; }
std::list<Character> &Arena::foes() { return _foes; }
const Character &Arena::player() const { return _players.front(); }
const std::vector<Character> &Arena::players() const { return _players; }
std::list<Bullet> &Arena::bullets() { return _bullets; }

void Arena::addBullet(Bullet &&bullet) { _bullets.push_back(std::move(bullet)); }

void Arena::draw() const {
    _background->draw();

    for (const Platform &platform : _platforms)
        platform.draw();

    for (const Character &foe : _foes)
        foe.draw();

    for (const auto &bullet : _bullets)
        bullet.draw();

    player().draw();
}

float Arena::loadFrom(const char *path) {
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError fileLoaded = doc.LoadFile(path);

    if (fileLoaded != tinyxml2::XML_SUCCESS)
        return .0f;

    const tinyxml2::XMLElement *svg = doc.FirstChildElement("svg");
    if (svg == nullptr)
        return 0;

    float backgroundSvgX = .0;
    float backgroundSvgY = .0;
    float backgroundSvgHeight = .0;

    float backgroundSvgToWindowHeightFactor = .0;

    // find background
    for (const tinyxml2::XMLElement *element = svg->FirstChildElement();
         element != nullptr; element = element->NextSiblingElement()) {

        const char *elementName = element->Name();
        
        if (strcmp(elementName, "rect") == 0 && element->Attribute("fill", "blue")) {
            float backgroundSvgWidth = element->FloatAttribute("width");
            backgroundSvgHeight = element->FloatAttribute("height");
            backgroundSvgX = element->FloatAttribute("x");
            backgroundSvgY = element->FloatAttribute("y");

            backgroundSvgToWindowHeightFactor = (float)_height / backgroundSvgHeight;

            _width = backgroundSvgWidth * backgroundSvgToWindowHeightFactor;

            _background = new Platform(
                .0f, .0f,
                _height, _width,
                ColorRgb((uint8_t)66, 66, 240)
            );

            std::cout
                << "Found BACKGROUND"
                << " at svg ("
                << backgroundSvgX
                << ", "
                << backgroundSvgY 
                << "), with dimensions ("
                << backgroundSvgWidth
                << ", "
                << backgroundSvgHeight
                << ")"
                << " Transformed to (0, 0)bl with dimensions ("
                << _width
                << ", "
                << _height
                << ")"
            << std::endl << std::endl;

            break;
        }
    }

    if (_background == nullptr)
        return 0;

    for (const tinyxml2::XMLElement *element = svg->FirstChildElement();
         element != nullptr; element = element->NextSiblingElement()) {

        const char *elementName = element->Name();
        
        if (strcmp(elementName, "rect") == 0 && !element->Attribute("fill", "blue")) {
            float x = element->FloatAttribute("x");
            float y = element->FloatAttribute("y");
            float w = element->FloatAttribute("width");
            float h = element->FloatAttribute("height");

            // because arena's origin is (0, 0) as the instance of Background Platform
            float svgToArenaX = (x - backgroundSvgX) * backgroundSvgToWindowHeightFactor;
            float svgToArenaY = (backgroundSvgHeight - (y - backgroundSvgY + h)) * backgroundSvgToWindowHeightFactor;
            
            _platforms.push_back(Platform(
                svgToArenaX, svgToArenaY,
                h * backgroundSvgToWindowHeightFactor,
                w * backgroundSvgToWindowHeightFactor,
                ColorRgb((uint8_t)92, 58, 10)
            ));

            std::cout
                << "Found PLATFORM"
                << " at svg ("
                << x
                << ", "
                << y
                << "), with dimensions ("
                << w
                << ", "
                << h
                << "), Transformed to ("
                << svgToArenaX
                << ", "
                << svgToArenaY
                << ")bl with dimensions ("
                << w * backgroundSvgToWindowHeightFactor
                << ", "
                << h * backgroundSvgToWindowHeightFactor
                << ")"
            << std::endl << std::endl;

        } else if (strcmp(elementName, "circle") == 0) {
            float cx = element->FloatAttribute("cx");
            float cy = element->FloatAttribute("cy");
            float r = element->FloatAttribute("r");

            float svgToArenaX = (cx - backgroundSvgX - r) * backgroundSvgToWindowHeightFactor;
            float svgToArenaY = (backgroundSvgHeight - (cy - backgroundSvgY + r)) * backgroundSvgToWindowHeightFactor;

            float characterHeight = 2 * r * backgroundSvgToWindowHeightFactor;
            float characterWidth = characterHeight * .34f;

            if (element->Attribute("fill", "green"))
                _players.push_back(Character(
                    svgToArenaX, svgToArenaY,
                    characterHeight, characterWidth,
                    ColorRgb((uint8_t)15, 189, 15)
                ));
            else
                _foes.push_back(Character(
                    svgToArenaX, svgToArenaY,
                    characterHeight, characterWidth,
                    ColorRgb((uint8_t)189, 15, 15)
                ));

            std::cout
                << "Found CHARACTER ["
                << element->Attribute("fill")
                << "] at ("
                << cx
                << ", "
                << cy
                << ") with radius "
                << r
                << ", Transformed to ("
                << svgToArenaX
                << ", "
                << svgToArenaY
                << ")bl with dimensions ("
                << characterWidth
                << ", "
                << characterHeight
                << ")"
            << std::endl << std::endl;
        }
    }

    return backgroundSvgToWindowHeightFactor;
}
