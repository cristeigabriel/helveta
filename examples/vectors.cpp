int main() {

  // randomize seed
  srand(time(NULL));

  // we're declaring 2 variables that will output a random result and will be
  // initialized as what it was at that time.
  static const int x = rand() % 1920, y = rand() % 1080;
  // we are declaring a 2D vector (same thing can be done with 3D ones) that
  // takes the values
  helveta::vec2_t<int> vec2D = {x, y};
  std::cout << "[0] our current vectors are : " << x << " and " << y
            << std::endl;

  // we are now going to declare another vector that is also randomized
  static const int     _x = rand() % 1280, _y = rand() % 720;
  helveta::vec2_t<int> _vec2D = {_x, _y};
  std::cout << "[1] our current vectors are : " << _x << " and " << _y
            << std::endl;

  // now we are going to calculate the dot product of vec2D and _vec2D
  std::cout << "the dot product of our first and second vector is : "
            << vec2D.dot(_vec2D) << std::endl;

  // now we are going to check if vec2D is equal to _vec2D
  if (vec2D.equal(_vec2D)) {

    std::cout << "vec2D is equal to _vec2D" << std::endl;
  } else {

    std::cout << "vec2D isn't equal to _vec2D" << std::endl;
  }

  // now we are going to check the length of vec2D and _vec2D
  std::cout << "the length of vec2D is : " << vec2D.length()
            << " , and the length of _vec2D is : " << _vec2D.length()
            << std::endl;

  // now we are going to normalize both vectors and print them.
  vec2D.normalize();
  _vec2D.normalize();
  std::cout << "vec2D now is : " << vec2D[0] << ", " << vec2D[1]
            << " and _vec2D is : " << _vec2D[0] << ", " << _vec2D[1];

  return 0;
}
