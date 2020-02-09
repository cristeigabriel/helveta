int main() {

  // we are going to initialize our colors
  helveta::color::color_t<int> color = {255, 255, 255, 220}; // white, alpha 220
  helveta::color::color_t<int> _color  = {255, 0, 0};        // red, alpha 255
  helveta::color::color_t<int> __color = {125, 0, 0}; // dark red, alpha 255

  // we are going to prind color's brightness then _color's brightness, then
  // __color's brightness
  std::cout << "color : " << color.brightness()
            << " _color : " << _color.brightness()
            << " __color : " << __color.brightness() << std::endl;

  // we are going to get the hues of color, _color and __color
  std::cout << "color : " << color.hue() << " _color : " << _color.hue()
            << " __color : " << __color.hue << std::endl;
  return 0;
}
