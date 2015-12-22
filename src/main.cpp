#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include "virtual-canvas.h"
#include "graphics.h"

void sigint_handler(int sig)
{
    exit(0);
}

void setup_sighandler()
{
    struct sigaction handler;
    handler.sa_handler = sigint_handler;

    sigemptyset(&handler.sa_mask);
    handler.sa_flags = 0;

    sigaction(SIGINT, &handler, NULL);
}

int main_main(int argc, char* args[])
{
    setup_sighandler();

    VirtualCanvas canvas(16, 2, 0);

    rgb_matrix::DrawCircle(&canvas, 5, 5, 5, rgb_matrix::Color(0xFF, 0x00, 0x00));
    canvas.StartSimulation();

    return 0;
}
