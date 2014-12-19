#include "../../src/lib/utils.h"

#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

using namespace std;

int main(int argc, char** argv) {

    string s = "FoO";
    s = upper(s);
    assert(s == "FOO");

    cout << "TEST_UTILS [OK]" << endl;
    
    return 0;


}
