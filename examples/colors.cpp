int main() {

  // we are going to initialize our colors
  helveta::color::color_t color   = {255, 255, 255, 220}; // white, alpha 220
  helveta::color::color_t _color  = {255, 0, 0};          // red, alpha 255
  helveta::color::color_t __color = {125, 0, 0};          // dark red, alpha 255

  // we are going to get the brightness of color, _color and __color's
  // brightness
  std::cout << "color : " << color.brightness()
            << " _color : " << _color.brightness()
            << " __color : " << __color.brightness() << std::endl;

  // we are going to get the hue of color, _color and __color
  std::cout << "color : " << color.hue() << " _color : " << _color.hue()
            << " __color : " << __color.hue() << std::endl;

  // we are going to get the saturation of color, _color and __color
  std::cout << "color : " << color.saturation()
            << " _color : " << _color.saturation()
            << " __color : " << __color.saturation() << std::endl;

  // we are going to print the alpha of color, _color and __color
  std::cout << "color : " << color.a << " _color : " << _color.a
            << " __color : " << __color.a << std::endl;
}
