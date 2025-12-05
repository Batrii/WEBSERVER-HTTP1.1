#pragma once
#include <clock.hpp>
#include <console.hpp>
#include <fcntl.h>
#include <vector>

int parseArgument(char* argument, clock_tt startClock);

void pathFd(int fd, clock_tt startClock);
void autoConfig(clock_tt startClock);