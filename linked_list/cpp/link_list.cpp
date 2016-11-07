#include "general_head.h"
template <class T> link_list<T>::link_list() {
  current = NULL;
  head = NULL;
}
template <class T> link_list<T>::~link_list() { destroy(); }

template <class T> void link_list<T>::add(int count) {
  node<T> *temp;
  // std::cout << "add" << std::endl;
  for (short i = 0; i < count; i++) {
    if (head == NULL) {
      head = new node<T>;
      current = head;
      current->next = NULL;
      current->prev = NULL;
    }
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
  } else
    delete head;
}

template <class T> bool link_list<T>::write(T val) {
  node<T> *tmp;
  tmp = current->prev;
  if (sizeof(val) != sizeof(tmp->value)) {
    return 0;
  } else {
    current->value = val;
    return 1;
  }
}

template <class T> void link_list<T>::append(T val) {
  add();
  write(val);
}
// int main() {
//   link_list<double> li;
//   li.create();
//   return 0;
// }
