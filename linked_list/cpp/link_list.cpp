#include "general_head.h"

template <class T> link_list<T>::~link_list() { destroy(); }

template <class T> void link_list<T>::create() {
  std::cout << "create" << std::endl;
  head = new node<T>;
  head->next = NULL;
  head->prev = NULL;
  current = head;
}
template <class T> void link_list<T>::add(int count) {
  node<T> *temp;
  // std::cout << "add" << std::endl;
  for (short i = 0; i < count; i++) {
    temp = current;
    // /***/ temp->value = v;
    current->next = new node<T>;
    current = current->next;
    current->next = NULL;
    current->prev = temp;
  }
}

template <class T> void link_list<T>::destroy() {
  node<T> *temp_1 = head; // Markdown the head;
  // std::cin.get();
  do {
    head = head->next;
    delete temp_1;
    // std::cout << (int *)temp_1 << std::endl;
    temp_1 = head;
  } while (head != NULL);
  // std::cout << "destroy()" << std::endl;
}

template <class T> void link_list<T>::rm(int count) {
  if (head->next != NULL) {
    for (int i = 0; i < count; i++) {
      current = current->prev;
      delete current->next;
    }

  } else {
    std::cerr << "You can't delete the head!(But you can change the value.)"
              << std::endl;
  }
}

template <class T> bool link_list<T>::write(T val) {
  node<T> *tmp;
  tmp = current->prev;
  if (std::sizeof(value) != std::sizeof(tmp->value)) {
    return 0;
  } else {
    current->value = val;
    return 1;
  }
}

template <class T> bool link_list<T>::append(T val) {
  add();
  write(val);
  return 1;
}
// int main() {
//   link_list<double> li;
//   li.create();
//   return 0;
// }
