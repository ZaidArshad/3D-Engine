#include "app/app.h"
#include <iostream>

int main()
{
     App app = App();
     Status status = app.run();
     return status;
}
