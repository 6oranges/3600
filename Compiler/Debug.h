
#pragma once
#include <iostream>
#define ShowMessages 1
#if ShowMessages == 1
#define MSG(X) std::cout << X <<std::endl
#else
#define MSG(X)
#endif