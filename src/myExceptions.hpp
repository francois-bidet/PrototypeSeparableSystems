#pragma once

#include <exception>

class TooManyIterations : public std::exception {};
class InvalidContraction : public std::exception {};
