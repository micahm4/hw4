#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
#include <iostream>
#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
int getHeight(Node * root, bool& valid) { //get height function
  if (root == nullptr) {
    return 0;
  }

  if (root->left == nullptr && root->right == nullptr) {
    return 1;
  }

  int left = getHeight(root->left, valid);
  int right = getHeight(root->right, valid);

  if (root->left != nullptr && root->right != nullptr) {
    if (left != right) {
      valid = false;
    }
  }

  if (left > right) {
    return left + 1;
  }
  else {
    return right + 1;
  }
}

bool equalPaths(Node * root)
{
    // Add your code below

    if (root == nullptr) {
      return true;
    }

    bool valid = true;

    getHeight(root, valid);

    return valid;

}

