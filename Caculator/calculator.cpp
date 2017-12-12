// Copyright(c) 2016 Copyright Tomas Wood All Rights Reserved.
#ifndef _Head_
#define _Head_
#include <cctype>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <queue>
#include <stack>
#include <string>
#endif
using namespace std;
bool out(string n);
bool input(string n);
int w_inside(char op);
int w_outside(char op);
double operate(string op, double a, double b);
int w_inside(char op) {
  // the output privority: the same meaning as
  // the privority in the stack;
  switch (op) {
  case '(':
    return 1;
  case '*':
  case '/':
  case '%':
  case 'x':
    return 5;
  case '+':
  case '-':
    return 3;
  case ')':
    return 6;
  }
  return 0;
}
int w_outside(char op) {
  /*
    input privority, the same as the privority outside the stack;
  */
  switch (op) {
  case '(':
    return 6;
  case '*':
  case '/':
  case '%':
  case 'x':
    return 4;
  case '+':
  case '-':
    return 2;
  case ')':
    return 1;
  }
  return 0;
}

bool input(string n) {

  stack<char> op;       // Store the operator
  queue<string> num;    // The main sequence;
  string a_num = "";    // form a number to put into sequence;
  string temp;          // temp;
  op.push('#');         // Judge the end of the original string;
  int lc = 0, rc = 0;   // Judge the whether the brackets are in pair;
  int count = n.size(); // Make it easy to use snippets

  for (size_t i = 0; i < count; i++) {
    if (n[i] == '(') // Count the left brackets
      lc++;
    if (n[i] == ')') // Count the right brackets
      rc++;
    if (((int)n[i] > 47 && (int)n[i] < 58) || (int)n[i] == 44 ||
        (int)n[i] == 46) {
      a_num = a_num + n[i];
      continue;
    }
    //  Get the numbers and form a string.

    if (n[i] == '-' && (i == 0 || n[i - 1] == '(')) {
      a_num = a_num + n[i];
      continue;
    }
    // Judge for (-1), (-2)*3 form;
    if (n[i] == '+' || n[i] == '-' || n[i] == '*' || n[i] == '/' ||
        n[i] == 'x' || n[i] == '(' || n[i] == ')' || n[i] == '#') {
      num.push(a_num);
      a_num.clear();
      if (n[i] == '#') {
        num.push(a_num);
        break;
      }

      if (w_outside(n[i]) > w_inside(op.top())) {
        // If the input privority ouside bigger the output privority inside:
        op.push(n[i]);

        continue;
      }
      if (w_outside(n[i]) == w_inside(op.top())) {
        op.pop();
        continue;
        // Always useless.
      }
      if (w_outside(n[i]) < w_inside(op.top())) {
        if (n[i] == ')') {
          while (op.top() != '(') {
            temp = op.top();
            num.push(temp);
            op.pop();
          }
          op.pop();
          continue;
        } // brackets judges.
        while (w_outside(n[i]) < w_inside(op.top())) {
          temp = op.top();
          num.push(temp);
          op.pop();
        }              // move the operator from the stack to the queue;
        op.push(n[i]); // input the biggest operator;
        continue;
      }
    }
  }
  if (rc != lc) { // brackets not in pair.
    cerr << "Error!!!" << endl;
    exit(0);
  }
  while (op.top() != '#') {
    temp = op.top();
    num.push(temp);
    op.pop();
  } // Judge end;
  queue<string> filter;
  if (num.size() < 3) {
    cerr << "Error!" << endl;
    exit(0);
  }

  while (!num.empty()) {

    if (num.front() != "" || num.front() != "\0") {
      filter.push(num.front());
    }

    num.pop();
  }
  // filter: delete the wrong string made unknown;
  // THIS IS A PATCH;
  double a, b;
  double result;
  stack<string> stk;
  while (!filter.empty()) {
    while (filter.front() != "+" && filter.front() != "-" &&
           filter.front() != "x" && filter.front() != "*" &&
           filter.front() != "/") {
      stk.push(filter.front());
      filter.pop();
    }

    if (stk.size() >= 2) {
      b = atof(stk.top().c_str());
      stk.pop();
      a = atof(stk.top().c_str());
      stk.pop();
      result = operate(filter.front(), a, b);
      stk.push(to_string(result));
      filter.pop();
    } else {
      std::cerr << "Error!" << '\n';
      exit(0);
    }
  }
  string f_res = stk.top();
  // Format the result.
  for (size_t i = f_res.size() - 1; f_res[i] != '.'; i--) {
    if (f_res[i] == '0') {
      f_res.erase(i);
      continue;
    }
    break;
  }
  if (atof(f_res.c_str()) == (int)atof(f_res.c_str())) {
    f_res = to_string((int)atof(f_res.c_str()));
  }
  out(f_res);
  return 1;
}
double operate(string op, double a, double b) {

  switch (op[0]) {
  case '+':
    return a + b;
  case '-':
    return a - b;
  case '*':
  case 'x':
    return a * b;
  case '/':
    if (b == 0) {
      cerr << "Errr!" << endl;
      exit(0);
    } else
      return a / b;
  }
  return -1;
}
bool out(string n) {
  /*
   __    __  __        __   __
  |  | | __| __| |__| |__  |__
  |__| ||__  __|    |  __| |__|

   */
  int k;
  if (n.size() <= 13)
    k = n.size();
  else
    k = 13;

  for (size_t i = 0; i < k; i++) {
    if (isdigit(n[i])) {
      // cout<<"___"<<n[i]<<endl;
      switch (n[i]) {
      case '-':
        printf("     ");
        break;
      case '0':
        printf(" __  ");
        break;
      case '1':
        printf("     ");
        break;
      case '2':
        printf(" __  ");
        break;
      case '3':
        printf(" __  ");
        break;
      case '4':
        printf("     ");
        break;
      case '5':
        printf(" __  ");
        break;
      case '6':
        printf(" __  ");
        break;
      case '7':
        printf(" __  ");
        break;
      case '8':
        printf(" __  ");
        break;
      case '9':
        printf(" __  ");
        break;
      }
    } else
      printf("   "); // dot: 3
  }
  cout << endl;
  for (size_t i = 0; i < k; i++) {
    if (isdigit(n[i])) {
      switch (n[i]) {
      case '-':
        printf("     ");
        break;
      case '0':
        printf("|  | ");
        break;
      case '1':
        printf("  |  ");
        break;
      case '2':
        printf(" __| ");
        break;
      case '3':
        printf(" __| ");
        break;
      case '4':
        printf("|__| ");
        break;
      case '5':
        printf("|__  ");
        break;
      case '6':
        printf("|__  ");
        break;
      case '7':
        printf("   | ");
        break;
      case '8':
        printf("|__| ");
        break;
      case '9':
        printf("|__| ");
        break;
      }
    } else if (n[i] == '-') {
      printf("__ ");
    } else
      printf("   "); // dot: 3
  }
  cout << endl;
  for (size_t i = 0; i < k; i++) {
    if (isdigit(n[i])) {
      switch (n[i]) {
      case '-':
        printf(" __  ");
        break;
      case '0':
        printf("|__| ");
        break;
      case '1':
        printf("  |  ");
        break;
      case '2':
        printf("|__  ");
        break;
      case '3':
        printf(" __| ");
        break;
      case '4':
        printf("   | ");
        break;
      case '5':
        printf(" __| ");
        break;
      case '6':
        printf("|__| ");
        break;
      case '7':
        printf("   | ");
        break;
      case '8':
        printf("|__| ");
        break;
      case '9':
        printf("   | ");
        break;
      }
    } else if (n[i] == '-')
      printf("   ");
    else
      printf(" * "); // dot: 3
  }
  if (n.size() > 13)
    printf("Err");
  cout << endl << endl;
  return 1;
}

int main() {
  // int a,b;
  printf("===================\n");
  printf("Awfully calculator\n\n");
  printf("Input the formula:\n");
  printf("===================\n\n");
  bool rw;
  string str;
  while (getline(cin, str) == 1) {
    if (str == "\0" || str == "\n") {
      exit(0);
    }
    str = str + "#";
    cin.clear();
    cin.sync();
    rw = input(str);
  }

  return 0;
}
