#pragma once
#ifndef FUNCTION_H
#define FUNCTION_H
#include <iostream>
#include <map>
#include <stack>
#include <string>
#include <utility>
#include <cmath>
#include <qmath.h>
#include <stdexcept>

namespace FormulaTree {

enum OperType {
  sin = 1,
  cos = 2,
  mult = '*',
  sum = '+',
  sub = '-',
  divide = '/',
  ln = 3,
  sqrt = 4,
  power = '^',
  open_bracket = '(',
  end_bracket = ')',
  dig = 5,
  arg = 'x'
};

struct Node {
  double num;
  OperType op;
  Node *left = nullptr, *right = nullptr;
};

class Function
{
 public:
  void PostOrder(Node* node);
  void Clear(Node*& root);
  Function();
  Function(std::string input);
  void Generate(std::string input);
  double Solve(double x);
  Node* root = nullptr;
 private:
  double _Solve(Node* node, double x);
  Node* _Generate(std::string input);
  OperType DefineOperType(int ind, std::string input);
  int OperLength(OperType op);
  std::map<OperType, int> operations = {{open_bracket, 1000},
                                        {end_bracket, 0},
                                        {mult, 1},
                                        {sum, 0},
                                        {divide, 1},
                                        {sub, 0},
                                        {power, 2},
                                        {sin, 3},
                                        {cos, 3},
                                        {ln, 3},
                                        {sqrt, 3}};
};
}
#endif // FUNCTION_H
