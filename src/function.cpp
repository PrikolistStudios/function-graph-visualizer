#include "function.h"

FormulaTree::Node *FormulaTree::Function::_Generate(std::string input) {
  std::stack<std::pair<OperType, int>> opers;
  std::stack<Node *> trees;
  input.insert(0, "(");
  input.push_back(')');
  int cur_ind = 0;
  while (cur_ind<input.size()) {
    OperType op_t = DefineOperType(cur_ind, input);
    if (op_t == dig) {
      Node *new_node = new Node;
      auto tmp = input.substr(cur_ind);
      new_node->num = std::stod(tmp);
      new_node->op = dig;
      int num_size = 0;
      if (input[cur_ind] == '-') {
//        num_size++;
        cur_ind++;
      }
      bool found_dot = 0;
      for (int i = cur_ind;
           i < input.size() && (std::isdigit(input[i]) || input[i] == '.');
           ++i) {
        if (input[i] == '.' && found_dot) break;
        if (input[i] == '.') found_dot = 1;
        num_size++;
      }

      trees.push(new_node);
      cur_ind += num_size;
    }  else if (op_t == arg) {
      Node *new_node = new Node;
      new_node->op = arg;
      trees.push(new_node);
      cur_ind++;
    } else {
      std::pair<OperType, int> oper = {op_t, operations[op_t]};
      if (op_t == open_bracket) {
        opers.push(oper);
      } else if (op_t == end_bracket) {
       while (opers.top().first != '(') {
          auto to_add = opers.top();
          opers.pop();
          Node *new_node = new Node;
          new_node->op = to_add.first;
          if(new_node->op==sin||new_node->op==cos || new_node->op==sqrt || new_node->op==ln) {
            new_node->left = trees.top();
            trees.pop();
            trees.push(new_node);
          } else {
            new_node->right = trees.top();
            trees.pop();
            new_node->left = trees.top();
            trees.pop();
            trees.push(new_node);
          }
        }
        opers.pop();
      } else {
        while (oper.second <= opers.top().second && opers.top().first != '(') {
          auto to_add = opers.top();
          opers.pop();
          Node *new_node = new Node;
          new_node->op = to_add.first;
          if(new_node->op==sin||new_node->op==cos || new_node->op==sqrt || new_node->op==ln) {
            new_node->left = trees.top();
            trees.pop();
            trees.push(new_node);
          } else {
            new_node->right = trees.top();
            trees.pop();
            new_node->left = trees.top();
            trees.pop();
            trees.push(new_node);
          }
        }
        opers.push(oper);
      }
      cur_ind += OperLength(op_t);
    }
  }
  return trees.top();
}

FormulaTree::OperType FormulaTree::Function::DefineOperType(int ind,
                                                            std::string input) {
  try {
    auto tmp = input.substr(ind);
    std::stod(tmp);
    if (input[ind] == '+' || (input[ind] == '-' && input[ind - 1] != '('))
      throw std::invalid_argument("penis");
    return dig;
  } catch(...) {
    if (input[ind] == 'x') return arg;
    if (input.substr(ind, 3) == "sin") return sin;
    if (input.substr(ind, 3) == "cos") return cos;
    if (input.substr(ind, 4) == "sqrt") return sqrt;
    if (input.substr(ind, 2) == "ln") return ln;
    return static_cast<OperType>(input[ind]);
  }
}

int FormulaTree::Function::OperLength(OperType op) {
  if (op == sin || op == cos) return 3;
  if (op == ln) return 2;
  if (op == sqrt) return 4;
  return 1;
}

double FormulaTree::Function::_Solve(FormulaTree::Node *node, double x) {
  if (!node) return 0;
  if (node->op == dig) return node->num;
  if (node->op == sum) return _Solve(node->left, x) + _Solve(node->right, x);
  if (node->op == mult) return _Solve(node->left, x) * _Solve(node->right, x);
  if (node->op == sub) return _Solve(node->left, x) - _Solve(node->right, x);
  if (node->op == divide) {
    auto tmp = _Solve(node->right, x);
    if (qFuzzyCompare(tmp, 0)) {
      throw std::invalid_argument("Zero division");
    }
    return _Solve(node->left, x) / tmp;
  }
  if (node->op == power)
    return qPow(_Solve(node->left, x), _Solve(node->right, x));
  if (node->op == sin) {
    return qSin(_Solve(node->left, x));
  }
  if (node->op == cos) {
    return qCos(_Solve(node->left, x));
  }
  if (node->op == sqrt) {
    auto tmp = _Solve(node->left, x);
    if (tmp < 0) throw std::invalid_argument("Negative sqrt");
    return std::sqrt(tmp);
  }
  if (node->op == ln) {
    auto tmp = _Solve(node->left, x);
    if (tmp < 0) throw std::invalid_argument("Negative ln");
    return qLn(tmp);
  }
  if (node->op == arg) return x;
}

void FormulaTree::Function::PostOrder(FormulaTree::Node *node) {
  if (!node) return;
  PostOrder(node->left);
  PostOrder(node->right);
  if (!node->left && !node->right) {
    std::cout << node->num << ' ';
  } else {
    std::cout << node->op << ' ';
  }
}

void FormulaTree::Function::Clear(FormulaTree::Node *&root) {
  if (root == nullptr) return;
  Clear(root->left);
  Clear(root->right);
  delete root;
  root = nullptr;
}

FormulaTree::Function::Function() {}

FormulaTree::Function::Function(std::string input) { root = _Generate(input); }

void FormulaTree::Function::Generate(std::string input) {
  Clear(root);
  root = _Generate(input);
}

double FormulaTree::Function::Solve(double x) { return _Solve(root, x); }
